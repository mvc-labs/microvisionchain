// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2022 The Bitcoin Core developers
// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MVC_SCRIPT_SCRIPTCACHE_H
#define MVC_SCRIPT_SCRIPTCACHE_H

#include "uint256.h"

#include <cstdint>

class CTransaction;

// DoS prevention: limit cache size to 64MB (over 2000000 entries on 64-bit
// systems). Due to how we count cache size, actual memory usage is slightly
// more (~64.5 MB)
static const unsigned int DEFAULT_MAX_SCRIPT_CACHE_SIZE = 64;
// Maximum sig cache size allowed
static const int64_t MAX_MAX_SCRIPT_CACHE_SIZE = 16384;

/** Initializes the script-execution cache */
void InitScriptExecutionCache();

/** Clear cache content */
void ClearCache();

/** Compute the cache key for a given transaction and flags. */
uint256 GetScriptCacheKey(const CTransaction &tx, uint32_t flags);

/** Check if a given key is in the cache. */
bool IsKeyInScriptCache(uint256 key, bool erase);

/** Add an entry in the cache. */
void AddKeyInScriptCache(uint256 key);

#endif // MVC_SCRIPT_SCRIPTCACHE_H
