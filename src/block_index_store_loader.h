// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <memory>
#include <mutex>

#include "block_index_store.h"

class CDBIterator;

class BlockIndexStoreLoader
{
public:
    BlockIndexStoreLoader(BlockIndexStore &blockIndexStoreIn) : mBlockIndexStore(blockIndexStoreIn) {};

    // may only be used in contexts where we are certain that nobody is using
    // CBlockIndex instances that are owned by this class
    bool ForceLoad( const Config& config, std::unique_ptr<CDBIterator> cursor );

    // may only be used in contexts where we are certain that nobody is using
    // CBlockIndex instances that are owned by this class
    void ForceClear()
    {
        std::lock_guard lock{ mBlockIndexStore.mMutex };

        mBlockIndexStore.mStore.clear();
        mBlockIndexStore.mBestHeader = nullptr;
        mBlockIndexStore.mDirtyBlockIndex.Clear();
    }
private:
    BlockIndexStore& mBlockIndexStore;
};
