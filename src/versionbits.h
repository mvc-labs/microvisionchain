// Copyright (c) 2016-2022 The Bitcoin Core developers
// Copyright (c) 2021-2023 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MVC_CONSENSUS_VERSIONBITS
#define MVC_CONSENSUS_VERSIONBITS

#include <cstdint>

/** What block version to use for new blocks (pre versionbits) */
static const int32_t VERSIONBITS_LAST_OLD_BLOCK_VERSION = 4;
/** Version bits are not used anymore.
    This variable is used in assembler.cpp for consistency with old code and to set the version of block that we are going to mine. */
static const int32_t VERSIONBITS_TOP_BITS = 0x20000000UL;

#endif
