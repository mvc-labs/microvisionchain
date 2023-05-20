// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2022 The Bitcoin Core developers
// Copyright (c) 2021-2023 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <rpc/http_response.h>

namespace rpc::client
{

void StringHTTPResponse::SetBody(const unsigned char* body, size_t size)
{
    if(body)
    {
        const char* data { reinterpret_cast<const char*>(body) };
        mBody = { data, size };
    }
}

void BinaryHTTPResponse::SetBody(const unsigned char* body, size_t size)
{
    if(body)
    {
        const uint8_t* data { reinterpret_cast<const uint8_t*>(body) };
        mBody = { data, data + size };
    }
}

}

