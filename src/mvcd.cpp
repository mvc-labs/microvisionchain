// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2021 The Bitcoin Core developers
// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/mvc-config.h"
#endif

#include "chainparams.h"
#include "clientversion.h"
#include "compat.h"
#include "config.h"
#include "fs.h"
#include "init.h"
#include "noui.h"
#include "scheduler.h"
#include "util.h"
#include "utilstrencodings.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/thread.hpp>

#include <cstdio>


/* Introduction text for doxygen: */

/*! \mainpage Developer documentation
 *
 * \section intro_sec Introduction
 *
 * This is the developer documentation of MicroVisionChain
 * (https://www.microvisionchain.com/). MVC is a client for the digital
 * currency called MicroVisionChain, which enables
 * instant payments to anyone, anywhere in the world. MicroVisionChain uses
 * peer-to-peer technology to operate with no central authority: managing
 * transactions and issuing money are carried out collectively by the network.
 *
 * \section Navigation
 * Use the buttons <code>Namespaces</code>, <code>Classes</code> or
 * <code>Files</code> at the top of the page to start navigating the code.
 */

void WaitForShutdown(boost::thread_group *threadGroup, const task::CCancellationToken& shutdownToken) {

    // Tell the main threads to shutdown.
    while (!shutdownToken.IsCanceled()) {
        MilliSleep(200);
    }
    if (threadGroup) {
        Interrupt(*threadGroup);
        threadGroup->join_all();
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// Start
//
bool AppInit(int argc, char *argv[]) {
    RenameThread("main");
    boost::thread_group threadGroup;
    CScheduler scheduler;

    // FIXME: Ideally, we'd like to build the config here, but that's currently
    // not possible as the whole application has too many global state. However,
    // this is a first step.
    auto& config = GlobalConfig::GetModifiableGlobalConfig();

    bool fRet = false;

    //
    // Parameters
    //
    // main()
    gArgs.ParseParameters(argc, argv);

    // Process help and version before taking care about datadir
    if (gArgs.IsArgSet("-?") || gArgs.IsArgSet("-h") ||
        gArgs.IsArgSet("-help") || gArgs.IsArgSet("-version")) {
        std::string strUsage = strprintf(_("%s"), _(PACKAGE_NAME)) +
                               " " + _("version") + " " + FormatFullVersion() +
                               "\n";

        if (gArgs.IsArgSet("-version")) {
            strUsage += FormatParagraph(LicenseInfo());
        } else {
            strUsage += "\n" + _("Usage:") + "\n" +
                        "  mvcd [options]                     " +
                        strprintf(_("Start %s Daemon"), _(PACKAGE_NAME)) + "\n";

            strUsage += "\n" + HelpMessage(HMM_MVCD, config);
        }

        fprintf(stdout, "%s", strUsage.c_str());
        return true;
    }

    try {
        if (!fs::is_directory(GetDataDir(false))) {
            fprintf(stderr,
                    "Error: Specified data directory \"%s\" does not exist.\n",
                    gArgs.GetArg("-datadir", "").c_str());
            return false;
        }
        try {
            gArgs.ReadConfigFile(gArgs.GetArg("-conf", MVC_CONF_FILENAME));
        } catch (const std::exception &e) {
            fprintf(stderr, "Error reading configuration file: %s\n", e.what());
            return false;
        }
        // Check for -testnet or -regtest parameter (Params() calls are only
        // valid after this clause)
        try {
            SelectParams(ChainNameFromCommandLine());
        } catch (const std::exception &e) {
            fprintf(stderr, "Error: %s\n", e.what());
            return false;
        }

        // Fill config with block size data
        config.SetDefaultBlockSizeParams(Params().GetDefaultBlockSizeParams());

        // Command-line RPC
        bool fCommandLine = false;
        for (int i = 1; i < argc; i++)
            if (!IsSwitchChar(argv[i][0]) &&
                !boost::algorithm::istarts_with(argv[i], "mvc:"))
                fCommandLine = true;

        if (fCommandLine) {
            fprintf(stderr, "Error: There is no RPC client functionality in "
                            "mvcd anymore. Use the mvc-cli utility "
                            "instead.\n");
            exit(EXIT_FAILURE);
        }
        // -server defaults to true for mvcd
        gArgs.SoftSetBoolArg("-server", true);
        // Set this early so that parameter interactions go to console
        InitLogging();
        InitParameterInteraction();
        if (!AppInitBasicSetup()) {
            // InitError will have been called with detailed error, which ends
            // up on console
            exit(1);
        }
        if (!AppInitParameterInteraction(config)) {
            // InitError will have been called with detailed error, which ends
            // up on console
            exit(1);
        }
        if (!AppInitSanityChecks()) {
            // InitError will have been called with detailed error, which ends
            // up on console
            exit(1);
        }
        if (gArgs.GetBoolArg("-daemon", false)) {
#if HAVE_DECL_DAEMON

            fprintf(stdout, "\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\n"
                            "\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\033[1;40;32m╭━╮╭━╮╭╮  ╭╮╭━━━╮\033[0m\033[1;40;34m▒▒▒▒\033[0m\033[1;40;31mMicroVision!\033[0m\033[1;40;34m▒▒▒▒\033[0m\n"
                            "\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\033[1;40;32m┃┃╰╯┃┃┃╰╮╭╯┃┃╭━╮┃\033[0m\033[1;40;34m▒▒▒▒  \033[0m\033[1;40;31mv0.2.0.0\033[0m\033[1;40;34m  ▒▒▒▒\033[0m\n"
                            "\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\033[1;40;32m┃╭╮╭╮┃╰╮┃┃╭╯┃┃ ╰╯\033[0m\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\n"
                            "\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\033[1;40;32m┃┃┃┃┃┃ ┃╰╯┃ ┃┃ ╭╮\033[0m\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\n"
                            "\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\033[1;40;32m┃┃┃┃┃┃ ╰╮╭╯ ┃╰━╯┃\033[0m\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\n"
                            "\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\033[1;40;32m╰╯╰╯╰╯  ╰╯  ╰━━━╯\033[0m\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\n"
                            "\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\033[1;40;32m╭━━━╮╭━━━╮╭━━━╮╭╮  ╭╮╭━━━╮╭━━━╮\033[0m\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\n"
                            "\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\033[1;40;32m┃╭━╮┃┃╭━━╯┃╭━╮┃┃╰╮╭╯┃┃╭━━╯┃╭━╮┃\033[0m\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\n"
                            "\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\033[1;40;32m┃╰━━╮┃╰━━╮┃╰━╯┃╰╮┃┃╭╯┃╰━━╮┃╰━╯┃\033[0m\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\n"
                            "\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\033[1;40;32m╰━━╮┃┃╭━━╯┃╭╮╭╯ ┃╰╯┃ ┃╭━━╯┃╭╮╭╯\033[0m\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\n"
                            "\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\033[1;40;32m┃╰━╯┃┃╰━━╮┃┃┃╰╮ ╰╮╭╯ ┃╰━━╮┃┃┃╰╮\033[0m\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\n"
                            "\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\033[1;40;32m╰━━━╯╰━━━╯╰╯╰━╯  ╰╯  ╰━━━╯╰╯╰━╯\033[0m\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\n"
                            "\033[1;40;34m▒▒▒▒▒▒▒\033[0m\033[1;40;32m╭━━━╮╭━━━━╮╭━━━╮╭━━━╮╭━━━━╮╭━━╮╭━╮ ╭╮╭━━━╮\033[0m\033[1;40;34m▒▒▒▒▒▒▒▒\033[0m\n"
                            "\033[1;40;34m▒▒▒▒▒▒▒\033[0m\033[1;40;32m┃╭━╮┃┃╭╮╭╮┃┃╭━╮┃┃╭━╮┃┃╭╮╭╮┃╰┫┣╯┃┃╰╮┃┃┃╭━╮┃\033[0m\033[1;40;34m▒▒▒▒▒▒▒▒\033[0m\n"
                            "\033[1;40;34m▒▒▒▒▒▒▒\033[0m\033[1;40;32m┃╰━━╮╰╯┃┃╰╯┃┃ ┃┃┃╰━╯┃╰╯┃┃╰╯ ┃┃ ┃╭╮╰╯┃┃┃ ╰╯\033[0m\033[1;40;34m▒▒▒▒▒▒▒▒\033[0m\n"
                            "\033[1;40;34m▒▒▒▒▒▒▒\033[0m\033[1;40;32m╰━━╮┃  ┃┃  ┃╰━╯┃┃╭╮╭╯  ┃┃   ┃┃ ┃┃╰╮┃┃┃┃╭━╮\033[0m\033[1;40;34m▒▒▒▒▒▒▒▒\033[0m\n"
                            "\033[1;40;34m▒▒▒▒▒▒▒\033[0m\033[1;40;32m┃╰━╯┃  ┃┃  ┃╭━╮┃┃┃┃╰╮  ┃┃  ╭┫┣╮┃┃ ┃┃┃┃╰┻━┃\033[0m\033[1;40;34m▒▒▒▒▒▒▒▒\033[0m\n"
                            "\033[1;40;34m▒▒▒▒▒▒▒\033[0m\033[1;40;32m╰━━━╯  ╰╯  ╰╯ ╰╯╰╯╰━╯  ╰╯  ╰━━╯╰╯ ╰━╯╰━━━╯\033[0m\033[1;40;34m▒▒▒▒▒▒▒▒\033[0m\n"
                            "\033[1;40;34m▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m\n");

            // Daemonize
            if (daemon(1, 0)) {
                // don't chdir (1), do close FDs (0)
                fprintf(stderr, "Error: daemon() failed: %s\n",
                        strerror(errno));
                return false;
            }
#else
            fprintf(
                stderr,
                "Error: -daemon is not supported on this operating system\n");
            return false;
#endif // HAVE_DECL_DAEMON
        }

        fRet = AppInitMain(config, threadGroup, scheduler, GetShutdownToken());
    } catch (const std::exception &e) {
        PrintExceptionContinue(&e, "AppInit()");
    } catch (...) {
        PrintExceptionContinue(nullptr, "AppInit()");
    }

    if (!fRet) {
        Interrupt(threadGroup);
        // threadGroup.join_all(); was left out intentionally here, because we
        // didn't re-test all of the startup-failure cases to make sure they
        // don't result in a hang due to some
        // thread-blocking-waiting-for-another-thread-during-startup case.
    } else {
        LogPrintf("Preload wait for shutdown\n");
        WaitForShutdown(&threadGroup, GetShutdownToken());
        LogPrintf("Preload wait for shutdown done\n");
    }
    LogPrintf("Checking Thread shutdown\n");
    Shutdown();

    return fRet;
}

int main(int argc, char *argv[]) {
    SetupEnvironment();

    // Connect mvcd signal handlers
    noui_connect();

    return (AppInit(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE);
}
