// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2022 The Bitcoin Core developers
// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "policy/fees.h"
#include "amount.h"

FeeFilterRounder::FeeFilterRounder(const CFeeRate &minIncrementalFee) {
    Amount minFeeLimit =
        std::max(Amount(1), minIncrementalFee.GetFeePerK() / 2);
    feeset.insert(Amount(0));
    for (double bucketBoundary = minFeeLimit.GetSatoshis();
         bucketBoundary <= double(MAX_FEERATE.GetSatoshis());
         bucketBoundary *= FEE_SPACING) {
        feeset.insert(Amount(int64_t(bucketBoundary)));
    }
}

Amount FeeFilterRounder::round(const Amount currentMinFee) {
    auto it = feeset.lower_bound(currentMinFee);
    if ((it != feeset.begin() && insecure_rand.rand32() % 3 != 0) ||
        it == feeset.end()) {
        it--;
    }

    return *it;
}
