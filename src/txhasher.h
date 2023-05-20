// Copyright (c) 2021-2023 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MVC_TXHASHER_H
#define MVC_TXHASHER_H

#include "primitives/transaction.h"
#include "hash.h"

/**
 * Hasher objects transactions for std::unordered_set and similar hash-based containers.
 */
class StaticHasherSalt
{
protected:
    static const uint64_t k0, k1;
};

class SaltedTxidHasher : private StaticHasherSalt
{
public:
    size_t operator()(const uint256& txid) const
    {
        return SipHashUint256(k0, k1, txid);
    }
};

class SaltedOutpointHasher : private StaticHasherSalt
{
public:
    size_t operator()(const COutPoint& outpoint) const
    {
        return SipHashUint256Extra(k0, k1, outpoint.GetTxId(), outpoint.GetN());
    }
};

#endif // MVC_TXHASHER_H
