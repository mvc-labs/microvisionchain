// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MVC_ABORTNODE_H
#define MVC_ABORTNODE_H

#include <iostream>
#include "consensus/validation.h"

bool AbortNode(const std::string &strMessage,
               const std::string &userMessage = "");

bool AbortNode(CValidationState &state, const std::string &strMessage,
               const std::string &userMessage = "");

#endif
