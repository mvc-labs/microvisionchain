// Copyright (c) 2015-2020 The Bitcoin Core developers
// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MVC_ZMQ_ZMQCONFIG_H
#define MVC_ZMQ_ZMQCONFIG_H

#if defined(HAVE_CONFIG_H)
#include "config/mvc-config.h"
#endif

#include <string>

#if ENABLE_ZMQ
#include <zmq.h>
#endif

#include "primitives/block.h"

void zmqError(const char *str);

#endif // MVC_ZMQ_ZMQCONFIG_H
