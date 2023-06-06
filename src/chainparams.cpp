// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2022 The Bitcoin Core developers
// Copyright (c) 2021-2023 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "policy/policy.h"
#include "script/script_num.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <cassert>

#include "chainparamsseeds.h"

#define GENESIS_ACTIVATION_MAIN                 1
#define GENESIS_ACTIVATION_TESTNET              1

static CBlock CreateGenesisBlock(const char *pszTimestamp,
                                 const CScript &genesisOutputScript,
                                 uint32_t nTime, uint32_t nNonce,
                                 uint32_t nBits, int32_t nVersion,
                                 const Amount genesisReward,
                                 bool isTestNet) {
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig =
        CScript() << 486604799 << CScriptNum(4)
                  << std::vector<uint8_t>((const uint8_t *)pszTimestamp,
                                          (const uint8_t *)pszTimestamp +
                                              strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime = nTime;
    genesis.nBits = nBits;
    genesis.nNonce = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    if (isTestNet) {
        genesis.hashPrevBlock.SetNull();
        genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    } else {
        genesis.hashPrevBlock.SetHex("00000000000000000102d94fde9bd0807a2cc7582fe85dd6349b73ce4e8d9322");
        genesis.hashMerkleRoot.SetHex("da2b9eb7e8a3619734a17b55c47bdd6fd855b0afa9c7e14e3a164a279e51bba9");
    }

    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation transaction
 * cannot be spent since it did not originally exist in the database.
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce,
                                 uint32_t nBits, int32_t nVersion,
                                 const Amount genesisReward,
                                 bool isTestNet) {
    const char *pszTimestamp =
        "Nothing to say";
    const CScript genesisOutputScript =
        CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909"
                              "a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112"
                              "de5c384df7ba0b8d578a4c702b6bf11d5f")
                  << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce,
                              nBits, nVersion, genesisReward, isTestNet);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.BitcoinSoftForksHeight = 8000;
        consensus.BitcoinSoftForksHash = uint256S(
            "00000000000000001573688db762f2e03cfe9bc7c6da8496c3f14743166d303a");
        consensus.powLimit = uint256S(
            "00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // two weeks
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        // 95% of 2016
        consensus.nRuleChangeActivationThreshold = 1916;
        // nPowTargetTimespan / nPowTargetSpacing
        consensus.nMinerConfirmationWindow = 2016;

        // The half life for the ASERT DAA. For every (nASERTHalfLife) seconds behind schedule the blockchain gets,
        // difficulty is cut in half. Doubled if blocks are ahead of schedule.
        // Two days
        consensus.nASERTHalfLife = 2 * 24 * 60 * 60;
        // June 13, 2023 hard fork
        consensus.asertActivationTime = 1686636000;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S(
            "000000000000000000000000000000000000000000021e0be558cbec88f59787");

        // By default assume that the signatures in ancestors of this block are
        // valid.
        consensus.defaultAssumeValid = uint256S(
            "000000000000000000e45ad2fbcc5ff3e85f0868dd8f00ad4e92dffabe28f8d2");

        // August 1, 2017 hard fork
        consensus.uahfHeight = 1;

        // November 13, 2017 hard fork
        consensus.daaHeight = 0;

        // February 2020, Genesis Upgrade
        consensus.genesisHeight = GENESIS_ACTIVATION_MAIN;

        /**
         * The message start string is designed to be unlikely to occur in
         * normal data. The characters are rarely used upper ASCII, not valid as
         * UTF-8, and produce a large 32-bit integer with any alignment.
         */
        diskMagic[0] = 0x69;
        diskMagic[1] = 0xC3;
        diskMagic[2] = 0x5A;
        diskMagic[3] = 0xA5;
        netMagic[0] = 0x52;
        netMagic[1] = 0x9B;
        netMagic[2] = 0x68;
        netMagic[3] = 0x96;
        nDefaultPort = 9883;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(0x5BEDB819, 0x4D8FDFF4, 0x18021FDB, 0x20000000,
                                     50 * COIN, false);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock ==
               uint256S("000000000000000001d956714215d96ffc00e0afda4cd0a96c96f8d802b1662b"));
        assert(genesis.hashMerkleRoot ==
               uint256S("da2b9eb7e8a3619734a17b55c47bdd6fd855b0afa9c7e14e3a164a279e51bba9"));

        // Note that of those with the service bits flag, most only support a
        // subset of possible options.
        // MicroVisionChain seeder
        vSeeds.push_back(CDNSSeedData("", "", true));
       
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<uint8_t>(1, 0);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<uint8_t>(1, 5);
        base58Prefixes[SECRET_KEY] = std::vector<uint8_t>(1, 128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        vFixedSeeds = std::vector<SeedSpec6>(
            pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = true;

        checkpointData = { {
                {0       ,uint256S("000000000000000001d956714215d96ffc00e0afda4cd0a96c96f8d802b1662b")},
                {10      ,uint256S("000000005ed7fcdbc7b1e02b2cade8aa5a6800d5c8266f935b2c0f26e420fb1b")},
                {100     ,uint256S("0000000000000efeb84247cba43f34a88587113ef2382f1cc97a067ec91032e9")},
                {1000    ,uint256S("000000000000040c71f55cc180ae44c9fe54c21b5b69ace211275ed55ba52c33")},
                {2000    ,uint256S("0000000000001fa09f4ea818c304a3adfc8f220bb36bc7bff87ec275e99be416")},
                {3000    ,uint256S("000000000000000310e8ddab9098c2ee573e430d6d8ebbc89dd27731eb7ba460")},
                {4000    ,uint256S("00000000000000000bdad66ec2ac05fb41ffd0e8d79cbbdbad09a9a47adb9d98")},
                {5000    ,uint256S("00000000000000000a3ab470c8ef06817771968eb788625e71debfe893be5f83")},
                {6000    ,uint256S("000000000000000006990a28e7ffff8d26ce1496b3db2c0c0563fcec3df322ac")},
                {7000    ,uint256S("00000000000000000beba1611b710233fac175805ac059078b437c4fbe027b0f")},
                {8000    ,uint256S("00000000000000001573688db762f2e03cfe9bc7c6da8496c3f14743166d303a")},
                {9000    ,uint256S("00000000000000001a40b48f48bf7bef5c3fc7d4e6cb853a43769a4333e6b175")},
                {10000   ,uint256S("0000000000000000267cebf54e6744ea0ca3810dc9ac9a45f9bf2af9d1cc202e")},
                {15000   ,uint256S("00000000000000001e42eaa37721614e3ccabca5060f3bcbb195021057564924")},
                {20000   ,uint256S("00000000000000000d72037cdd71687274a92e868ca934349d5b3933c604d97e")},
            }};

        // Data as of block
        // 000000000000000001d2ce557406b017a928be25ee98906397d339c3f68eec5d
        // (height 523992).
        chainTxData = ChainTxData{
            // UNIX timestamp of last known number of transactions.
            1522608016,
            // Total number of transactions between genesis and that timestamp
            // (the tx=... number in the SetBestChain mvcd.log lines)
            248589038,
            // Estimated number of transactions per second after that timestamp.
            3.2};

        defaultBlockSizeParams = DefaultBlockSizeParams{
            // activation time 
            MAIN_NEW_BLOCKSIZE_ACTIVATION_TIME,
            // max block size
            MAIN_DEFAULT_MAX_BLOCK_SIZE,
            // max generated block size before activation
            MAIN_DEFAULT_MAX_GENERATED_BLOCK_SIZE_BEFORE,
            // max generated block size after activation
            MAIN_DEFAULT_MAX_GENERATED_BLOCK_SIZE_AFTER
        };

        fTestBlockCandidateValidity = false;
        fDisbleBIP30Checks = false;
        fCanDisbleBIP30Checks = false;
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 168000;
        consensus.BitcoinSoftForksHeight = 227931;
        consensus.BitcoinSoftForksHash = uint256S(
            "000000000000024b89b42a942fe0d9fea3bb44ab7bd1b19115dd6a759c0808b8");
        consensus.powLimit = uint256S(
            "00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // two weeks
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        // 75% for testchains
        consensus.nRuleChangeActivationThreshold = 1916;
        // nPowTargetTimespan / nPowTargetSpacing
        consensus.nMinerConfirmationWindow = 2016;

        // The half life for the ASERT DAA. For every (nASERTHalfLife) seconds behind schedule the blockchain gets,
        // difficulty is cut in half. Doubled if blocks are ahead of schedule.
        // Two days
        consensus.nASERTHalfLife = 2 * 24 * 60 * 60;
        consensus.asertActivationTime = 1685426400;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S(
            "000000000000000000000000000000000000000000a0f3064330647e2f6c4828");

        // By default assume that the signatures in ancestors of this block are
        // valid.
        consensus.defaultAssumeValid = uint256S(
            "000000000000000000e45ad2fbcc5ff3e85f0868dd8f00ad4e92dffabe28f8d2");

        // August 1, 2017 hard fork
        consensus.uahfHeight = 1;

        // November 13, 2017 hard fork
        consensus.daaHeight = 3186;

        // February 2020, Genesis Upgrade
        consensus.genesisHeight = GENESIS_ACTIVATION_TESTNET;

        diskMagic[0] = 0xA5;
        diskMagic[1] = 0x5A;
        diskMagic[2] = 0xC3;
        diskMagic[3] = 0x69;
        netMagic[0] = 0xD4;
        netMagic[1] = 0x48;
        netMagic[2] = 0x28;
        netMagic[3] = 0x12;
        nDefaultPort = 19883;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(0x5BEDB819, 781518760, 0x1d00ffff, 0x20000000,
                                     50 * COIN, true);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock ==
                 uint256S("000000005abc148d1e91534997189fcce59056c980a7164e8eca2e2ce29a1575"));
        assert(genesis.hashMerkleRoot ==
               uint256S("921c9ad4264610101e46b4e67b7c030fbcc4ca9633bbd40b3079a62cf3ef531d"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        // MicroVisionChain seeder
        vSeeds.push_back(CDNSSeedData("", "", true));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<uint8_t>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<uint8_t>(1, 196);
        base58Prefixes[SECRET_KEY] = std::vector<uint8_t>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        vFixedSeeds = std::vector<SeedSpec6>(
            pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        // vFixedSeeds.clear();
        vSeeds.clear();

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = { {
            {0, uint256S("000000005abc148d1e91534997189fcce59056c980a7164e8eca2e2ce29a1575")}
            }};

        // Data as of block
        // 000000000000000001d2ce557406b017a928be25ee98906397d339c3f68eec5d
        // (height 523992).
        chainTxData = ChainTxData{
            // UNIX timestamp of last known number of transactions.
            1522608016,
            // Total number of transactions between genesis and that timestamp
            // (the tx=... number in the SetBestChain mvcd.log lines)
            248589038,
            // Estimated number of transactions per second after that timestamp.
            3.2};
            
        defaultBlockSizeParams = DefaultBlockSizeParams{
            // activation time 
            TESTNET_NEW_BLOCKSIZE_ACTIVATION_TIME,
            // max block size
            TESTNET_DEFAULT_MAX_BLOCK_SIZE,
            // max generated block size before activation
            TESTNET_DEFAULT_MAX_GENERATED_BLOCK_SIZE_BEFORE,
            // max generated block size after activation
            TESTNET_DEFAULT_MAX_GENERATED_BLOCK_SIZE_AFTER
        };

        fTestBlockCandidateValidity = false;
        fDisbleBIP30Checks = false;
        fCanDisbleBIP30Checks = true;
    }
};


static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

void ResetNetMagic(CChainParams& chainParam, const std::string& hexcode)
{
    if(!HexToArray(hexcode, chainParam.netMagic))
        throw std::runtime_error(strprintf("%s: Bad hex code %s.", __func__, hexcode)); 
}


bool HexToArray(const std::string& hexstring, CMessageHeader::MessageMagic& array){
    if(!IsHexNumber(hexstring))
        return false;

    const std::vector<uint8_t> hexVect = ParseHex(hexstring);

    if(hexVect.size()!= array.size())
        return false;

    std::copy(hexVect.begin(),hexVect.end(),array.begin());

    return true;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string &chain) {
    if (chain == CBaseChainParams::MAIN) {
        return std::unique_ptr<CChainParams>(new CMainParams());
    }

    if (chain == CBaseChainParams::TESTNET) {
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    }

    throw std::runtime_error(
        strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string &network) {
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);

    // If not mainnet, allow to set the parameter magicbytes (for testing propose)
    const bool isMagicBytesSet = gArgs.IsArgSet("-magicbytes");
    if(network != CBaseChainParams::MAIN && isMagicBytesSet){
        const std::string magicbytesStr = gArgs.GetArg("-magicbytes", "0f0f0f0f");
        LogPrintf("Manually set magicbytes [%s].\n",magicbytesStr);
        ResetNetMagic(*globalChainParams,magicbytesStr);
    }
}
