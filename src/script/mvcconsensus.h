// Copyright (c) 2009-2022 The Bitcoin Core developers
// Copyright (c) 2021-2023 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MVC_MVCCONSENSUS_H
#define MVC_MVCCONSENSUS_H

#include <cstdint>

#if defined(BUILD_MVC_INTERNAL) && defined(HAVE_CONFIG_H)
#include "config/mvc-config.h"
#if defined(_WIN32)
#if defined(DLL_EXPORT)
#if defined(HAVE_FUNC_ATTRIBUTE_DLLEXPORT)
#define EXPORT_SYMBOL __declspec(dllexport)
#else
#define EXPORT_SYMBOL
#endif
#endif
#elif defined(HAVE_FUNC_ATTRIBUTE_VISIBILITY)
#define EXPORT_SYMBOL __attribute__((visibility("default")))
#endif
#elif defined(MSC_VER) && !defined(STATIC_LIBMVCCONSENSUS)
#define EXPORT_SYMBOL __declspec(dllimport)
#endif

#ifndef EXPORT_SYMBOL
#define EXPORT_SYMBOL
#endif

#ifdef __cplusplus
extern "C" {
#endif

class CScriptConfig;

#define MVCCONSENSUS_API_VER 1

typedef enum mvcconsensus_error_t {
    mvcconsensus_ERR_OK = 0,
    mvcconsensus_ERR_TX_INDEX,
    mvcconsensus_ERR_TX_SIZE_MISMATCH,
    mvcconsensus_ERR_TX_DESERIALIZE,
    mvcconsensus_ERR_AMOUNT_REQUIRED,
    mvcconsensus_ERR_INVALID_FLAGS,
} mvcconsensus_error;

/** Script verification flags */
enum {
    mvcconsensus_SCRIPT_FLAGS_VERIFY_NONE = 0,
    // evaluate P2SH (BIP16) subscripts
    mvcconsensus_SCRIPT_FLAGS_VERIFY_P2SH = (1U << 0),
    // enforce strict DER (BIP66) compliance
    mvcconsensus_SCRIPT_FLAGS_VERIFY_DERSIG = (1U << 2),
    // enforce NULLDUMMY (BIP147)
    mvcconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY = (1U << 4),
    // enable CHECKLOCKTIMEVERIFY (BIP65)
    mvcconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY = (1U << 9),
    // enable CHECKSEQUENCEVERIFY (BIP112)
    mvcconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY = (1U << 10),
    // enable WITNESS (BIP141)
    mvcconsensus_SCRIPT_FLAGS_VERIFY_WITNESS_DEPRECATED = (1U << 11),
    // enable SIGHASH_FORKID replay protection
    mvcconsensus_SCRIPT_ENABLE_SIGHASH_FORKID = (1U << 16),
    mvcconsensus_SCRIPT_FLAGS_VERIFY_ALL =
        mvcconsensus_SCRIPT_FLAGS_VERIFY_P2SH |
        mvcconsensus_SCRIPT_FLAGS_VERIFY_DERSIG |
        mvcconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY |
        mvcconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY |
        mvcconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY,
};

/// Returns 1 if the input nIn of the serialized transaction pointed to by txTo
/// correctly spends the scriptPubKey pointed to by scriptPubKey under the
/// additional constraints specified by flags.
/// If not nullptr, err will contain an error/success code for the operation
EXPORT_SYMBOL int mvcconsensus_verify_script(
    const CScriptConfig& config,
    const uint8_t *scriptPubKey, unsigned int scriptPubKeyLen,
    const uint8_t *txTo, unsigned int txToLen, unsigned int nIn,
    unsigned int flags, mvcconsensus_error *err);

EXPORT_SYMBOL int mvcconsensus_verify_script_with_amount(
    const CScriptConfig& config,
    const uint8_t *scriptPubKey, unsigned int scriptPubKeyLen, int64_t amount,
    const uint8_t *txTo, unsigned int txToLen, unsigned int nIn,
    unsigned int flags, mvcconsensus_error *err);

EXPORT_SYMBOL unsigned int mvcconsensus_version();

#ifdef __cplusplus
} // extern "C"
#endif

#undef EXPORT_SYMBOL

#endif // MVC_MVCCONSENSUS_H
