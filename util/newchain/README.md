# New chain creation
This document describes how to create a new chain from MVC.

## Genesis block
The hardcoded genesis block is as below:
 ```json
 The hardcoded genesis block:

    Time      : 2018-11-15 18:16:57 (UTC)
    Nonce     : 0x4D8FDFF4
    Bits      : 0x18021FDB
    Version   : 0x20000000
    Reward    : 5000000000 (satoshi)
    PrevBlock : "00000000000000000102d94fde9bd0807a2cc7582fe85dd6349b73ce4e8d9322"
    MerkleRoot: "da2b9eb7e8a3619734a17b55c47bdd6fd855b0afa9c7e14e3a164a279e51bba9"
```
To create a new chain with the hardcoded genesis block, change macro definition `DAA_PROOF_WORKS_MODIFY` to 0, 
nothing else  has to be changed in this part.

Or, to create a chain with new genesis block: 

Change the macro definition `DAA_PROOF_WORKS_MODIFY` and `CHAIN_GENERATION_WITH_HARDCODED_GENESIS` to 0 ,
and generate the new genesis block.

Here is [a sample way to generate new genesis block](./newgenesisblock.md) .


## Configuration parameters
Some of the parameters need to be changed( src/chainparams.cpp) .

1. `diskMagic` - Hardcoded as 0xA55AC369
2. `netMagic`  - Hardcoded as 0x96689B52
3. `nDefaultPort` - Hardcoded as 9883,which can be configured in mvc.conf
4. `nMinimumChainWork` - Change the hardcode value or configured in mvc.conf
5. `checkpointData` 
6. `ChainTxData`

## Forks' height
1. `BitcoinSoftForksHeight` - Hardcoded as 8000
    
    Block height at which BIP34 BIP65 BIP66 BIP68 BIP112 and BIP113 become active .

2. `asertActivationTime` and `asertAnchorParams`
    MVC mainnet use chainwork-144 as difficulty adjustment algorithm (DAA) before height 21256, then the DAA changed to 
    aserti3-2d .
    The upgrade was deployed by MedianTimePast check.
    Set the `asertActivationTime` to an appropriate  time and  commented out the `asertAnchorParams`;
    when aserti3-2d becomes active, hardcode the `asertAnchorParams`;

## Seeds

To generate fixed seeds, follow the guide [here](../seeds/README.md) .

Or DNS seeds, follow sipa's [Bitcoin-seeder](https://github.com/sipa/bitcoin-seeder) .