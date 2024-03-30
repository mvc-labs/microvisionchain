// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2022 The Bitcoin Core developers
// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MVC_RPCTOJSON_H
#define MVC_RPCTOJSON_H

#include "uint256.h"
#include "text_writer.h"
#include "httpserver.h" //for HTTPRequest
#include <univalue.h>

class CScript;

// Following functions are implemented in blockchain.cpp
void headerBlockToJSON(const Config& config, const CBlockHeader& blockHeader,
                       const CBlockIndex* blockindex, const CDiskBlockMetaData& diskBlockMetaData,
                       const int confirmations, const std::optional<uint256>& nextBlockHash, CJSONWriter& jWriter);
UniValue blockheaderToJSON(const CBlockIndex *blockindex, 
                           const int confirmations, 
                           const std::optional<uint256>& nextBlockHash);

UniValue blockStatusToJSON(const BlockStatus&);

// Following functions are implemented in rawtransaction.cpp
void getrawtransaction(const Config& config,
                       const JSONRPCRequest& request,
                       HTTPRequest& httpReq,
                       bool processedInBatch);
void getrawtransaction(const Config& config,
                       const JSONRPCRequest& request,
                       CTextWriter& textWriter,
                       bool processedInBatch,
                       std::function<void()>HttpCallback);
void decoderawtransaction(const Config& config,
                          const JSONRPCRequest& request,
                          HTTPRequest& httpReq, 
                          bool processedInBatch);
void decoderawtransaction(const Config& config,
                          const JSONRPCRequest& request,
                          CTextWriter& textWriter,
                          bool processedInBatch,
                          std::function<void()>HttpCallback);



#endif // MVC_RPCTOJSON_H
