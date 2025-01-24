## CEtherOps
The purpose of this repository is to implement EVM equivalent operations over 256 bit integers in C, so that it can be used in Stylus contracts. This project is a port of [uint256.go](https://github.com/holiman/uint256/), which is the integer library used by [go-ethereum](https://github.com/ethereum/go-ethereum).

#### The Opcodes
There are 25 opcodes for integer operations. General descriptions of all EVM opcodes can be seen at [evm.codes](https://evm.codes/) and in the Solidity docs for [Yul](https://docs.soliditylang.org/en/latest/yul.html#evm-dialect). You can see how they're implemented in [go-ethereum/core/vm/instructions.go](https://github.com/ethereum/go-ethereum/blob/master/core/vm/instructions.go#L30-L231) and [uint256/uint256.go](https://github.com/holiman/uint256/blob/master/uint256.go).

**Supported Opcodes**
|     |     |     |     |     |
|-----|-----|-----|-----|-----|
| Add | Mul | Sub | Div | SDiv |
| Mod | SMod | AddMod | MulMod | Exp |
| SignExtend | Lt | Gt | Slt | Sgt |
| Eq | IsZero | And | Or | Xor |
| Not | Byte | Shl | Shr | Sar |

## Design Goals
The end goal of this project is to have an importable library in C Stylus contracts.
```c
#include <CEtherOps/uint256.h>
```

## Testing
There are two tests to run for this project.

#### C
The C tests are broken into two parts:
1. Statically determined [tests](./test/uint256.t.c) in C
2. Randomly generated [tests](./test/randombytes.go) where the source of truth is the Golang uint256.go library compiled to be used in C

Run the C/Go test:
```sh
make testc
```

#### EVM
The EVM tests are deployed to the Arbitrum Sepolia testnet. We randomly generate inputs using Javascript, then call the test function, where the source of truth for test results is computed in the EVM itself. See [this file](./test/uint256.t.sol) to see how the Solidity tests compare the Stylus results to the EVM results.

To run the Solidity test, first install the npm dependencies:
```sh
npm install .
```
Then run the EVM test:
```sh
make testsol
```

## Build & Deploy
To build the Stylus contract, run:
```sh
make
```
To deploy it, you need a testnet account with testnet ETH. Set the following environment variables:
```text
PRIVATE_KEY=0x...
ENDPOINT=https://sepolia-rollup.arbitrum.io/rpc
```
Then run the command:
```sh
cargo stylus deploy --wasm-file=build/uint256_stripped.wasm --private-key=$PRIVATE_KEY --endpoint=$ENDPOINT
```
alternatively, run:
```sh
bash scripts/deploy.sh
```
