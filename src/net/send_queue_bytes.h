// Copyright (c) 2021-2023 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#pragma once

#include <atomic>

class CSendQueueBytes
{
private:
    // nSendQueueBytes holds data of how many bytes are currently in queue for specific node
    size_t nSendQueueBytes = 0;
    // nTotalSendQueuesBytes holds data of how many bytes are currently in all queues across the network (all nodes)
    static std::atomic_size_t nTotalSendQueuesBytes;

public:
    ~CSendQueueBytes() {
        nTotalSendQueuesBytes -= nSendQueueBytes;
    }

    size_t operator-= (size_t nBytes) {
        nSendQueueBytes -= nBytes;
        nTotalSendQueuesBytes -= nBytes;
        return nSendQueueBytes;
    }

     size_t operator+= (size_t nBytes) {
        nSendQueueBytes += nBytes;
        nTotalSendQueuesBytes += nBytes;
        return nSendQueueBytes;
    }

    size_t getSendQueueBytes() const {
        return nSendQueueBytes;
    }

    static size_t getTotalSendQueuesBytes() {
        return nTotalSendQueuesBytes;
    }
};

