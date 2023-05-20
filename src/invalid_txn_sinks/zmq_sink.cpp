// Copyright (c) 2021-2023 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include "zmq_sink.h"

#include "validationinterface.h"

using namespace InvalidTxnPublisher;

#if ENABLE_ZMQ
void CInvalidTxnZmqSink::Publish(const InvalidTxnInfo& invalidTxInfo)
{
    auto messageSize = EstimateMessageSize(invalidTxInfo, true);
    CStringWriter tw;
    CJSONWriter jw(tw, false);
    invalidTxInfo.ToJson(jw, messageSize <= maxMessageSize);
    std::string jsonString = tw.MoveOutString();
    GetMainSignals().InvalidTxMessageZMQ(jsonString);
}
#endif