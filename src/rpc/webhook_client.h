// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "rpc/http_request.h"
#include "rpc/http_response.h"
#include "threadpool.h"

#include <future>
#include <memory>

class Config;

namespace rpc::client
{

/**
 * A class to manage sending of HTTP requests to a webhook endpoint.
 * Requests are sent asynchronously and the result returned in a future.
 */

class WebhookClient
{
  public:

    WebhookClient(const Config& config);
    ~WebhookClient() = default;

    // No copying / moving
    WebhookClient(const WebhookClient&) = delete;
    WebhookClient(WebhookClient&&) = delete;
    WebhookClient& operator=(const WebhookClient&) = delete;
    WebhookClient& operator=(WebhookClient&&) = delete;

    /** 
     * Submit the given request to the specified server.
     *
     * Takes ownership of the passed in request and response objects, and
     * returns the filled in response via a std::future once the result
     * becomes available.
     *
     * FIXME: Would be better if it used unique_ptrs instead of shared_ptrs,
     * but will have to wait until MSVC fixes this bug:
     * https://developercommunity.visualstudio.com/t/unable-to-move-stdpackaged-task-into-any-stl-conta/108672
     */
    std::future<std::shared_ptr<HTTPResponse>> SubmitRequest(
        const RPCClientConfig& clientConfig,
        std::shared_ptr<HTTPRequest>&& request,
        std::shared_ptr<HTTPResponse>&& response);

  private:

    // A thread pool for asynchronously submitting HTTP requests.
    // Leave as the last member of the class so that it is destroyed first.
    CThreadPool<CQueueAdaptor> mSubmitPool;

};

extern std::unique_ptr<WebhookClient> g_pWebhookClient;

}   // namespace

