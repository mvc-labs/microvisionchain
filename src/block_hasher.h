// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "uint256.h"

struct BlockHasher {
    size_t operator()(const uint256 &hash) const { return hash.GetCheapHash(); }
};
