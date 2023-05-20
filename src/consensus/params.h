// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2022 The Bitcoin Core developers
// Copyright (c) 2021-2023 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MVC_CONSENSUS_PARAMS_H
#define MVC_CONSENSUS_PARAMS_H

#include "uint256.h"

namespace Consensus {

/**
 * Parameters that influence chain consensus.
 */
struct Params {
    uint256 hashGenesisBlock;
    int nSubsidyHalvingInterval;
    /** Block height and hash which BIP34 BIP65 BIP66 BIP68 BIP112 and BIP113 become active*/
    int32_t BitcoinSoftForksHeight;
    uint256 BitcoinSoftForksHash;
    /** Block height at which UAHF kicks in */
    int32_t uahfHeight;
    /** Block height at which the new DAA becomes active */
    int32_t daaHeight;
    /** Block height at which the Genesis becomes active.
      * The specified block height is the height of the block where the new rules are active.
      * It is not the height of the last block with the old rules.
      */
    int32_t genesisHeight;
    /**
     * Minimum blocks including miner confirmation of the total of 2016 blocks
     * in a retargeting period, (nPowTargetTimespan / nPowTargetSpacing) which
     * is also used for BIP9 deployments.
     * Examples: 1916 for 95%, 1512 for testchains.
     */
    uint32_t nRuleChangeActivationThreshold;
    uint32_t nMinerConfirmationWindow;
    /** Proof of work parameters */
    uint256 powLimit;
    bool fPowAllowMinDifficultyBlocks;
    bool fPowNoRetargeting;
    int64_t nPowTargetSpacing;
    int64_t nPowTargetTimespan;
    int64_t DifficultyAdjustmentInterval() const {
        return nPowTargetTimespan / nPowTargetSpacing;
    }
    uint256 nMinimumChainWork;
    uint256 defaultAssumeValid;
};
} // namespace Consensus

#endif // MVC_CONSENSUS_PARAMS_H
