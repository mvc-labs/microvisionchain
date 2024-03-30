// Copyright (c) 2021-2024 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "journal_entry.h"
#include "txmempool.h"

namespace mining
{
CJournalEntry::CJournalEntry(const CTransactionWrapperRef& txn,
                             uint64_t txnSize,
                             const Amount& fee,
                             GroupID groupId,
                             bool isCpfpGroupPayingTx)
    : mTxn{txn},
      mTxnSize{txnSize},
      mFee{fee},
      mGroupId{groupId},
      isCpfpPayingTx{isCpfpGroupPayingTx}
{}

CJournalEntry::CJournalEntry(const CTxMemPoolEntry& entry)
    : CJournalEntry{entry.tx,
                    entry.GetTxSize(),
                    entry.GetFee(),
                    entry.GetCPFPGroupId(),
                    entry.GetCPFPGroup() ? (entry.GetTxId() == entry.GetCPFPGroup()->PayingTransactionId()) : false}
{}

}
