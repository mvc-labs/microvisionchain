// Copyright (c) 2021-2023 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "disk_block_pos.h"
#include "serialize.h"

#include <numeric>

struct CDiskTxPos : public CDiskBlockPos {
private:
    uint64_t nTxOffset{ 0 }; // after header

public:
    uint64_t TxOffset() const { return nTxOffset; }

    ADD_SERIALIZE_METHODS

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream &s, Operation ser_action) {
        READWRITE(*(CDiskBlockPos *)this);

        // Legacy 32 bit sizes used for reading and writing.
        // When writing size larger or equal than max 32 bit value,
        // max 32 bit value (0xFFFFFFFF) is written in 32 bit field
        // and actual size is written in separate 64 bit field.
        // When reading, separate 64 bit value should be read when 32 bit value
        // is max (0xFFFFFFFF).
        unsigned int offset =
              nTxOffset >= std::numeric_limits<unsigned int>::max()
            ? std::numeric_limits<unsigned int>::max()
            : static_cast<unsigned int>(nTxOffset);
        READWRITE(VARINT(offset));

        if (offset == std::numeric_limits<unsigned int>::max())
        {
            READWRITE(VARINT(nTxOffset));
        }
        else
        {
            nTxOffset = offset;
        }
    }

    CDiskTxPos(const CDiskBlockPos &blockIn, uint64_t nTxOffsetIn)
        : CDiskBlockPos{ blockIn }
        , nTxOffset{ nTxOffsetIn }
    {}

    CDiskTxPos() = default;
};
