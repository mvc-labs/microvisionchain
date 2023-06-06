# MVC version 0.1.4 Release Notes

## List of Changes

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