// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MVC_CHECKPOINTS_H
#define MVC_CHECKPOINTS_H

#include "uint256.h"

#include <map>

class CBlockIndex;
struct CCheckpointData;

/**
 * Block-chain checkpoints are compiled-in sanity checks.
 * They are updated every release or three.
 */
namespace Checkpoints {

//! Returns true if block passes checkpoint checks
bool CheckBlock(const CCheckpointData &data, int32_t nHeight, const uint256 &hash);

//! Returns last CBlockIndex* in mapBlockIndex that is a checkpoint
CBlockIndex *GetLastCheckpoint(const CCheckpointData &data);

} // namespace Checkpoints

#endif // MVC_CHECKPOINTS_H
