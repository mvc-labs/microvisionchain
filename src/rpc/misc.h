// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2022 The Bitcoin Core developers
// Copyright (c) 2021-2023 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MVC_RPCMISC_H
#define MVC_RPCMISC_H

#include <string>
#include <optional>

class CScript;
class CWallet;
class UniValue;

/**
 * Returns a std::optional<uint32> that is represented by flagName.
 * If flagName is unknown std::nullopt is returned and err string is set.
 */
std::optional<uint32_t> GetFlagNumber(const std::string& flagName, std::string& err);

CScript createmultisig_redeemScript(CWallet *const pwallet,
                                    const UniValue &params);

#endif // MVC_RPCBLOCKCHAIN_H
