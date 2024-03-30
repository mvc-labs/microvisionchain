# MVC version 0.2.0 Release Notes


* Scale the default max block size up to 4 Gigabytes from 128 megabytes(activation timestamp 1713225600).
* Added more intuitive startup configuration items.
* Hardcode the ASERT-DAA's anchor parameters, do simple check  instead of MTP check.
* Modify ChainTxData to make `verificationprogress` reasonable.
* Add new configuration items to control blackhole address's CoinsViewCache.
* The index service for P2PKH and MetaContract Fungible Token (FT) assets.
* Add fixed seed nodes and the script (in python) to generate list of seed nodes.
* Add some macro definitions for new chain creation from MVC.
* Support cross-platform compilation (on going) .
* Add new checkpoints.
* Bug fixed and redundant code removed;
* Copyright updated.
* Version updated.

See details [here](./release-notes-v0.2.0.md) .

# MVC version 0.1.4 Release Notes

Added ASERT difficulty adjustment algorithm with two days half-life, aka, aserti3-2d. 
This upgrade uses a MTP-based activation,1686636000(2023-06-13 06:00:00 +0000) for mainnet and 1685426400(2023-05-30 06:00:00 +0000) for testnet.
The block before the activation block will be used as the anchor block for the ASERT algorithm.
It is highly recommended to upgrade this update, otherwise it will fork after ASERT activation.

Added parameter minrelaytxfee. 
Fees smaller than which are considered zero fee for relaying, mining and transaction creation (default: 250). 
If you set it to zero then a transaction spammer can cheaply fill blocks using 1-satoshi-fee transactions, so which is not allowed. 
If you are mining, be careful setting this, it should be set above the real cost to you of processing a transaction.

Upgraded minimum chain work and added new checkpoints;

Bug fixed and redundant code removed;

Update version to v0.1.4.0



# MVC version 0.1.3.1 Release Notes

Remove dustlimitfactor and dustrelayfee settings.

Hard code dust threshold to zero to allow 1 sat transaction outputs.

Bug fix.




# MVC Node software – Upgrade to v0.1.3 Release

Significant improvement in the performance of node validation of transactions.

Improved node performance for processing smart contracts.

New RPC method for receiving UTXOs.

Increased the default limit for node processing of smart contract scripts.

Significant improvement in the ability of nodes to parallel process transactions and smart contracts.

Allows parallel shard downloads of the same block from multiple nodes.

Enhances the connectivity of the MVC network.

Notification information for new block double-spending.

Added Webhook interface for double-spending and fork detection.

Removed the ancestor chain quantity limit.

Added RPC to provide Merkle proof that transactions are included in blocks.

Fixed known issues in many past versions.