// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2022 The Bitcoin Core developers
// Copyright (c) 2021-2023 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparamsbase.h"

#include "tinyformat.h"
#include "util.h"

#include <cassert>

const std::string CBaseChainParams::MAIN = "main";
const std::string CBaseChainParams::TESTNET = "test";

void AppendParamsHelpMessages(std::string &strUsage, bool debugHelp) {
    strUsage += HelpMessageGroup(_("Chain selection options:"));
    strUsage += HelpMessageOpt("-testnet", _("Use the test chain"));
    strUsage += HelpMessageOpt(
        "-regtest", "Enter regression test mode, which uses a special "
                    "chain in which blocks can be solved instantly. "
                    "This is intended for regression testing tools and app "
                    "development.");
}

CBaseChainParams::CBaseChainParams(int port, const std::string& data_dir)
    : nRPCPort{port}, strDataDir{data_dir}
{
}

static std::unique_ptr<CBaseChainParams> globalChainBaseParams;

const CBaseChainParams &BaseParams() {
    assert(globalChainBaseParams);
    return *globalChainBaseParams;
}

std::unique_ptr<CBaseChainParams>
CreateBaseChainParams(const std::string &chain) {
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CBaseChainParams>(
            new CBaseChainParams(9882, ""));
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CBaseChainParams>(
            new CBaseChainParams(19882, "testnet3"));
    else
        throw std::runtime_error(
            strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectBaseParams(const std::string &chain) {
    globalChainBaseParams = CreateBaseChainParams(chain);
}

std::string ChainNameFromCommandLine() {
    if (gArgs.GetBoolArg("-testnet", false)) return CBaseChainParams::TESTNET;
    return CBaseChainParams::MAIN;
}
