// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2021 The Bitcoin Core developers
// Copyright (c) 2021-2023 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "config.h"
#include "consensus/validation.h"
#include "primitives/transaction.h"
#include "validation.h"

bool CheckFinalTx(
    const CTransaction &tx,
    int32_t nChainActiveHeight,
    int nMedianTimePast,
    int flags = -1) {

    auto &config = GlobalConfig::GetConfig();
    CValidationState state;
    return ContextualCheckTransactionForCurrentBlock(
                config,
                tx,
                nChainActiveHeight,
                nMedianTimePast,
                state,
                flags);
}
