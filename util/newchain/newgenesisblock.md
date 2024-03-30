This document describes a simple way to generate a new genesis block

## Tips
Suitable for low difficulty genesis block only.

## Steps

1. Change the macro definition `NEW_GENESIS_BLOCK` to `1` (defined in src/chainparams.cpp) .
2. Parameters :
    ```
        nTime : the time the new genesis block is generated
        nBits : usually 0x1d00ffff (difficulty = 1)
        genesisReward : usually 50 * COIN
    ```
3. Fill above parameters into where `newGenesisBlock(...)` is called (class CMainParams in src/chainparams.cpp) .
4. Rebuild the project and run the node binary.This could take a long time before you see the result as below:
    ```
    ----------NEW GENESIS BLOCK--------
    nTime : 1542305817 
    nBits : 486604799 
    nonce : 781518760 
    hash  : 000000005abc148d1e91534997189fcce59056c980a7164e8eca2e2ce29a1575 
    mrekle: 921c9ad4264610101e46b4e67b7c030fbcc4ca9633bbd40b3079a62cf3ef531d 
    ```
5. Change the macro definition `NEW_GENESIS_BLOCK` back to `0` .
6. Fill above parameters into where `CreateGenesisBlock()` is called (class CMainParams in src/chainparams.cpp).
    ```c
    ...

    #if NEW_GENESIS_BLOCK
            genesis = newGenesisBlock(1542305817,0x1d00ffff,0x20000000, 50 * COIN, consensus.powLimit,false);
            exit(0);
    #endif
            // replace nTime nonce nBits here
            genesis = CreateGenesisBlock(1542305817, 781518760, 486604799, 0x20000000,
                                        50 * COIN, false);
            consensus.hashGenesisBlock = genesis.GetHash();
            // replace hash here
            assert(consensus.hashGenesisBlock ==
                uint256S("000000005abc148d1e91534997189fcce59056c980a7164e8eca2e2ce29a1575"));
            //    replace mrekle here
            assert(genesis.hashMerkleRoot ==
                uint256S("921c9ad4264610101e46b4e67b7c030fbcc4ca9633bbd40b3079a62cf3ef531d"));
    ...
    
    ```
7. Rebuild the project and run the node binary, the new genesis block will be generated.