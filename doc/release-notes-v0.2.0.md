# MVC version 0.2.0 Release Notes

## Changes since 0.1.4

* Scale the default max block size up to 4 Gigabytes from 128 megabytes(activation timestamp 1713225600).


* Added more intuitive startup configuration items.

        firstBlockReward            : reward of the first block in satoshi
        initialReward               : initial reward after the first block in satoshi
        subsidyHalvingInterval      : interval(in blocks) of the  subsidy halving
        firstBlockGenesisLockScript : the public key hash which the first block's reward must send to
        genesisLockHeight           : how many blocks the reward must send to the appointed PKH

This is a replacement for the original configuration "`-chaininitparam`" . 

Both "`-chaininitparam`" and the new items can be configured in this version. One thing to note , if "`-chaininitparam`" is
configured, the node will give priority to "`-chaininitparam`" and ignore the new items. Which means, if you want to use the new configurations
Mentioned above, the "`-chaininitparam`" must be deleated from mvc.conf.
* Hardcode the ASERT-DAA's anchor parameters, do simple check  instead of MTP check.

The ASERT-DAA became avtivied at block 21256, hardcoded the anchor block's bits and prevblock's timestamp to chain parameters.

```c
consensus.asertAnchorParams = Consensus::Params::ASERTAnchor {
    21256,         // anchor block height
    0x1836845b,    // anchor block nBits
    1686641614,    // anchor block previous block timestamp
};
```

Optimize the process of ASERT avtivation check, and add block 21256 to checkpoints.

* Modify ChainTxData to make `verificationprogress` reasonable.

Take block `00000000000000000a6d8c52708fde5cb1cb84c33dab6120315a9ea6e13e96c8`(height 63095) as anchor. 

* Add new configuration items to control blackhole address's CoinsViewCache.

`-cacheBlockHoleAddress` : default `true` 

Once `-cacheBlockHoleAddress` is configured as `false`, the TX outs send to blackhole address will be marked as unspendable.
Which means these TX outs will not be added to `CoinsViewCache` .

* The index service for P2PKH and MetaContract Fungible Token (FT) assets.

A data service that contains index APIs such as : 

`broadcast transaction`

`query P2PKH UTXO and balance by an address`

`query FT UTXO and balance by an address`

More details [here](https://github.com/mvc-labs/mvc-assets-indexer) .

* Add fixed seed nodes and the script (in python) to generate list of seed nodes.

[/util/seeds/README.md](../util/seeds/README.md)

* Add some macro definitions for new chain creation from MVC.

```
CHAIN_GENERATION_WITH_HARDCODED_GENESIS : Whether or not to create new chain with hardcoded genesisblock

NEW_GENESIS_BLOCK : to generate a new genesis block

DAA_PROOF_WORKS_MODIFY : Pure chainwork-144 DAA or not
```

To generate a new genesis block : [/util/newchain/newgenesisblock.md](../util/newchain/newgenesisblock.md)

To create a new chain : [/util/newchain/README.md](../util/newchain/README.md)

Notification : [/util/newchain/notice.md](../util/newchain/notice.md)

* Support cross-platform compilation (on going) .

Add support for cross-platform compilation (darwin/bsd/windows) .

Please notice that the developing is still on going ,and without fully tested.

The current recommended build environment is still ubuntu 20.04

* Add new checkpoints.

* Bug fixed and redundant code removed;

* Copyright updated.

* Version updated.


# Previous Releases
* [Version 0.1.2](release-notes-v0.1.2.md) - [Download](https://github.com/mvc-labs/mvc-mining-instruction/releases/download/v0.1.2.0/mvc.tar.gz) - [Source](https://github.com/mvc-labs/mvc-mining-instruction/archive/refs/tags/v0.1.2.0.zip) - 2023-1-28
* [Version 0.1.3](release-notes-v0.1.3.md) - [Download](https://github.com/mvc-labs/mvc-mining-instruction/releases/download/v0.1.3.0/mvc.tar.gz) - [Source](https://github.com/mvc-labs/mvc-mining-instruction/archive/refs/tags/v0.1.3.0.zip) - 2023-4-3
* [Version 0.1.3.1](release-notes-v0.1.3.1.md) - [Download](https://github.com/mvc-labs/mvc-mining-instruction/releases/download/v0.1.3.1/mvc.tar.gz) - [Source](https://github.com/mvc-labs/mvc-mining-instruction/archive/refs/tags/v0.1.3.1.zip) - 2023-4-24
* [Version 0.1.4](release-notes-v0.1.4.md) - [Download](https://github.com/mvc-labs/mvc-mining-instruction/releases/download/v0.1.4.0/mvc.tar.gz) - [Source](https://github.com/mvc-labs/mvc-mining-instruction/archive/refs/tags/v0.1.4.0.zip) - 2023-6-6

