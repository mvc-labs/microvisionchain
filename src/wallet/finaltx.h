// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2021 The Bitcoin Core developers
// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MVC_DEPRECATED_FINALTX_H
#define MVC_DEPRECATED_FINALTX_H

class CTransaction;

/**
 * CheckFinalTx is deprecated. In order to provide a sane migration path away
 * from it, it is still provided in this header file. Or maybe we'll just
 * blackhole the wallet at some point.
 */
bool CheckFinalTx(
    const CTransaction &tx,
    int32_t nChainActiveHeight,
    int nMedianTimePast,
    int flags = -1);

#endif // MVC_DEPRECATED_FINALTX_H
