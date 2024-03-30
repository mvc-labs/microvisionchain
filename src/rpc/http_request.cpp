// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2022 The Bitcoin Core developers
// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <rpc/http_request.h>
#include <rpc/protocol.h>

namespace rpc::client
{

// Create a properly formatted JSONRPCRequest
HTTPRequest HTTPRequest::CreateJSONRPCRequest(const RPCClientConfig& config, const std::string& method, const UniValue& params)
{
    // Format contents
    std::string contents { JSONRPCRequestObj(method, params, 1).write() + "\n" };

    // Format endpoint
    std::string endPoint {"/"};
    if(!config.GetWallet().empty())
    {
        // We need to pass a wallet ID via the URI
        endPoint = "/wallet/" + EncodeURI(config.GetWallet());
    }

    // Create request
    return { endPoint, contents, RequestCmdType::POST };
}

// Create a generic JSON POST request
HTTPRequest HTTPRequest::CreateJSONPostRequest(const RPCClientConfig& config, const UniValue& json)
{
    // Format contents
    std::string contents { json.write() + "\r\n" };
    return CreateJSONPostRequest(config, std::move(contents));
}

// Create a generic JSON POST request
HTTPRequest HTTPRequest::CreateJSONPostRequest(const RPCClientConfig& config, const std::string contents)
{
    // Create request
    HTTPRequest request { config.GetEndpoint(), contents, RequestCmdType::POST };
    request.AddHeader({"Content-Type", "application/json"});
    return request;
}

// Create a properly formatted query request to a double-spend endpoint
HTTPRequest HTTPRequest::CreateDSEndpointQueryRequest(const RPCClientConfig& config, const std::string& txid)
{
    // Format endpoint
    std::string endpoint { config.GetEndpoint() + "query/" + txid };

    // Create request
    return { endpoint, RequestCmdType::GET };
}

}

