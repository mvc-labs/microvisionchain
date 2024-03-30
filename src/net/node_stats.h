// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <net/net_types.h>

class StreamStats
{
public:
    std::string streamType;
    int64_t nLastSend;
    int64_t nLastRecv;
    uint64_t nSendBytes;
    uint64_t nRecvBytes;
    uint64_t nSendSize;
    uint64_t nRecvSize;
    uint64_t nSpotBytesPerSec;
    uint64_t nMinuteBytesPerSec;
    bool fPauseRecv;
    mapMsgCmdSize mapSendBytesPerMsgCmd;
    mapMsgCmdSize mapRecvBytesPerMsgCmd;
};

class AssociationStats
{
public:
    int64_t nLastSend;
    int64_t nLastRecv;
    CAddress addr;
    mapMsgCmdSize mapSendBytesPerMsgCmd;
    mapMsgCmdSize mapRecvBytesPerMsgCmd;
    uint64_t nSendBytes;
    uint64_t nRecvBytes;
    uint64_t nSendSize;
    uint64_t nRecvSize;
    uint64_t nAvgBandwidth;

    std::vector<StreamStats> streamStats;
    std::string assocID;
    std::string streamPolicyName;
};

class NodeStats
{
public:
    NodeId nodeid;
    ServiceFlags nServices;
    bool fRelayTxes;
    bool fPauseSend;
    bool fUnpauseSend;
    bool fInbound;
    bool fAddnode;
    bool fWhitelisted;
    int64_t nTimeConnected;
    int64_t nTimeOffset;
    std::string addrName;
    int nVersion;
    std::string cleanSubVer;
    int nStartingHeight;
    double dPingTime;
    double dPingWait;
    double dMinPing;
    // What this peer sees as my address
    std::string addrLocal;
    size_t nInvQueueSize;

    AssociationStats associationStats;
};
