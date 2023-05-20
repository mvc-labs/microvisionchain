// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2022 The Bitcoin Core developers
// Copyright (c) 2021-2023 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MVC_INIT_H
#define MVC_INIT_H

#include <string>

#include "sync.h"
#include "taskcancellation.h"
#if ENABLE_ZMQ
#include "zmq/zmqnotificationinterface.h"
#endif

class Config;
class ConfigInit;
class CScheduler;
class CWallet;

#if ENABLE_ZMQ
/**
* cs_zmqNotificationInterface is used to protect pzmqNotificationInterface. One of the race conditions can occur
* at shutdown when pzmqNotificationInterface gets deleted while RPC thread might still be using it.
*/
extern CCriticalSection cs_zmqNotificationInterface;
extern CZMQNotificationInterface *pzmqNotificationInterface;
#endif

namespace boost {
class thread_group;
} // namespace boost

void StartShutdown();
task::CCancellationToken GetShutdownToken();
/** Interrupt threads */
void Interrupt(boost::thread_group &threadGroup);
void Shutdown();
//! Initialize the logging infrastructure
void InitLogging();
//! Parameter interaction: change current parameters depending on various rules
void InitParameterInteraction();

/** Initialize mvc core: Basic context setup.
 *  @note This can be done before daemonization.
 *  @pre Parameters should be parsed and config file should be read.
 */
bool AppInitBasicSetup();
/**
 * Initialization: parameter interaction.
 * @note This can be done before daemonization.
 * @pre Parameters should be parsed and config file should be read,
 * AppInitBasicSetup should have been called.
 */
bool AppInitParameterInteraction(ConfigInit &config);
/**
 * Initialization sanity checks: ecc init, sanity checks, dir lock.
 * @note This can be done before daemonization.
 * @pre Parameters should be parsed and config file should be read,
 * AppInitParameterInteraction should have been called.
 */
bool AppInitSanityChecks();
/**
 * MVC core main initialization.
 * @note This should only be done after daemonization.
 * @pre Parameters should be parsed and config file should be read,
 * AppInitSanityChecks should have been called.
 */
bool AppInitMain(ConfigInit &config, boost::thread_group &threadGroup,
                 CScheduler &scheduler, const task::CCancellationToken& shutdownToken);

/** The help message mode determines what help message to show */
enum HelpMessageMode { HMM_MVCD };

/** Help for options shared between UI and daemon (for -help) */
std::string HelpMessage(HelpMessageMode mode, const Config& config);
/** Returns licensing information (for -version) */
std::string LicenseInfo();

#endif // MVC_INIT_H
