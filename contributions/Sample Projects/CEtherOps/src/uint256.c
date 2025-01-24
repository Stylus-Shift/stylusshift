/*
* EVM Opcodes
* */
#include <uint256.h>

/*
    arithmetic operations
*/
void u256_add(u256 res, u256 x, u256 y) {
    u64 carry;
    carry = add64(&res[0], x[0], y[0], 0);
    carry = add64(&res[1], x[1], y[1], carry);
    carry = add64(&res[2], x[2], y[2], carry);
            add64(&res[3], x[3], y[3], carry);
}

void u256_mul(u256 res, u256 x, u256 y) {
    u64 carry0, carry1, carry2;
    u64 res1, res2;
    u64 x0 = x[0];
    u64 x1 = x[1];
    u64 x2 = x[2];
    u64 x3 = x[3];
    u64 y0 = y[0];
    u64 y1 = y[1];
    u64 y2 = y[2];
    u64 y3 = y[3];

    mul64(&carry0, &res[0], x0, y0);
    umul_hop(&carry0, &res1, carry0, x1, y0);
    umul_hop(&carry0, &res2, carry0, x2, y0);

    umul_hop(&carry1, &res[1], res1, x0, y1);
    umul_step(&carry1, &res2, res2, x1, y1, carry1);

    umul_hop(&carry2, &res[2], res2, x0, y2);

    res[3] = x3*y0 + x2*y1 + x0*y3 + x1*y2 + carry0 + carry1 + carry2;
}

void u256_sub(u256 res, u256 x, u256 y) {
    u64 carry;
    carry = sub64(&res[0], x[0], y[0], 0);
    carry = sub64(&res[1], x[1], y[1], carry);
    carry = sub64(&res[2], x[2], y[2], carry);
            sub64(&res[3], x[3], y[3], carry);
}

void u256_neg(u256 res, u256 x) {
    u256 zero;
    clear_words(&zero[0], 4);
    u256_sub(res, zero, x);
}

void u256_div(u256 res, u256 x, u256 y) {
    clear_words(&res[0], 4);
    if (is_zero(y) || greater_than(y, x)) {
        return;
    }
    if (eq(x, y)) {
        res[0] = 1;
        return;
    }

    if (is_uint64(x)) {
        res[0] = x[0] / y[0];
        return;
    }

    u256 _;
    udivrem(res, x, 4, y, _);
}

void u256_sdiv(u256 res, u256 n, u256 d) {
    int n_sign = sign(n);
    int d_sign = sign(d);
    if (n_sign > 0) {
        if (d_sign > 0) {
            u256_div(res, n, d);
            return;
        } else {
            u256 neg_d;
            u256_neg(neg_d, d);

            u256_div(res, n, neg_d);
            u256_neg(res, res);
            return;
        }
    }

    if (d_sign < 0) {
        u256 neg_n;
        u256_neg(neg_n, n);

        u256 neg_d;
        u256_neg(neg_d, d);

        u256_div(res, neg_n, neg_d);
        return;
    }

    u256 neg_n;
    u256_neg(neg_n, n);

    u256_div(res, neg_n, d);
    u256_neg(res, res);
}

void u256_mod(u256 res, u256 x, u256 y) {
    clear_words(&res[0], 4);
    if (is_zero(y) || eq(x, y)) {
        return;
    }

    if (less_than(x, y)) {
        copy_words(&res[0], &x[0], 4);
        return;
    }

    if (is_uint64(x)) {
        res[0] = x[0] % y[0];
        return;
    }

    u256 _;
    udivrem(_, x, 4, y, res);
}

void u256_smod(u256 res, u256 x, u256 m) {
    int m_sign = sign(m);
    int x_sign = sign(x);

    u256 tmp_x;
    copy_words(&tmp_x[0], &x[0], 4);
    u256 tmp_m;
    copy_words(&tmp_m[0], &m[0], 4);

    if (x_sign == -1) {
        u256_neg(tmp_x, tmp_x);
    }
    if (m_sign == -1) {
        u256_neg(tmp_m, tmp_m);
    }
    u256_mod(res, tmp_x, tmp_m);
    if (x_sign == -1) {
        u256_neg(res, res);
    }
}

void u256_add_mod(u256 res, u256 x, u256 y, u256 m) {
    if ((m[3] != 0) && (x[3] <= m[3]) && (y[3] <= m[3])) {
        u64 gte_c1 = 0;
        u64 gte_c2 = 0;
        u256 tmp_x, tmp_y, result;
        clear_words(&tmp_x[0], 4);
        clear_words(&tmp_y[0], 4);
        clear_words(&result[0], 4);

        gte_c1 = sub64(&tmp_x[0], x[0], m[0], gte_c1);
        gte_c1 = sub64(&tmp_x[1], x[1], m[1], gte_c1);
        gte_c1 = sub64(&tmp_x[2], x[2], m[2], gte_c1);
        gte_c1 = sub64(&tmp_x[3], x[3], m[3], gte_c1);

        gte_c2 = sub64(&tmp_y[0], y[0], m[0], gte_c2);
        gte_c2 = sub64(&tmp_y[1], y[1], m[1], gte_c2);
        gte_c2 = sub64(&tmp_y[2], y[2], m[2], gte_c2);
        gte_c2 = sub64(&tmp_y[3], y[3], m[3], gte_c2);

        if (gte_c1 == 0) {
            x = tmp_x;
        }
        if (gte_c2 == 0) {
            y = tmp_y;
        }
        u64 c1 = 0;
        u64 c2 = 0;
        u256 tmp;
        clear_words(&tmp[0], 4);

        c1 = add64(&result[0], x[0], y[0], c1);
        c1 = add64(&result[1], x[1], y[1], c1);
        c1 = add64(&result[2], x[2], y[2], c1);
        c1 = add64(&result[3], x[3], y[3], c1);

        c2 = sub64(&tmp[0], result[0], m[0], c2);
        c2 = sub64(&tmp[1], result[1], m[1], c2);
        c2 = sub64(&tmp[2], result[2], m[2], c2);
        c2 = sub64(&tmp[3], result[3], m[3], c2);

        if (c1 == 0 && c2 != 0) {
            copy_words(&res[0], &result[0], 4);
            return;
        }
        copy_words(&res[0], &tmp[0], 4);
        return;
    }

    if (is_zero(m)) {
        clear_words(&res[0], 4);
        return;
    }

    u64 sum[5];
    clear_words(&sum[0], 5);
    bool overflow = add_overflow(sum, x, y);
    if (overflow) {
        sum[4] = 1;
        u64 quot[5]; clear_words(&quot[0], 5);
        udivrem(quot, sum, 5, m, res);
        return;
    }
    u256_mod(res, sum, m);
}

void u256_mul_mod(u256 res, u256 x, u256 y, u256 m) {
    if (is_zero(x) || is_zero(y) || is_zero(m)) {
        clear_words(&res[0], 4);
        return;
    }
    u512 p;
    umul(p, x, y);

    if (m[3] != 0) {
        u320 mu;
        reciprocal(mu, m);
        reduce4(res, p, m, mu);
        return;
    }

    u256 ph, pl;
    copy_words(&pl[0], &p[0], 4);
    copy_words(&ph[0], &p[4], 4);

    if (is_zero(ph)) {
        u256_mod(res, pl, m);
        return;
    }

    u512 _;
    udivrem(_, p, 8, m, res);
}

void u256_exp(u256 res, u256 base, u256 exponent) {
    clear_words(&res[0], 4);
    res[0] = 1;
    u256 multiplier;
    copy_words(&multiplier[0], &base[0], 4);
    int exp_bit_len = bit_len(exponent);

    int cur_bit = 0;
    u64 word = exponent[0];
    for (;cur_bit < exp_bit_len && cur_bit < 64; cur_bit++) {
        if ((word&1) == 1) {
            u256_mul(res, res, multiplier);
        }
        squared(multiplier);
        word >>= 1;
    }

    word = exponent[1];
    for (;cur_bit < exp_bit_len && cur_bit < 128; cur_bit++) {
        if ((word&1) == 1) {
            u256_mul(res, res, multiplier);
        }
        squared(multiplier);
        word >>= 1;
    }

    word = exponent[2];
    for (;cur_bit < exp_bit_len && cur_bit < 192; cur_bit++) {
        if ((word&1) == 1) {
            u256_mul(res, res, multiplier);
        }
        squared(multiplier);
        word >>= 1;
    }

    word = exponent[3];
    for (;cur_bit < exp_bit_len && cur_bit < 256; cur_bit++) {
        if ((word&1) == 1) {
            u256_mul(res, res, multiplier);
        }
        squared(multiplier);
        word >>= 1;
    }
}

void u256_sign_extend(u256 res, u256 x, u256 b) {
    copy_words(&res[0], &x[0], 4);
    if (greater_than_uint64(b, 30)) {
        return;
    }

    u64 e = b[0];
    u64 sign_word_index = e >> 3;
    u64 sign_byte_index = e & 7;
    u64 sign_word = res[sign_word_index];
    u64 sign_byte_offset = sign_byte_index << 3;
    u64 sign_byte = sign_byte_offset >= 64 ? 0 : sign_word >> sign_byte_offset;
    u64 sext_byte = (u64)(int64_t)(int8_t)sign_byte;

    u64 sext, sign_mask;
    if (sign_byte_offset >= 64) {
        sext = 0;
        sign_mask = 0;
    } else {
        sext = sext_byte << sign_byte_offset;
        sign_mask = MAX_U64 << sign_byte_offset;
    }
    u64 value = sign_word & ~sign_mask;

    res[sign_word_index] = sext | value;
    u64 sign_ex = (u64)(((int64_t)sext_byte) >> 8);

    switch (sign_word_index) {
        case 2:
            res[3] = sign_ex;
            return;
        case 1:
            res[3] = sign_ex;
            res[2] = sign_ex;
            return;
        case 0:
            res[3] = sign_ex;
            res[2] = sign_ex;
            res[1] = sign_ex;
    }
}

/*
    comparison operations
*/
bool u256_lt(u256 x, u256 y) {
    return less_than(x, y);
}

bool u256_gt(u256 x, u256 y) {
    return greater_than(x, y);
}

bool u256_slt(u256 x, u256 y) {
    int x_sign = sign(x);
    int y_sign = sign(y);

    if (x_sign >= 0 && y_sign < 0) {
        return false;
    } else if (x_sign < 0 && y_sign >= 0) {
        return true;
    }

    return less_than(x, y);
}

bool u256_sgt(u256 x, u256 y) {
    int x_sign = sign(x);
    int y_sign = sign(y);

    if (x_sign >= 0 && y_sign < 0) {
        return true;
    } else if (x_sign < 0 && y_sign >= 0) {
        return false;
    }

    return greater_than(x, y);
}

bool u256_eq(u256 x, u256 y) {
    return eq(x, y);
}

bool u256_is_zero(u256 x) {
    return is_zero(x);
}

/*
    bitwise operations
*/
void u256_and(u256 res, u256 x, u256 y) {
    res[0] = x[0] & y[0];
    res[1] = x[1] & y[1];
    res[2] = x[2] & y[2];
    res[3] = x[3] & y[3];
}

void u256_or(u256 res, u256 x, u256 y) {
    res[0] = x[0] | y[0];
    res[1] = x[1] | y[1];
    res[2] = x[2] | y[2];
    res[3] = x[3] | y[3];
}

void u256_xor(u256 res, u256 x, u256 y) {
    res[0] = x[0] ^ y[0];
    res[1] = x[1] ^ y[1];
    res[2] = x[2] ^ y[2];
    res[3] = x[3] ^ y[3];
}

void u256_not(u256 res, u256 x) {
    res[0] = ~x[0];
    res[1] = ~x[1];
    res[2] = ~x[2];
    res[3] = ~x[3];
}

void u256_byte(u256 res, u256 x, u256 i) {
    clear_words(&res[0], 4);
    u64 index = i[0];
    if (index >= 32 || ((i[1] | i[2] | i[3]) != 0)) {
        return;
    }
    u64 number = x[4-1-index/8];
    u64 shift = 56 - ((index & 0x7) << 3); // 0 <= shift <= 56
    res[0] = shift > 0 ? (number >> shift) & 0xff : number & 0xff;
}

void u256_shl(u256 res, u256 x, u256 shift) {
    if (less_than_uint64(shift, 256)) {
        lsh(res, x, shift[0]);
        return;
    }
    clear_words(&res[0], 4);
}

void u256_shr(u256 res, u256 x, u256 shift) {
    if (less_than_uint64(shift, 256)) {
        rsh(res, x, shift[0]);
        return;
    }
    clear_words(&res[0], 4);
}

void u256_sar(u256 res, u256 value, u256 shift) {
    if (greater_than_uint64(shift, 256)) {
        if (sign(value) >= 0) {
            clear_words(&res[0], 4);
        } else {
            set_all_one(&res[0]);
        }
        return;
    }
    srsh(res, value, shift[0]);
}
