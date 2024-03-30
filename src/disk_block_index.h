// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#pragma once

#include "block_index.h"
#include "logging.h"
#include "serialize.h"
#include "tinyformat.h"
#include "uint256.h"

#include <string>

/** Used to marshal pointers into hashes for db storage. */
class CDiskBlockIndex {
public:

    explicit CDiskBlockIndex(CBlockIndex &pindexIn)
        : hashPrev(pindexIn.pprev ? pindexIn.pprev->GetBlockHash() : uint256())
        , blockIndex(pindexIn)
    {}

    ADD_SERIALIZE_METHODS

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream &s, Operation ser_action) {
        int nVersion = s.GetVersion();
        if (!(s.GetType() & SER_GETHASH)) {
            READWRITE(VARINT(nVersion));
        }

        READWRITE(VARINT(blockIndex.nHeight));
        READWRITE(blockIndex.nStatus);
        READWRITE(VARINT(blockIndex.nTx));
        if (blockIndex.nStatus.hasData() || blockIndex.nStatus.hasUndo()) {
            READWRITE(VARINT(blockIndex.nFile));
        }
        if (blockIndex.nStatus.hasData()) {
            READWRITE(VARINT(blockIndex.nDataPos));
        }
        if (blockIndex.nStatus.hasUndo()) {
            READWRITE(VARINT(blockIndex.nUndoPos));
        }
        if(blockIndex.nStatus.getValidity() == BlockValidity::SCRIPTS)
        {
            blockIndex.mValidationCompletionTime =
                CBlockIndex::SteadyClockTimePoint::min();
        }

        // block header
        READWRITE(blockIndex.nVersion);
        READWRITE(hashPrev);
        READWRITE(blockIndex.hashMerkleRoot);
        READWRITE(blockIndex.nTime);
        READWRITE(blockIndex.nBits);
        READWRITE(blockIndex.nNonce);
        if (blockIndex.nStatus.hasDiskBlockMetaData())
        {
            try {
                READWRITE(blockIndex.mDiskBlockMetaData);
            } catch (const std::ios_base::failure &) {
                blockIndex.nStatus = blockIndex.nStatus.withDiskBlockMetaData(false);
                LogPrintf("Can not read metadata from block %s. Probably upgrading from downgraded version. \n", GetBlockHash().ToString());
            }
        }

        if(blockIndex.nStatus.hasDataForSoftConsensusFreeze())
        {
            try
            {
                READWRITE(VARINT(blockIndex.mSoftConsensusFreezeForNBlocks));
            }
            catch (std::ios_base::failure&)
            {
                blockIndex.nStatus = blockIndex.nStatus.withDataForSoftConsensusFreeze(false);
                blockIndex.mSoftConsensusFreezeForNBlocks = -1;
                LogPrintf("Can not read soft consensus freeze status for block %s from database. Probably upgrading from downgraded version.\n", GetBlockHash().ToString());
            }
        }
        else if(ser_action.ForRead())
        {
            blockIndex.mSoftConsensusFreezeForNBlocks = -1;
        }
    }

    uint256 GetBlockHash() const {
        CBlockHeader block;
        block.nVersion = blockIndex.nVersion;
        block.hashPrevBlock = hashPrev;
        block.hashMerkleRoot = blockIndex.hashMerkleRoot;
        block.nTime = blockIndex.nTime;
        block.nBits = blockIndex.nBits;
        block.nNonce = blockIndex.nNonce;
        return block.GetHash();
    }

    bool IsGenesis() const { return hashPrev.IsNull(); }
    const uint256& GetHashPrev() const
    {
        assert( !hashPrev.IsNull() );
        return hashPrev;
    }

    std::string ToString() const {
        std::string str = "CDiskBlockIndex(";
        str += blockIndex.ToString();
        str += strprintf("\n                hashBlock=%s, hashPrev=%s)",
                         GetBlockHash().ToString(), hashPrev.ToString());
        return str;
    }

private:
    uint256 hashPrev; // hashPrev is not set only for genesis block
    CBlockIndex& blockIndex;
};

