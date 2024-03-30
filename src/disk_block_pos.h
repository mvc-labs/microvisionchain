// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "serialize.h"
#include "tinyformat.h"

#include <string>

struct CDiskBlockPos {
private:
    int nFile{ -1 };
    unsigned int nPos{ 0 };

public:
    int File() const { return nFile; }
    unsigned int Pos() const { return nPos; }

    ADD_SERIALIZE_METHODS

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream &s, Operation ser_action) {
        READWRITE(VARINT(nFile));
        READWRITE(VARINT(nPos));
    }

    CDiskBlockPos() = default;

    CDiskBlockPos(int nFileIn, unsigned int nPosIn) {
        nFile = nFileIn;
        nPos = nPosIn;
    }

    bool operator==(const CDiskBlockPos& other) const
    {
        return (nFile == other.nFile && nPos == other.nPos);
    }

    bool operator!=(const CDiskBlockPos& other) const
    {
        return !(*this == other);
    }

    bool IsNull() const { return (nFile == -1); }

    std::string ToString() const {
        return strprintf("CBlockDiskPos(nFile=%i, nPos=%i)", nFile, nPos);
    }
};
