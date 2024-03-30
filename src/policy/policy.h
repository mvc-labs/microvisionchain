// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2022 The Bitcoin Core developers
// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MVC_POLICY_POLICY_H
#define MVC_POLICY_POLICY_H

#include "consensus/consensus.h"
#include "script/interpreter.h"
#include "script/standard.h"

#include <optional>
#include <string>

class Config;
class CCoinsViewCache;

namespace task{class CCancellationToken;}

/** Default max block size parameters
 */
static const uint64_t MAIN_DEFAULT_MAX_BLOCK_SIZE = INT64_MAX; 
static const uint64_t TESTNET_DEFAULT_MAX_BLOCK_SIZE = INT64_MAX;

/** Default before and after max generated block size parameters and their activation times.
 */
static const uint64_t MAIN_NEW_BLOCKSIZE_ACTIVATION_TIME = 1713225600; 
static const uint64_t MAIN_DEFAULT_MAX_GENERATED_BLOCK_SIZE_BEFORE = 128 * ONE_MEGABYTE;
static const uint64_t MAIN_DEFAULT_MAX_GENERATED_BLOCK_SIZE_AFTER = 4 * ONE_GIGABYTE;

static const uint64_t TESTNET_NEW_BLOCKSIZE_ACTIVATION_TIME = 1713225600; 
static const uint64_t TESTNET_DEFAULT_MAX_GENERATED_BLOCK_SIZE_BEFORE = 128 * ONE_MEGABYTE;
static const uint64_t TESTNET_DEFAULT_MAX_GENERATED_BLOCK_SIZE_AFTER = 4 * ONE_GIGABYTE;


/** Default for -blockmintxfee, which sets the minimum feerate for a transaction
 * in blocks created by mining code **/
static const Amount DEFAULT_BLOCK_MIN_TX_FEE(500);
/** The maximum size for transactions we're willing to relay/mine - before genesis*/
static const uint64_t MAX_TX_SIZE_POLICY_BEFORE_GENESIS = 100000 - 1; // -1 because pre genesis policy validation was >=
/** The default size for transactions we're willing to relay/mine */
static const uint64_t DEFAULT_MAX_TX_SIZE_POLICY_AFTER_GENESIS = 10 * ONE_MEGABYTE;
/** The default minimum input (previous output) ScriptPubKey size to output ScriptPubKey size ratio to qualify for consolidation transaction */
static const uint64_t DEFAULT_MIN_CONSOLIDATION_FACTOR = 20;
/** The default maximum size for input scriptSig in a consolidation transaction */
static const uint64_t DEFAULT_MAX_CONSOLIDATION_INPUT_SCRIPT_SIZE = 150;
/** The default minimum number of confirmations to be eligible as an input in consolidation transaction */
static const uint64_t DEFAULT_MIN_CONF_CONSOLIDATION_INPUT = 6;
/** consolidation transaction with non standard inputs */
static const bool DEFAULT_ACCEPT_NON_STD_CONSOLIDATION_INPUT = false;

/** Maximum number of signature check operations in an IsStandard() P2SH script
 */
static const unsigned int MAX_P2SH_SIGOPS = 15;
/** The maximum number of sigops we're willing to relay/mine in a single tx before Genesis */
static const unsigned int MAX_TX_SIGOPS_COUNT_POLICY_BEFORE_GENESIS = MAX_TX_SIGOPS_COUNT_BEFORE_GENESIS / 5;
/** The maximum number of sigops we're willing to relay/mine in a single tx after Genesis */
static const unsigned int MAX_TX_SIGOPS_COUNT_POLICY_AFTER_GENESIS = UINT32_MAX;
/** Default policy value for -maxtxsigopscountspolicy, maximum number of sigops we're willing to relay/mine in a single tx after Genesis */
static const unsigned int DEFAULT_TX_SIGOPS_COUNT_POLICY_AFTER_GENESIS = MAX_TX_SIGOPS_COUNT_POLICY_AFTER_GENESIS;
/** Default for -maxmempool, maximum megabytes of mempool memory usage */
static const unsigned int DEFAULT_MAX_MEMPOOL_SIZE = 1000;
/** Default for -maxnonfinalmempool, maximum megabytes of non-final mempool memory usage */
static const unsigned int DEFAULT_MAX_NONFINAL_MEMPOOL_SIZE = 50;
/** Minimum feerate increase for mempool limiting **/
static const CFeeRate MEMPOOL_FULL_FEE_INCREMENT(Amount(1000));
/** Default for -maxscriptsizepolicy **/
static const unsigned int DEFAULT_MAX_SCRIPT_SIZE_POLICY_AFTER_GENESIS = 500 * ONE_KILOBYTE;
/** Default -maxmempoolsizedisk factor, maximum megabytes of total mempool disk usage as scaled -maxmempool */
static const unsigned int DEFAULT_MAX_MEMPOOL_SIZE_DISK_FACTOR = 0;
/** Default percentage of total mempool size (ram+disk) to use as max limit for CPFP transactions */
static const unsigned int DEFAULT_MEMPOOL_MAX_PERCENT_CPFP = 10;

/**
 * Min feerate for defining dust. Historically this has been the same as the
 * minRelayTxFee, however changing the dust limit changes which transactions are
 * standard and should be done with care and ideally rarely. It makes sense to
 * only increase the dust limit after prior releases were already not creating
 * outputs below the new threshold.
 * We will statically assert this to be the same value as DEFAULT_MIN_RELAY_TX_FEE
 * because of CORE-647
 */
static constexpr Amount DUST_RELAY_TX_FEE(250);

/**
 * The threshold below which a transaction is considered dust.
 * The dust limit factor expresses this value as a multiple (in percent) of the dust relay fee
 * applied to a transaction output.
 */
static constexpr int64_t DEFAULT_DUST_LIMIT_FACTOR{300};

/*
* Number of blocks around GENESIS activation (72 blocks before and 72 blocks after) when
* nodes will not be banned if they send a script that is not valid. That means if a node
* sends a GENESIS only valid transaction before GENESIS is activated it will not be baned.
* Same applies for a node that sent a PRE-GENESIS only valid transaction after GENESIS
* is activated
*/
static const int DEFAULT_GENESIS_GRACEFULL_ACTIVATION_PERIOD = 72;

/*
* Maximum number of blocks for Genesis graceful period on either side of the Genesis 
* activation block (span of ~100 days)
*/
static const int MAX_GENESIS_GRACEFULL_ACTIVATION_PERIOD = 7200;

// Default policy value for maximum number of non-push operations per script
static const uint64_t DEFAULT_OPS_PER_SCRIPT_POLICY_AFTER_GENESIS = UINT32_MAX;

// Default policy value for maximum number of public keys per multisig after GENESIS
static const uint64_t DEFAULT_PUBKEYS_PER_MULTISIG_POLICY_AFTER_GENESIS = UINT32_MAX;

/** Maximum stack memory usage (used instead of MAX_SCRIPT_ELEMENT_SIZE and MAX_STACK_ELEMENTS) after Genesis. **/
static const uint64_t DEFAULT_STACK_MEMORY_USAGE_POLICY_AFTER_GENESIS = 100 * ONE_MEGABYTE;

// Default policy value for script number length after Genesis
static const uint64_t DEFAULT_SCRIPT_NUM_LENGTH_POLICY_AFTER_GENESIS = 10 * ONE_KILOBYTE;

// Default policy value for coins cache size threshold before coins are no longer
// loaded into cache but instead returned directly to the caller.
static const uint64_t MIN_COINS_PROVIDER_CACHE_SIZE = ONE_MEGABYTE;
static const uint64_t DEFAULT_COINS_PROVIDER_CACHE_SIZE = ONE_GIGABYTE;

static const std::set<std::string> DEFAULT_CLIENTUA_BAN_PATTERNS {"abc","cash","bch"};

/**
 * Standard script verification flags that standard transactions will comply
 * with. However scripts violating these flags may still be present in valid
 * blocks and we must accept those blocks.
 */
static const unsigned int STANDARD_SCRIPT_VERIFY_FLAGS =
    MANDATORY_SCRIPT_VERIFY_FLAGS | SCRIPT_VERIFY_DERSIG |
    SCRIPT_VERIFY_MINIMALDATA | SCRIPT_VERIFY_NULLDUMMY |
    SCRIPT_VERIFY_DISCOURAGE_UPGRADABLE_NOPS | SCRIPT_VERIFY_CLEANSTACK |
    SCRIPT_VERIFY_CHECKLOCKTIMEVERIFY | SCRIPT_VERIFY_CHECKSEQUENCEVERIFY;

/** For convenience, standard but not mandatory verify flags. */
static const unsigned int STANDARD_NOT_MANDATORY_VERIFY_FLAGS =
    STANDARD_SCRIPT_VERIFY_FLAGS & ~MANDATORY_SCRIPT_VERIFY_FLAGS;

/** returns flags for "standard" script*/
inline unsigned int StandardScriptVerifyFlags(bool genesisEnabled,
                                       bool utxoAfterGenesis) {
    unsigned int scriptFlags = STANDARD_SCRIPT_VERIFY_FLAGS;
    if (utxoAfterGenesis) {
        scriptFlags |= SCRIPT_UTXO_AFTER_GENESIS;
    }
    if (genesisEnabled) {
        scriptFlags |= SCRIPT_GENESIS;
        scriptFlags |= SCRIPT_VERIFY_SIGPUSHONLY;
    }
    return scriptFlags;
}

/** Get the flags to use for non-final transaction checks */
inline unsigned int StandardNonFinalVerifyFlags(bool genesisEnabled)
{
    unsigned int flags { LOCKTIME_MEDIAN_TIME_PAST };
    if(!genesisEnabled) {
        flags |= LOCKTIME_VERIFY_SEQUENCE;
    }
    return flags;
}

/** Consolidation transactions are free */
bool IsDustReturnTxn (const CTransaction &tx);
bool IsConsolidationTxn(const Config &config, const CTransaction &tx, const CCoinsViewCache &inputs, int32_t tipHeight);

bool IsStandard(const Config &config, const CScript &scriptPubKey, int32_t nScriptPubKeyHeight, txnouttype &whichType);

/**
 * Check for standard transaction types
 * @param[in] nHeight represents the height that transactions was mined or the height that
 * we expect transcation will be mined in (in case transcation is being added to mempool)
 * @return True if all outputs (scriptPubKeys) use only standard transaction
 * forms
 */
bool IsStandardTx(const Config &config, const CTransaction &tx, int32_t nHeight, std::string &reason);

/**
 * Check for standard transaction types
 * @param[in] mapInputs    Map of previous transactions that have outputs we're
 * spending
 * @return True if all inputs (scriptSigs) use only standard transaction forms
 */
std::optional<bool> AreInputsStandard(
    const task::CCancellationToken& token,
    const Config& config,
    const CTransaction& tx,
    const CCoinsViewCache &mapInputs,
    const int32_t mempoolHeight);

#endif // MVC_POLICY_POLICY_H
