// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2021 The Bitcoin Core developers
// Copyright (c) 2021-2023 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"
#include "chain.h"
#include "config.h"
#include "consensus/params.h"
#include "primitives/block.h"
#include "uint256.h"
#include "validation.h"

static std::atomic<const CBlockIndex *> cachedAnchor{nullptr};

void ResetASERTAnchorBlockCache() noexcept {
    cachedAnchor = nullptr;
}

/**
 * Returns a pointer to the anchor block used for ASERT.
 * As anchor we use the first block for which IsASERTEnabled() returns true.
 * This block happens to be the last block which was mined under the old DAA
 * rules.
 *
 * This function is meant to be removed some time after the upgrade, once
 * the anchor block is deeply buried, and behind a hard-coded checkpoint.
 *
 * Preconditions: - pindex must not be nullptr
 *                - pindex must satisfy: IsASERTEnabled(params, pindex) == true
 * Postcondition: Returns a pointer to the first (lowest) block for which
 *                IsASERTEnabled is true, and for which IsASERTEnabled(pprev)
 *                is false (or for which pprev is nullptr). The return value may
 *                be pindex itself.
 */
static const CBlockIndex *GetASERTAnchorBlock(const CBlockIndex *const pindex,
                                              const Config &config) {
    assert(pindex);

    // - We check if we have a cached result, and if we do and it is really the
    //   ancestor of pindex, then we return it.
    //
    // - If we do not or if the cached result is not the ancestor of pindex,
    //   then we proceed with the more expensive walk back to find the ASERT
    //   anchor block.
    //
    // CBlockIndex::GetAncestor() is reasonably efficient; it uses CBlockIndex::pskip
    // Note that if pindex == cachedAnchor, GetAncestor() here will return cachedAnchor,
    // which is what we want.
    const CBlockIndex *lastCached = cachedAnchor.load();
    if (lastCached && pindex->GetAncestor(lastCached->GetHeight()) == lastCached)
        return lastCached;

    // Slow path: walk back until we find the first ancestor for which IsASERTEnabled() == true.
    const CBlockIndex *anchor = pindex;

    while (anchor->GetPrev()) {
        // first, skip backwards testing IsASERTEnabled
        // The below code leverages CBlockIndex::pskip to walk back efficiently.
        if (IsASERTEnabled(config, anchor->GetSkip())) {
            // skip backward
            anchor = anchor->GetSkip();
            continue; // continue skipping
        }
        // cannot skip here, walk back by 1
        if (!IsASERTEnabled(config, anchor->GetPrev())) {
            // found it -- highest block where Axion is not enabled is anchor->pprev, and
            // anchor points to the first block for which IsASERTEnabled() == true
            break;
        }
        anchor = anchor->GetPrev();
    }

    // Overwrite the cache with the anchor we found. More likely than not, the next
    // time we are asked to validate a header it will be part of same / similar chain, not
    // some other unrelated chain with a totally different anchor.
    cachedAnchor = anchor;

    return anchor;
}

/**
 * Compute the next required proof of work using an absolutely scheduled
 * exponentially weighted target (ASERT).
 *
 * With ASERT, we define an ideal schedule for block issuance (e.g. 1 block every 600 seconds), and we calculate the
 * difficulty based on how far the most recent block's timestamp is ahead of or behind that schedule.
 * We set our targets (difficulty) exponentially. For every [nHalfLife] seconds ahead of or behind schedule we get, we
 * double or halve the difficulty.
 */
static uint32_t GetNextASERTWorkRequired(const CBlockIndex *pindexPrev,
                                         const CBlockHeader *pblock,
                                         const Config &config,
                                         const Consensus::Params::ASERTAnchor &anchorParams) noexcept {
    // This cannot handle the genesis block and early blocks in general.
    assert(pindexPrev != nullptr);

    // We make no further assumptions other than the height of the prev block must be >= that of the anchor block.
    assert(pindexPrev->GetHeight() >= anchorParams.nHeight);

    const Consensus::Params &params = config.GetChainParams().GetConsensus();

    const arith_uint256 powLimit = UintToArith256(params.powLimit);

    // For nTimeDiff calculation, the timestamp of the parent to the anchor block is used,
    // as per the absolute formulation of ASERT.
    // This is somewhat counterintuitive since it is referred to as the anchor timestamp, but
    // as per the formula the timestamp of block M-1 must be used if the anchor is M.
    assert(pindexPrev->GetPrev() != nullptr);

    const arith_uint256 refBlockTarget = arith_uint256().SetCompact(anchorParams.nBits);

    // Time difference is from anchor block's parent block's timestamp
    const int64_t nTimeDiff = pindexPrev->GetBlockTime() - anchorParams.nPrevBlockTime;
    // Height difference is from current block to anchor block
    const int nHeightDiff = pindexPrev->GetHeight() - anchorParams.nHeight;

    // Do the actual target adaptation calculation in separate
    // CalculateASERT() function
    arith_uint256 nextTarget = CalculateASERT(refBlockTarget,
                                              params.nPowTargetSpacing,
                                              nTimeDiff,
                                              nHeightDiff,
                                              powLimit,
                                              params.nASERTHalfLife);

    // CalculateASERT() already clamps to powLimit.
    return nextTarget.GetCompact();
}

uint32_t GetNextASERTWorkRequired(const CBlockIndex *pindexPrev,
                                  const CBlockHeader *pblock,
                                  const Config &config,
                                  const CBlockIndex *pindexAnchorBlock) noexcept {

    const Consensus::Params &params = config.GetChainParams().GetConsensus();

    // If hard-coded params exist for this chain, we use those
    if (params.asertAnchorParams) {
        return GetNextASERTWorkRequired(pindexPrev, pblock, config, *params.asertAnchorParams);
    }

    // Otherwise, caller should have specified the anchor block (chain where it has not yet
    // activated such as ScaleNet).
    //
    // Anchor block is the block on which all ASERT scheduling calculations are based.
    // It too must exist, and it must have a valid parent.
    assert(pindexAnchorBlock != nullptr);

    // Note: time difference is to parent of anchor block (or to anchor block itself iff anchor is genesis).
    //       (according to absolute formulation of ASERT)
    const auto anchorTime = pindexAnchorBlock->GetPrev()
                                    ? pindexAnchorBlock->GetPrev()->GetBlockTime()
                                    : pindexAnchorBlock->GetBlockTime();

    const Consensus::Params::ASERTAnchor anchorParams{
        pindexAnchorBlock->GetHeight(),
        pindexAnchorBlock->GetBits(),
        anchorTime
    };

    // Call the overloaded function that does the actual calculation using anchorParams
    return GetNextASERTWorkRequired(pindexPrev, pblock, config, anchorParams);
}

// ASERT calculation function.
// Clamps to powLimit.
arith_uint256 CalculateASERT(const arith_uint256 &refTarget,
                             const int64_t nPowTargetSpacing,
                             const int64_t nTimeDiff,
                             const int64_t nHeightDiff,
                             const arith_uint256 &powLimit,
                             const int64_t nHalfLife) noexcept {

    // Input target must never be zero nor exceed powLimit.
    assert(refTarget > 0 && refTarget <= powLimit);

    // We need some leading zero bits in powLimit in order to have room to handle
    // overflows easily. 32 leading zero bits is more than enough.
    assert((powLimit >> 224) == 0);

    // Height diff should NOT be negative.
    assert(nHeightDiff >= 0);

    // It will be helpful when reading what follows, to remember that
    // nextTarget is adapted from anchor block target value.

    // Ultimately, we want to approximate the following ASERT formula, using only integer (fixed-point) math:
    //     new_target = old_target * 2^((blocks_time - IDEAL_BLOCK_TIME * (height_diff + 1)) / nHalfLife)

    // First, we'll calculate the exponent:
    assert( llabs(nTimeDiff - nPowTargetSpacing * nHeightDiff) < (1ll << (63 - 16)) );
    const int64_t exponent = ((nTimeDiff - nPowTargetSpacing * (nHeightDiff + 1)) * 65536) / nHalfLife;

    // Next, we use the 2^x = 2 * 2^(x-1) identity to shift our exponent into the [0, 1) interval.
    // The truncated exponent tells us how many shifts we need to do
    // Note1: This needs to be a right shift. Right shift rounds downward (floored division),
    //        whereas integer division in C++ rounds towards zero (truncated division).
    // Note2: This algorithm uses arithmetic shifts of negative numbers. This
    //        is unpecified but very common behavior for C++ compilers before
    //        C++20, and standard with C++20. We must check this behavior e.g.
    //        using static_assert.
    static_assert(int64_t(-1) >> 1 == int64_t(-1),
                  "ASERT algorithm needs arithmetic shift support");

    // Now we compute an approximated target * 2^(exponent/65536.0)

    // First decompose exponent into 'integer' and 'fractional' parts:
    int64_t shifts = exponent >> 16;
    const auto frac = uint16_t(exponent);
    assert(exponent == (shifts * 65536) + frac);

    // multiply target by 65536 * 2^(fractional part)
    // 2^x ~= (1 + 0.695502049*x + 0.2262698*x**2 + 0.0782318*x**3) for 0 <= x < 1
    // Error versus actual 2^x is less than 0.013%.
    const uint32_t factor = 65536 + ((
        + 195766423245049ull * frac
        + 971821376ull * frac * frac
        + 5127ull * frac * frac * frac
        + (1ull << 47)
        ) >> 48);
    // this is always < 2^241 since refTarget < 2^224
    arith_uint256 nextTarget = refTarget * factor;

    // multiply by 2^(integer part) / 65536
    shifts -= 16;
    if (shifts <= 0) {
        nextTarget >>= -shifts;
    } else {
        // Detect overflow that would discard high bits
        const auto nextTargetShifted = nextTarget << shifts;
        if ((nextTargetShifted >> shifts) != nextTarget) {
            // If we had wider integers, the final value of nextTarget would
            // be >= 2^256 so it would have just ended up as powLimit anyway.
            nextTarget = powLimit;
        } else {
            // Shifting produced no overflow, can assign value
            nextTarget = nextTargetShifted;
        }
    }

    if (nextTarget == 0) {
        // 0 is not a valid target, but 1 is.
        nextTarget = arith_uint256(1);
    } else if (nextTarget > powLimit) {
        nextTarget = powLimit;
    }

    // we return from only 1 place for copy elision
    return nextTarget;
}



/**
 * Compute the next required proof of work using the legacy MVC difficulty
 * adjustment + Emergency Difficulty Adjustment (EDA).
 */
static uint32_t GetNextEDAWorkRequired(const CBlockIndex *pindexPrev,
                                       const CBlockHeader *pblock,
                                       const Config &config) {
    const Consensus::Params &params = config.GetChainParams().GetConsensus();

    // Only change once per difficulty adjustment interval
    int32_t nHeight = pindexPrev->GetHeight() + 1;
    if (nHeight % params.DifficultyAdjustmentInterval() == 0) {
        // Go back by what we want to be 14 days worth of blocks
        assert(nHeight >= params.DifficultyAdjustmentInterval());
        int32_t nHeightFirst = nHeight - params.DifficultyAdjustmentInterval();
        const CBlockIndex *pindexFirst = pindexPrev->GetAncestor(nHeightFirst);
        assert(pindexFirst);

        return CalculateNextWorkRequired(pindexPrev,
                                         pindexFirst->GetBlockTime(), config);
    }

    const uint32_t nProofOfWorkLimit =
        UintToArith256(params.powLimit).GetCompact();

    if (params.fPowAllowMinDifficultyBlocks) {
        // Special difficulty rule for testnet:
        // If the new block's timestamp is more than 2* 10 minutes then allow
        // mining of a min-difficulty block.
        if (pblock->GetBlockTime() >
            pindexPrev->GetBlockTime() + 2 * params.nPowTargetSpacing) {
            return nProofOfWorkLimit;
        }

        // Return the last non-special-min-difficulty-rules-block
        const CBlockIndex *pindex = pindexPrev;
        while (!pindex->IsGenesis() &&
               pindex->GetHeight() % params.DifficultyAdjustmentInterval() != 0 &&
               pindex->GetBits() == nProofOfWorkLimit) {
            pindex = pindex->GetPrev();
        }

        return pindex->GetBits();
    }

    // We can't go below the minimum, so bail early.
    uint32_t nBits = pindexPrev->GetBits();
    if (nBits == nProofOfWorkLimit) {
        return nProofOfWorkLimit;
    }

    // If producing the last 6 blocks took less than 12h, we keep the same
    // difficulty.
    const CBlockIndex *pindex6 = pindexPrev->GetAncestor(nHeight - 7);
    assert(pindex6);
    int64_t mtp6blocks =
        pindexPrev->GetMedianTimePast() - pindex6->GetMedianTimePast();
    if (mtp6blocks < 12 * 3600) {
        return nBits;
    }

    // If producing the last 6 blocks took more than 12h, increase the
    // difficulty target by 1/4 (which reduces the difficulty by 20%).
    // This ensures that the chain does not get stuck in case we lose
    // hashrate abruptly.
    arith_uint256 nPow;
    nPow.SetCompact(nBits);
    nPow += (nPow >> 2);

    // Make sure we do not go below allowed values.
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    if (nPow > bnPowLimit) nPow = bnPowLimit;

    return nPow.GetCompact();
}

uint32_t GetNextWorkRequired(const CBlockIndex *pindexPrev,
                             const CBlockHeader *pblock, const Config &config) {
    const Consensus::Params &params = config.GetChainParams().GetConsensus();

    // Genesis block
    if (pindexPrev == nullptr || pindexPrev->GetHeight() <= 6) {
        return UintToArith256(params.powLimit).GetCompact();
    }
    
    if (IsASERTEnabled(config, pindexPrev)) {
        const CBlockIndex *panchorBlock = nullptr;
        if (!params.asertAnchorParams) {
            // No hard-coded anchor params -- find the anchor block dynamically
            panchorBlock = GetASERTAnchorBlock(pindexPrev, config);
        }
        return GetNextASERTWorkRequired(pindexPrev, pblock, config, panchorBlock);
    }

    if (IsDAAEnabled(config, pindexPrev->GetHeight())) {
        return GetNextCashWorkRequired(pindexPrev, pblock, config);
    }

    return GetNextEDAWorkRequired(pindexPrev, pblock, config);
}

uint32_t CalculateNextWorkRequired(const CBlockIndex *pindexPrev,
                                   int64_t nFirstBlockTime,
                                   const Config &config) {
    const Consensus::Params &params = config.GetChainParams().GetConsensus();

    // Limit adjustment step
    int64_t nActualTimespan = pindexPrev->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan / 4) {
        nActualTimespan = params.nPowTargetTimespan / 4;
    }

    if (nActualTimespan > params.nPowTargetTimespan * 4) {
        nActualTimespan = params.nPowTargetTimespan * 4;
    }

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;
    bnNew.SetCompact(pindexPrev->GetBits());
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;

    if (bnNew > bnPowLimit) bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, uint32_t nBits, const Config &config) {
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;


    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);
    
    // Check range
    if (fNegative || bnTarget == 0 || fOverflow ||
        bnTarget >
            UintToArith256(config.GetChainParams().GetConsensus().powLimit)) {
        return false;
    }

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget) {
        return false;
    }

    return true;
}

/**
 * Compute a target based on the work done between 2 blocks and the time
 * required to produce that work.
 */
static arith_uint256 ComputeTarget(const uint32_t offset,
                                   const CBlockIndex *pindexFirst,
                                   const CBlockIndex *pindexLast,
                                   const Consensus::Params &params) {
    assert(pindexLast->GetHeight() > pindexFirst->GetHeight());

    /**
     * From the total work done and the time it took to produce that much work,
     * we can deduce how much work we expect to be produced in the targeted time
     * between blocks.
     */
    arith_uint256 work = pindexLast->GetChainWork() - pindexFirst->GetChainWork();
    work *= params.nPowTargetSpacing;

    // In order to avoid difficulty cliffs, we bound the amplitude of the
    // adjustment we are going to do to a factor in [0.5, 2].
    int64_t nActualTimespan =
        pindexLast->GetBlockTime() - pindexFirst->GetBlockTime();
    if (nActualTimespan > 2 * offset * params.nPowTargetSpacing) {
        nActualTimespan = 2 * offset * params.nPowTargetSpacing;
    } else if (nActualTimespan < offset / 2 * params.nPowTargetSpacing) {
        nActualTimespan = offset / 2 * params.nPowTargetSpacing;
    }

    work /= nActualTimespan;

    /**
     * We need to compute T = (2^256 / W) - 1 but 2^256 doesn't fit in 256 bits.
     * By expressing 1 as W / W, we get (2^256 - W) / W, and we can compute
     * 2^256 - W as the complement of W.
     */
    return (-work) / work;
}

/**
 * To reduce the impact of timestamp manipulation, we select the block we are
 * basing our computation on via a median of 3.
 */
static const CBlockIndex *GetSuitableBlock(const CBlockIndex *pindex) {
    assert(pindex->GetHeight() >= 3);

    /**
     * In order to avoid a block with a very skewed timestamp having too much
     * influence, we select the median of the 3 top most blocks as a starting
     * point.
     */
    const CBlockIndex *blocks[3];
    blocks[2] = pindex;
    blocks[1] = pindex->GetPrev();
    blocks[0] = blocks[1]->GetPrev();

    // Sorting network.
    if (blocks[0]->GetBlockTime() > blocks[2]->GetBlockTime())
    {
        std::swap(blocks[0], blocks[2]);
    }

    if (blocks[0]->GetBlockTime() > blocks[1]->GetBlockTime())
    {
        std::swap(blocks[0], blocks[1]);
    }

    if (blocks[1]->GetBlockTime() > blocks[2]->GetBlockTime())
    {
        std::swap(blocks[1], blocks[2]);
    }

    // We should have our candidate in the middle now.
    return blocks[1];
}

/**
 * Compute the next required proof of work using a weighted average of the
 * estimated hashrate per block.
 *
 * Using a weighted average ensure that the timestamp parameter cancels out in
 * most of the calculation - except for the timestamp of the first and last
 * block. Because timestamps are the least trustworthy information we have as
 * input, this ensures the algorithm is more resistant to malicious inputs.
 */
uint32_t GetNextCashWorkRequired(const CBlockIndex *pindexPrev,
                                 const CBlockHeader *pblock,
                                 const Config &config) {
    const Consensus::Params &params = config.GetChainParams().GetConsensus();
    const int32_t nHeight = pindexPrev->GetHeight();
    // This cannot handle the genesis block and early blocks in general.
    assert(pindexPrev);

    // Special difficulty rule for testnet:
    // If the new block's timestamp is more than 2* 10 minutes then allow
    // mining of a min-difficulty block.
    if (config.GetChainParams().NetworkIDString() == CBaseChainParams::TESTNET) {
        if (params.fPowAllowMinDifficultyBlocks &&
            (pblock->GetBlockTime() >
            pindexPrev->GetBlockTime() + 2 * params.nPowTargetSpacing)) {
            return UintToArith256(params.powLimit).GetCompact();
        }
    } else {
        if (params.fPowAllowMinDifficultyBlocks &&
            nHeight >10 && 
            nHeight <2200 && 
            (pblock->GetBlockTime() >
            pindexPrev->GetBlockTime() + 2 * params.nPowTargetSpacing)) {
            return UintToArith256(params.powLimit).GetCompact();
        }
    }


    // Compute the difficulty based on the full adjustment interval.
    
    assert(nHeight >= 6);

    // Get the last suitable block of the difficulty interval.
    const CBlockIndex *pindexLast = GetSuitableBlock(pindexPrev);
    assert(pindexLast);

    // Get the first suitable block of the difficulty interval.
    uint32_t nHeightFirst = 0;
    if (nHeight <= 144 + 3) {
        nHeightFirst = 3;
    } else {
        nHeightFirst = nHeight - 144;
    }

    const CBlockIndex *pindexFirst =
        GetSuitableBlock(pindexPrev->GetAncestor(nHeightFirst));
    assert(pindexFirst);

    // Compute the target based on time and work done during the interval.
    const arith_uint256 nextTarget =
        ComputeTarget(nHeight - nHeightFirst, pindexFirst, pindexLast, params);

    const arith_uint256 powLimit = UintToArith256(params.powLimit);
    if (nextTarget > powLimit) {
        return powLimit.GetCompact();
    }

    return nextTarget.GetCompact();
}
