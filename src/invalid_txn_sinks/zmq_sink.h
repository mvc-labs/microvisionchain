// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "invalid_txn_publisher.h"

namespace InvalidTxnPublisher
{
#if ENABLE_ZMQ
    class CInvalidTxnZmqSink : public CInvalidTxnSink
    {
        int64_t maxMessageSize;
    public:
        CInvalidTxnZmqSink(int64_t maxMsgSize)
            :maxMessageSize(maxMsgSize)
        {}

        void Publish(const InvalidTxnInfo& invalidTxInfo) override;
    };
#endif
}