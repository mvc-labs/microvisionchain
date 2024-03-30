// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2021 The Bitcoin Core developers
// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MVC_POW_H
#define MVC_POW_H

#include <cstdint>
#include "arith_uint256.h"


class CBlockHeader;
class CBlockIndex;
class Config;
class uint256;

namespace Consensus {
struct Params;
}

uint32_t GetNextWorkRequired(const CBlockIndex *pindexPrev,
                             const CBlockHeader *pblock, const Config &config);
uint32_t CalculateNextWorkRequired(const CBlockIndex *pindexPrev,
                                   int64_t nFirstBlockTime,
                                   const Config &config);

/**
 * Check whether a block hash satisfies the proof-of-work requirement specified
 * by nBits
 */
bool CheckProofOfWork(uint256 hash, uint32_t nBits, const Config &config);

/**
 * MVC's difficulty adjustment mechanism.
 */
uint32_t GetNextCashWorkRequired(const CBlockIndex *pindexPrev,
                                 const CBlockHeader *pblock,
                                 const Config &config);

arith_uint256 CalculateASERT(const arith_uint256 &refTarget,
                             const int64_t nPowTargetSpacing,
                             const int64_t nTimeDiff,
                             const int64_t nHeightDiff,
                             const arith_uint256 &powLimit,
                             const int64_t nHalfLife) noexcept;

uint32_t GetNextASERTWorkRequired(const CBlockIndex *pindexPrev,
                                  const CBlockHeader *pblock,
                                  const Consensus::Params &params,
                                  const CBlockIndex *pindexAnchorBlock) noexcept;

/**
 * ASERT caches a special block index for efficiency. If block indices are
 * freed then this needs to be called to ensure no dangling pointer when a new
 * block tree is created.
 * (this is temporary and will be removed after the ASERT constants are fixed)
 */
void ResetASERTAnchorBlockCache() noexcept;

#endif // MVC_POW_H
