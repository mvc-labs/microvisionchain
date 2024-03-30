// Copyright (c) 2009-2022 The Bitcoin Core developers
// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "threadpriority.h"

// Enable enum_cast for ThreadPriority, so we can log informatively
const enumTableT<ThreadPriority>& enumTable(ThreadPriority)
{
    static enumTableT<ThreadPriority> table
    {
        { ThreadPriority::Low,      "L" },
        { ThreadPriority::Normal,   "N" },
        { ThreadPriority::High,     "H" }
    };
    return table;
}

