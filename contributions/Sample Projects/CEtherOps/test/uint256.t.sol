// SPDX-License-Identifier: MIT License
pragma solidity ^0.8.19;


interface Uint256 {
    // arithmetic
    function Add(uint x, uint y) external pure returns (uint z);
    function Mul(uint x, uint y) external pure returns (uint z);
    function Sub(uint x, uint y) external pure returns (uint z);
    function Div(uint x, uint y) external pure returns (uint z);
    function SDiv(uint x, uint y) external pure returns (uint z);
    function Mod(uint x, uint y) external pure returns (uint z);
    function SMod(uint x, uint y) external pure returns (uint z);
    function AddMod(uint x, uint y, uint m) external pure returns (uint);
    function MulMod(uint x, uint y, uint m) external pure returns (uint);
    function Exp(uint x, uint y) external pure returns (uint z);
    function SignExtend(uint x, uint y) external pure returns (uint z);

    // comparison
    function Lt(uint x, uint y) external pure returns (bool z);
    function Gt(uint x, uint y) external pure returns (bool z);
    function Slt(uint x, uint y) external pure returns (bool z);
    function Sgt(uint x, uint y) external pure returns (bool z);
    function Eq(uint x, uint y) external pure returns (bool z);
    function IsZero(uint x) external pure returns (bool z);

    // bitwise
    function And(uint x, uint y) external pure returns (uint z);
    function Or(uint x, uint y) external pure returns (uint z);
    function Xor(uint x, uint y) external pure returns (uint z);
    function Not(uint x) external pure returns (uint z);
    function Byte(uint x, uint y) external pure returns (uint z);
    function Shl(uint x, uint shift) external pure returns (uint z);
    function Shr(uint x, uint shift) external pure returns (uint z);
    function Sar(uint x, uint shift) external pure returns (uint z);
}

contract Uint256Test {
    Uint256 public stylusContract = Uint256(0x54FBcd29133d7C0aB47AA952348D8354232a5870);

    function test_values(uint256[] memory values, uint256 runners)
        public view returns (uint, uint, uint, uint, uint, uint, uint)
    {
        uint have;
        uint want;
        uint x;
        uint y;
        uint z;
        for (uint i = 0; i < (values.length/3); i+=3) {
            have;
            want;
            x = values[i];
            y = values[i+1];
            z = values[i+2];

            if ((runners & 1) > 0) {
                // Add
                have = stylusContract.Add(x, y);
                assembly {
                    want := add(x, y)
                }
                if (have != want) return (0, 0, have, want, x, y, z);
            }

            if ((runners & 2) > 0) {
                // Mul
                have = stylusContract.Mul(x, y);
                assembly {
                    want := mul(x, y)
                }
                if (have != want) return (0, 1, have, want, x, y, z);
            }

            if ((runners & 4) > 0) {
                // Sub
                have = stylusContract.Sub(x, y);
                assembly {
                    want := sub(x, y)
                }
                if (have != want) return (0, 2, have, want, x, y, z);
            }

            if ((runners & 8) > 0) {
                // Div
                have = stylusContract.Div(x, y);
                assembly {
                    want := div(x, y)
                }
                if (have != want) return (0, 3, have, want, x, y, z);
            }

            if ((runners & 16) > 0) {
                // SDiv
                have = stylusContract.SDiv(x, y);
                assembly {
                    want := sdiv(x, y)
                }
                if (have != want) return (0, 4, have, want, x, y, z);
            }

            if ((runners & 32) > 0) {
                // Mod
                have = stylusContract.Mod(x, y);
                assembly {
                    want := mod(x, y)
                }
                if (have != want) return (0, 5, have, want, x, y, z);
            }

            if ((runners & 64) > 0) {
                // SMod
                have = stylusContract.SMod(x, y);
                assembly {
                    want := smod(x, y)
                }
                if (have != want) return (0, 6, have, want, x, y, z);
            }

            if ((runners & 128) > 0) {
                // AddMod
                have = stylusContract.AddMod(x, y, z);
                assembly {
                    want := addmod(x, y, z)
                }
                if (have != want) return (0, 7, have, want, x, y, z);
            }

            if ((runners & 256) > 0) {
                // MulMod
                have = stylusContract.MulMod(x, y, z);
                assembly {
                    want := mulmod(x, y, z)
                }
                if (have != want) return (0, 8, have, want, x, y, z);
            }

            if ((runners & 512) > 0) {
                // Exp
                have = stylusContract.Exp(x, y);
                assembly {
                    want := exp(x, y)
                }
                if (have != want) return (0, 9, have, want, x, y, z);
            }

            if ((runners & 1024) > 0) {
                // SignExtend
                have = stylusContract.SignExtend(x, y);
                assembly {
                    want := signextend(y, x)
                }
                if (have != want) return (0, 10, have, want, x, y, z);
            }

            /*
               Comparisons
            */
            if ((runners & 2048) > 0) {
                // Lt
                have = stylusContract.Lt(x, y) ? 1 : 0;
                assembly {
                    want := lt(x, y)
                }
                if (have != want) return (0, 11, have, want, x, y, z);
            }

            if ((runners & 4096) > 0) {
                // Gt
                have = stylusContract.Gt(x, y) ? 1 : 0;
                assembly {
                    want := gt(x, y)
                }
                if (have != want) return (0, 12, have, want, x, y, z);
            }

            if ((runners & 8192) > 0) {
                // Slt
                have = stylusContract.Slt(x, y) ? 1 : 0;
                assembly {
                    want := slt(x, y)
                }
                if (have != want) return (0, 13, have, want, x, y, z);
            }

            if ((runners & 16384) > 0) {
                // Sgt
                have = stylusContract.Sgt(x, y) ? 1 : 0;
                assembly {
                    want := sgt(x, y)
                }
                if (have != want) return (0, 14, have, want, x, y, z);
            }

            if ((runners & 32768) > 0) {
                // Eq
                have = stylusContract.Eq(x, y) ? 1 : 0;
                assembly {
                    want := eq(x, y)
                }
                if (have != want) return (0, 15, have, want, x, y, z);
            }

            if ((runners & 65536) > 0) {
                // IsZero
                have = stylusContract.IsZero(x) ? 1 : 0;
                assembly {
                    want := iszero(x)
                }
                if (have != want) return (0, 16, have, want, x, y, z);
            }

            /*
                Bitwise
            */
            if ((runners & 131072) > 0) {
                // And
                have = stylusContract.And(x, y);
                assembly {
                    want := and(x, y)
                }
                if (have != want) return (0, 17, have, want, x, y, z);
            }

            if ((runners & 262144) > 0) {
                // Or
                have = stylusContract.Or(x, y);
                assembly {
                    want := or(x, y)
                }
                if (have != want) return (0, 18, have, want, x, y, z);
            }

            if ((runners & 524288) > 0) {
                // Xor
                have = stylusContract.Xor(x, y);
                assembly {
                    want := xor(x, y)
                }
                if (have != want) return (0, 19, have, want, x, y, z);
            }

            if ((runners & 1048576) > 0) {
                // Not
                have = stylusContract.Not(x);
                assembly {
                    want := not(x)
                }
                if (have != want) return (0, 20, have, want, x, y, z);
            }

            if ((runners & 2097152) > 0) {
                // Byte
                have = stylusContract.Byte(x, y);
                assembly {
                    want := byte(y, x)
                }
                if (have != want) return (0, 21, have, want, x, y, z);
            }

            if ((runners & 4194304) > 0) {
                // Shl
                have = stylusContract.Shl(x, y);
                assembly {
                    want := shl(y, x)
                }
                if (have != want) return (0, 22, have, want, x, y, z);
            }

            if ((runners & 8388608) > 0) {
                // Shr
                have = stylusContract.Shr(x, y);
                assembly {
                    want := shr(y, x)
                }
                if (have != want) return (0, 23, have, want, x, y, z);
            }

            if ((runners & 16777216) > 0) {
                // Sar
                have = stylusContract.Sar(x, y);
                assembly {
                    want := sar(y, x)
                }
                if (have != want) return (0, 24, have, want, x, y, z);
            }
        }
        return (1, 0, 0, 0, 0, 0, 0);
    }
}
