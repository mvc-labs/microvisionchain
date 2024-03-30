// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2022 The Bitcoin Core developers
// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
/**
 * Money parsing/formatting utilities.
 */
#ifndef MVC_UTILMONEYSTR_H
#define MVC_UTILMONEYSTR_H

#include <cstdint>
#include <string>

#include "amount.h"

std::string FormatMoney(const Amount &n);
bool ParseMoney(const std::string &str, Amount &nRet);
bool ParseMoney(const char *pszIn, Amount &nRet);

#endif // MVC_UTILMONEYSTR_H
