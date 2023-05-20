// Copyright (c) 2009-2022 The Bitcoin Core developers
// Copyright (c) 2021-2023 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <enum_cast.h>

// Some pre-defined thread priority levels.
enum class ThreadPriority : int
{
    Low = 0,
    Normal = 1,
    High = 2
};

// Enable enum_cast for ThreadPriority, so we can log informatively
const enumTableT<ThreadPriority>& enumTable(ThreadPriority);
