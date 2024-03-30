// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <cstdint>

/**
 * Default configuration values used in dsdetected message processing.
 */
struct DSDetectedDefaults
{
    // Default maximum transaction size to report over a dsdetected webhook (in
    // MB)
    static constexpr uint64_t DEFAULT_MAX_WEBHOOK_TXN_SIZE{100};
};
