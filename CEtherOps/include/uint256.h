#include <uint256_core.h>

/*
    The 25 EVM Opcodes for uint256 operations.

    See: https://www.evm.codes/
*/

// arithmetic
void u256_add(u256 res, u256 x, u256 y);
void u256_mul(u256 res, u256 x, u256 y);
void u256_sub(u256 res, u256 x, u256 y);
void u256_div(u256 res, u256 x, u256 y);
void u256_sdiv(u256 res, u256 x, u256 y);
void u256_mod(u256 res, u256 x, u256 m);
void u256_smod(u256 res, u256 x, u256 m);
void u256_add_mod(u256 res, u256 x, u256 y, u256 m);
void u256_mul_mod(u256 res, u256 x, u256 y, u256 m);
void u256_exp(u256 res, u256 x, u256 exponent);
void u256_sign_extend(u256 res, u256 b, u256 x);

// comparison
bool u256_lt(u256 x, u256 y);
bool u256_gt(u256 x, u256 y);
bool u256_slt(u256 x, u256 y);
bool u256_sgt(u256 x, u256 y);
bool u256_eq(u256 x, u256 y);
bool u256_is_zero(u256 x);

// bitwise
void u256_and(u256 res, u256 x, u256 y);
void u256_or(u256 res, u256 x, u256 y);
void u256_xor(u256 res, u256 x, u256 y);
void u256_not(u256 res, u256 x);
void u256_byte(u256 res, u256 x, u256 i);
void u256_shl(u256 res, u256 x, u256 shift);
void u256_shr(u256 res, u256 x, u256 shift);
void u256_sar(u256 res, u256 x, u256 shift);
