// SPDX-License-Identifier: MIT License
pragma solidity ^0.8.19;

/*
    The 25 EVM Opcodes for uint256 operations.

    See: https://www.evm.codes/
*/
abstract contract Uint256 {
    // arithmetic
    function Add(uint x, uint y) public pure virtual returns (uint z);
    function Mul(uint x, uint y) public pure virtual returns (uint z);
    function Sub(uint x, uint y) public pure virtual returns (uint z);
    function Div(uint x, uint y) public pure virtual returns (uint z);
    function SDiv(uint x, uint y) public pure virtual returns (uint z);
    function Mod(uint x, uint y) public pure virtual returns (uint z);
    function SMod(uint x, uint y) public pure virtual returns (uint z);
    function AddMod(uint x, uint y, uint m) public pure virtual returns (uint);
    function MulMod(uint x, uint y, uint m) public pure virtual returns (uint);
    function Exp(uint x, uint y) public pure virtual returns (uint z);
    function SignExtend(uint x, uint y) public pure virtual returns (uint z);

    // comparison
    function Lt(uint x, uint y) public pure virtual returns (bool z);
    function Gt(uint x, uint y) public pure virtual returns (bool z);
    function Slt(uint x, uint y) public pure virtual returns (bool z);
    function Sgt(uint x, uint y) public pure virtual returns (bool z);
    function Eq(uint x, uint y) public pure virtual returns (bool z);
    function IsZero(uint x) public pure virtual returns (bool z);

    // bitwise
    function And(uint x, uint y) public pure virtual returns (uint z);
    function Or(uint x, uint y) public pure virtual returns (uint z);
    function Xor(uint x, uint y) public pure virtual returns (uint z);
    function Not(uint x) public pure virtual returns (uint z);
    function Byte(uint x, uint y) public pure virtual returns (uint z);
    function Shl(uint x, uint shift) public pure virtual returns (uint z);
    function Shr(uint x, uint shift) public pure virtual returns (uint z);
    function Sar(uint x, uint shift) public pure virtual returns (uint z);
}
