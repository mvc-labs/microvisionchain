Shared Libraries
================

## mvcconsensus

The purpose of this library is to make the verification functionality that is critical to mvc's consensus available to other applications, e.g. to language bindings.

### API

The interface is defined in the C header `mvcconsensus.h` located in  `src/script/mvcconsensus.h`.

#### Version

`mvcconsensus_version` returns an `unsigned int` with the API version *(currently at an experimental `0`)*.

#### Script Validation

`mvcconsensus_verify_script` returns an `int` with the status of the verification. It will be `1` if the input script correctly spends the previous output `scriptPubKey`.

##### Parameters
- `const unsigned char *scriptPubKey` - The previous output script that encumbers spending.
- `unsigned int scriptPubKeyLen` - The number of bytes for the `scriptPubKey`.
- `const unsigned char *txTo` - The transaction with the input that is spending the previous output.
- `unsigned int txToLen` - The number of bytes for the `txTo`.
- `unsigned int nIn` - The index of the input in `txTo` that spends the `scriptPubKey`.
- `unsigned int flags` - The script validation flags *(see below)*.
- `mvcconsensus_error* err` - Will have the error/success code for the operation *(see below)*.

##### Script Flags
- `mvcconsensus_SCRIPT_FLAGS_VERIFY_NONE`
- `mvcconsensus_SCRIPT_FLAGS_VERIFY_P2SH` - Evaluate P2SH BIP16 subscripts
- `mvcconsensus_SCRIPT_FLAGS_VERIFY_DERSIG` - Enforce strict DER BIP66 compliance
- `mvcconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY` - Enforce NULLDUMMY BIP147
- `mvcconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY` - Enable CHECKLOCKTIMEVERIFY BIP65
- `mvcconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY` - Enable CHECKSEQUENCEVERIFY BIP112
- `mvcconsensus_SCRIPT_FLAGS_VERIFY_WITNESS` - Enable WITNESS BIP141

##### Errors
- `mvcconsensus_ERR_OK` - No errors with input parameters *(see the return value of `mvcconsensus_verify_script` for the verification status)*
- `mvcconsensus_ERR_TX_INDEX` - An invalid index for `txTo`
- `mvcconsensus_ERR_TX_SIZE_MISMATCH` - `txToLen` did not match with the size of `txTo`
- `mvcconsensus_ERR_DESERIALIZE` - An error deserializing `txTo`
- `mvcconsensus_ERR_AMOUNT_REQUIRED` - Input amount is required if WITNESS is used
