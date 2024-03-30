// Copyright (c) 2009-2021 The Bitcoin Core developers
// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MVC_PROCESSINGBLOCKINDEX_H
#define MVC_PROCESSINGBLOCKINDEX_H

#include "primitives/block.h"
#include "undo.h"

class ProcessingBlockIndex
{
public:
    template<typename T> struct UnitTestAccess;

    ProcessingBlockIndex( CBlockIndex& index ) : mIndex(index) {}

    DisconnectResult DisconnectBlock(
        const CBlock& block,
        CCoinsViewCache& view,
        const task::CCancellationToken& shutdownToken) const;

private:

    DisconnectResult ApplyBlockUndo(
        const CBlockUndo& blockUndo,
        const CBlock& block,
        CCoinsViewCache& view,
        const task::CCancellationToken& shutdownToken) const;

    CBlockIndex& mIndex;
};

#endif
