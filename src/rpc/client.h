// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2022 The Bitcoin Core developers
// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <rpc/client_config.h>

namespace rpc::client
{

class HTTPRequest;
class HTTPResponse;

/**
 * Class to perform HTTP RPC/REST requests.
 */
class RPCClient
{
  public:
    explicit RPCClient(const RPCClientConfig& config) : mConfig{config} {}

    // Submit a request and wait for a response
    void SubmitRequest(HTTPRequest& request, HTTPResponse* response) const;

  private:

    // Config to describe the required connection type
    RPCClientConfig mConfig {};
};

}

