// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <net/node_state.h>

/** Map maintaining per-node state. */
std::map<NodeId, CNodeStatePtr> mapNodeState {};
std::shared_mutex mapNodeStateMtx {};

// Fetch node state
CNodeStateRef GetState(NodeId pnode)
{
    // Lock access for reading to the map of node states
    std::shared_lock<std::shared_mutex> lock { mapNodeStateMtx };

    auto it { mapNodeState.find(pnode) };
    if(it == mapNodeState.end()) {
        // Not found, return null
        return {};
    }
    // Return a shared ref to the item in the map, locked appropriately
    return { it->second, it->second->mMtx };
}

