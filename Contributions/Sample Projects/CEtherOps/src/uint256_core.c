/*
* Core logic for u256 library
* */
#include <uint256_core.h>

/*
    Helper functions
*/
extern void revert();

void clear_words(u64 *dest, int num_words) {
    __builtin_memset(dest, 0, num_words * sizeof(u64));
}

void copy_words(u64 *dest, u64 *src, int num_words) {
    __builtin_memcpy(dest, src, num_words * sizeof(u64));
}

static const unsigned char len8tab[] = {
    0x00, 0x01, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08
};

u64 len64(u64 x) {
    u64 n = 0;
    if (x >= (1ULL<<32ULL)) {
        x >>= 32;
        n = 32ULL;
    }
    if (x >= (1ULL<<16ULL)) {
        x >>= 16;
        n += 16ULL;
    }
    if (x >= (1ULL<<8ULL)) {
        x >>= 8;
        n += 8ULL;
    }
    return n + len8tab[x];
}

u64 leading_zeros64(u64 x) {
    return 64ULL - len64(x);
}

u64 add64(u64 *res, const u64 x, const u64 y, const u64 carry) {
    *res = x + y + carry;
    return ((x & y) | ((x | y) &~ *res)) >> 63;
}

bool add_overflow(u256 res, const u256 x, const u256 y) {
    u64 carry;
    carry = add64(&res[0], x[0], y[0], 0);
    carry = add64(&res[1], x[1], y[1], carry);
    carry = add64(&res[2], x[2], y[2], carry);
    carry = add64(&res[3], x[3], y[3], carry);
    return carry != 0;
}

void mul64(u64 *hi, u64 *lo, const u64 x, const u64 y) {
    const u64 mask32 = (1ULL<<32) - 1;
    u64 x0, x1, y0, y1, w0, w1, w2, t;
    x0 = x & mask32;
    x1 = x >> 32;
    y0 = y & mask32;
    y1 = y >> 32;
    w0 = x0 * y0;
    t = x1*y0 + (w0>>32);
    w1 = t & mask32;
    w2 = t >> 32;
    w1 += x0*y1;
    *hi = x1*y1 + w2 + (w1>>32);
    *lo = x*y;
}

u64 sub64(u64 *res, const u64 x, const u64 y, const u64 borrow) {
    *res = x - y - borrow;
    return ((~x & y) | (~(x ^ y) & *res)) >> 63;
}

void div32(u32 *quo, u32 *rem, const u32 hi, const u32 lo, const u32 y) {
    if (y != 0 && y <= hi) {
        return;
    }
    u64 z = ((u64)hi)<<32 | ((u64)lo);
    *quo = (u32)(z/((u64)y));
    *rem = (u32)(z%((u64)y));
}

void div64(u64 *quo, u64 *rem, u64 hi, u64 lo, u64 y) {
    if (y == 0) {
        revert();
    }

    if (y <= hi) {
        revert();
    }

    const u64 two32 = (1ULL << 32);
    const u64 mask32 = two32 - 1;

    u64 s = leading_zeros64(y);

    u64 yn1, yn0, un32, un10, un1, un0, q1, rhat;
    if (s == 64) {
        y = 0;
        un32 = lo;
        un10 = 0;
    } else if (s==0) {
        // y = y;
        un32 = hi;
        un10 = lo;
    } else {
        y <<= s;
        un32 = (hi<<s) | lo>>(64-s);
        un10 = lo << s;
    }
    /* y <<= s; */
    yn1 = y >> 32;
    yn0 = y & mask32;
    /* un32 = hi<<s | lo>>(64-s); */
    /* un10 = lo << s; */
    un1 = un10 >> 32;
    un0 = un10 & mask32;
    q1 = un32 / yn1;
    rhat = un32 - q1*yn1;

    while (q1 >= two32 || q1*yn0 > two32*rhat + un1) {
        q1--;
        rhat += yn1;
        if (rhat >= two32) {
            break;
        }
    }

    u64 un21, q0;
    un21 = un32*two32 + un1 - q1*y;
    q0 = un21 / yn1;
    rhat = un21 - q0*yn1;

    while (q0 >= two32 || q0*yn0 > two32*rhat+un0) {
        q0--;
        rhat += yn1;
        if (rhat >= two32) {
            break;
        }
    }

    *quo = q1*two32 + q0;
    *rem = s == 64 ? 0 : (un21*two32 + un0 - q0*y) >> s;
    /* *rem = (un21*two32 + un0 - q0*y) >> s; */
}

bool is_uint64(u256 x) {
    return (x[1] | x[2] | x[3]) == 0;
}

bool less_than(u256 x, u256 y) {
    u64 carry, _;
    carry = sub64(&_, x[0], y[0], 0);
    carry = sub64(&_, x[1], y[1], carry);
    carry = sub64(&_, x[2], y[2], carry);
    carry = sub64(&_, x[3], y[3], carry);
    return carry != 0;
}

bool greater_than(u256 x, u256 y) {
    return less_than(y, x);
}

bool signed_less_than(u256 x, u256 y) {
    u64 carry, _;
    carry = sub64(&_, x[0], y[0], 0);
    carry = sub64(&_, x[1], y[1], carry);
    carry = sub64(&_, x[2], y[2], carry);
    carry = sub64(&_, x[3], y[3], carry);
    return carry != 0;
}

bool signed_greater_than(u256 x, u256 y) {
    u64 carry, _;
    carry = sub64(&_, x[0], y[0], 0);
    carry = sub64(&_, x[1], y[1], carry);
    carry = sub64(&_, x[2], y[2], carry);
    carry = sub64(&_, x[3], y[3], carry);
    return carry != 0;
}

bool eq(u256 x, u256 y) {
    return (x[0]==y[0]) && (x[1]==y[1]) && (x[2]==y[2]) && (x[3]==y[3]);
}

bool is_zero(u256 x) {
    return (x[0] | x[1] | x[2] | x[3]) == 0;
}


bool uint64_with_overflow(u64 *n, u256 x) {
    *n = x[0];
    return (x[1] | x[2] | x[3]) != 0;
}

void set_all_one(u256 x) {
    x[0] = 0xffffffffffffffffULL;
    x[1] = 0xffffffffffffffffULL;
    x[2] = 0xffffffffffffffffULL;
    x[3] = 0xffffffffffffffffULL;
}

int sign(u256 x) {
    if (is_zero(x)) {
        return 0;
    }
    if (x[3] < 0x8000000000000000ULL) {
        return 1;
    }
    return -1;
}

bool less_than_uint64(u256 x, u64 n) {
    return (x[0] < n) && ((x[1] | x[2] | x[3]) == 0);
}

bool greater_than_uint64(u256 x, u64 n) {
    return (x[0] > n) || ((x[1] | x[2] | x[3]) != 0);
}

int bit_len(u256 x) {
    if (x[3] != 0) {
        return 192 + len64(x[3]);
    }
    if (x[2] != 0) {
        return 128 + len64(x[2]);
    }
    if (x[1] != 0) {
        return 64 + len64(x[1]);
    }
    return len64(x[0]);
}

u64 reciprocal2by1(const u64 d) {
    u64 recip, _;
    div64(&recip, &_, ~d, ~0ULL, d);
    return recip;
}

void udivrem2by1(u64 *quot, u64 *rem, const u64 uh, const u64 ul, const u64 d,
                 const u64 recip) {
    u64 qh, ql;
    mul64(&qh, &ql, recip, uh);
    u64 carry = add64(&ql, ql, ul, 0);
    add64(&qh, qh, uh, carry);
    qh++;

    u64 r = ul - qh*d;

    if (r > ql) {
        qh--;
        r += d;
    }

    if (r >= d) {
        qh++;
        r -= d;
    }

    *quot = qh;
    *rem = r;
}

u64 udivrem_by1(u64 * quot, u64 * u, const int len_u, const u64 d) {
    u64 recip = reciprocal2by1(d);
    u64 rem = u[len_u - 1];
    for (int j = len_u - 2; j >= 0; j--) {
        udivrem2by1(&quot[j], &rem, rem, u[j], d, recip);
    }
    return rem;
}

u64 add_to(u64 *x, u64 *y, const int len_y) {
    u64 carry = 0;
    for (int i = 0; i < len_y; i++) {
        carry = add64(&x[i], x[i], y[i], carry);
    }
    return carry;
}

u64 sub_mul_to(u64 *x, u64 *y, const int len_y, const u64 multiplier) {
    u64 borrow = 0;
    for (int i = 0; i < len_y; i++) {
        u64 carry1, s;
        carry1 = sub64(&s, x[i], borrow, 0);
        u64 ph, pl;
        mul64(&ph, &pl, y[i], multiplier);
        u64 carry2, t;
        carry2 = sub64(&t, s, pl, 0);
        x[i] = t;
        borrow = ph + carry1 + carry2;
    }
    return borrow;
}

void udivrem_knuth(u64 *quot, u64 *u, int len_u, u64 *d, int len_d) {
    u64 dh = d[len_d-1];
    u64 dl = d[len_d-2];
    u64 recip = reciprocal2by1(dh);

    for (int j = len_u - len_d - 1; j >= 0; j--) {
        u64 u2 = u[j+len_d];
        u64 u1 = u[j+len_d-1];
        u64 u0 = u[j+len_d-2];

        u64 qhat, rhat;
        if (u2 >= dh) {
            qhat = ~0ULL;
        } else {
            udivrem2by1(&qhat, &rhat, u2, u1, dh, recip);
            u64 ph, pl;
            mul64(&ph, &pl, qhat, dl);
            if (ph > rhat || (ph == rhat && pl > u0)) {
                qhat--;
            }
        }
        u64 borrow = sub_mul_to(&u[j], d, len_d, qhat);
        u[j+len_d] = u2 - borrow;
        if (u2 < borrow) {
            qhat--;
            u[j+len_d] += add_to(&u[j], d, len_d);
        }

        quot[j] = qhat;
    }
}

void udivrem(u64 *quot, u64 *u, const int len_u, const u256 d, u256 rem) {
    int d_len = 0;
    for (int i = 3; i >= 0; i--) {
        if (d[i] != 0) {
            d_len = i + 1;
            break;
        }
    }
    u64 shift = leading_zeros64(d[d_len-1]);

    u256 dn;
    clear_words(&dn[0], 4);
    for (int i = d_len - 1; i > 0; i--) {
        if (shift == 64) {
            dn[i] = d[i-1];
        } else if (shift == 0) {
            dn[i] = d[i];
        } else {
            dn[i] = d[i] << shift | d[i-1] >> (64-shift);
        }
        /* dn[i] = d[i] << shift | d[i-1] >> (64-shift); */
    }
    dn[0] = shift == 64 ? 0 : d[0] << shift;

    int u_len = 0;
    for (int i = len_u - 1; i >= 0; i--) {
        if (u[i] != 0) {
            u_len = i + 1;
            break;
        }
    }

    if (u_len < d_len) {
        if (rem != NULL) {
            clear_words(&rem[0], 4);
            copy_words(&rem[0], &u[0], len_u);
        }
        return;
    }

    u64 un[9];
    clear_words(&un[0], 9);
    /* un[u_len] = u[u_len-1] >> (64 - shift); */
    for (int i = u_len - 1; i > 0; i--) {
        if (shift == 64) {
            un[i] = u[i-1];
        } if (shift == 0) {
            un[i] = u[i];
        } else {
            un[i] = u[i] << shift | u[i-1] >> (64-shift);

        }
        /* un[i] = u[i] << shift | u[i-1] >> (64-shift); */
    }
    /* un[0] = u[0] << shift; */
    if (shift == 64) {
        un[u_len] = u[u_len-1];
        un[0] = 0;
    } else if (shift == 0) {
        un[u_len] = 0;
        un[0] = u[0];
    } else {
        un[u_len] = u[u_len-1] >> (64-shift);
        un[0] = u[0] << shift;
    }

    if (d_len == 1) {
        u64 r = udivrem_by1(quot, un, u_len+1, dn[0]);
        if (rem != NULL) {
            rem[0] = shift == 64 ? 0 : r >> shift;
            /* rem[0] = r >> shift; */
            clear_words(&rem[1], 3);
        }
        return;
    }

    udivrem_knuth(quot, un, u_len+1, dn, d_len);

    if (rem != NULL) {
        clear_words(&rem[0], 4);
        for (int i = 0; i < d_len-1; i++) {
            if (shift == 64) {
                rem[i] = un[i+1];
            } else if (shift == 0) {
                rem[i] = un[i];
            } else {
                rem[i] = un[i] >> shift | un[i+1] << (64-shift);
            }
            /* rem[i] = un[i] >> shift | un[i+1] << (64 - shift); */
        }
        rem[d_len-1] = shift == 64 ? 0 : un[d_len-1] >> shift;
        /* rem[d_len-1] = un[d_len-1] >> shift; */
    }
}

void umul_hop(u64 *hi, u64 *lo, const u64 z, const u64 x, const u64 y) {
    mul64(hi, lo, x, y);
    u64 carry = add64(lo, *lo, z, 0);
    add64(hi, *hi, 0, carry);
}

void umul_step(u64 *hi, u64 *lo, const u64 z, const u64 x, const u64 y,
               u64 carry) {
    mul64(hi, lo, x, y);
    carry = add64(lo, *lo, carry, 0);
    add64(hi, *hi, 0, carry);
    carry = add64(lo, *lo, z, 0);
    add64(hi, *hi, 0, carry);
}

void umul(u512 res, const u256 x, const u256 y) {
    u64 carry, carry4, carry5, carry6;
    u64 res1, res2, res3, res4, res5;

    mul64(&carry, &res[0], x[0], y[0]);
    umul_hop(&carry, &res1, carry, x[1], y[0]);
    umul_hop(&carry, &res2, carry, x[2], y[0]);
    umul_hop(&carry4, &res3, carry, x[3], y[0]);

    umul_hop(&carry, &res[1], res1, x[0], y[1]);
    umul_step(&carry, &res2, res2, x[1], y[1], carry);
    umul_step(&carry, &res3, res3, x[2], y[1], carry);
    umul_step(&carry5, &res4, carry4, x[3], y[1], carry);

    umul_hop(&carry, &res[2], res2, x[0], y[2]);
    umul_step(&carry, &res3, res3, x[1], y[2], carry);
    umul_step(&carry, &res4, res4, x[2], y[2], carry);
    umul_step(&carry6, &res5, carry5, x[3], y[2], carry);

    umul_hop(&carry, &res[3], res3, x[0], y[3]);
    umul_step(&carry, &res[4], res4, x[1], y[3], carry);
    umul_step(&carry, &res[5], res5, x[2], y[3], carry);
    umul_step(&res[7], &res[6], carry6, x[3], y[3], carry);
}

void squared(u256 x) {
    u64 carry0, carry1, carry2;
    u64 res0, res1, res2, res3;

    mul64(&carry0, &res0, x[0], x[0]);
    umul_hop(&carry0, &res1, carry0, x[0], x[1]);
    umul_hop(&carry0, &res2, carry0, x[0], x[2]);

    umul_hop(&carry1, &res1, res1, x[0], x[1]);
    umul_step(&carry1, &res2, res2, x[1], x[1], carry1);

    umul_hop(&carry2, &res2, res2, x[0], x[2]);

    res3 = 2*(x[0]*x[3]+x[1]*x[2]) + carry0 + carry1 + carry2;

    x[0] = res0;
    x[1] = res1;
    x[2] = res2;
    x[3] = res3;
}

void lsh192(u256 res, u256 x) {
    res[3] = x[0];
    res[2] = 0;
    res[1] = 0;
    res[0] = 0;
}

void lsh128(u256 res, u256 x) {
    res[3] = x[1];
    res[2] = x[0];
    res[1] = 0;
    res[0] = 0;
}

void lsh64(u256 res, u256 x) {
    res[3] = x[2];
    res[2] = x[1];
    res[1] = x[0];
    res[0] = 0;
}

void rsh192(u256 res, u256 x) {
    res[0] = x[3];
    res[1] = 0;
    res[2] = 0;
    res[3] = 0;
}

void rsh128(u256 res, u256 x) {
    res[0] = x[2];
    res[1] = x[3];
    res[2] = 0;
    res[3] = 0;
}

void rsh64(u256 res, u256 x) {
    res[0] = x[1];
    res[1] = x[2];
    res[2] = x[3];
    res[3] = 0;
}

void srsh192(u256 res, u256 x) {
    res[0] = x[3];
    res[1] = 0xffffffffffffffffULL;
    res[2] = 0xffffffffffffffffULL;
    res[3] = 0xffffffffffffffffULL;
}

void srsh128(u256 res, u256 x) {
    res[0] = x[2];
    res[1] = x[3];
    res[2] = 0xffffffffffffffffULL;
    res[3] = 0xffffffffffffffffULL;
}

void srsh64(u256 res, u256 x) {
    res[0] = x[1];
    res[1] = x[2];
    res[2] = x[3];
    res[3] = 0xffffffffffffffffULL;
}

void lsh(u256 res, u256 x, u64 n) {
    if (n == 0) {
        copy_words(&res[0], &x[0], 4);
        return;
    } else if (n >= 192) {
        lsh192(res, x);
        n -= 192;
        if (n >= 64) {
            res[3] = 0;
        } else if (n > 0) {
            res[3] <<= n;
        }
        return;
    } else if (n >= 128) {
        lsh128(res, x);
        n -= 128;
        if (n > 0) {
            res[3] = (res[3] << n) | (res[2] >> (64-n));
            res[2] <<= n;
        }
        return;
    } else if (n >= 64) {
        lsh64(res, x);
        n -= 64;
        if (n > 0) {
            res[3] = (res[3] << n) | (res[2] >> (64-n));
            res[2] = (res[2] << n) | (res[1] >> (64-n));
            res[1] <<= n;
        }
        return;
    } else {
        copy_words(&res[0], &x[0], 4);
        if (n > 0) {
            res[3] = (res[3] << n) | (res[2] >> (64-n));
            res[2] = (res[2] << n) | (res[1] >> (64-n));
            res[1] = (res[1] << n) | (res[0] >> (64-n));
            res[0] <<= n;
        }
        return;
    }
}

void rsh(u256 res, u256 x, u64 n) {
    if (n == 0) {
        copy_words(&res[0], &x[0], 4);
        return;
    } else if (n >= 192) {
        rsh192(res, x);
        n -= 192;
        if (n >= 64) {
            res[0] = 0;
        } else if (n > 0) {
            res[0] >>= n;
        }
        return;
    } else if (n >= 128) {
        rsh128(res, x);
        n -= 128;
        if (n > 0) {
            res[0] = (res[0] >> n) | (res[1] << (64-n));
            res[1] >>= n;
        }
        return;
    } else if (n >= 64) {
        rsh64(res, x);
        n -= 64;
        if (n > 0) {
            res[0] = (res[0] >> n) | (res[1] << (64-n));
            res[1] = (res[1] >> n) | (res[2] << (64-n));
            res[2] >>= n;
        }
        return;
    } else {
        copy_words(&res[0], &x[0], 4);
        if (n > 0) {
            res[0] = (res[0] >> n) | (res[1] << (64-n));
            res[1] = (res[1] >> n) | (res[2] << (64-n));
            res[2] = (res[2] >> n) | (res[3] << (64-n));
            res[3] >>= n;
        }
        return;
    }
}

bool is_bit_set(u256 x, u64 n) {
    return (x[n/64] & (1ULL << (n % 64))) != 0;
}

void srsh(u256 res, u256 x, u64 n) {
    if (!is_bit_set(x, 255)) {
        rsh(res, x, n);
        return;
    }
    u64 shift = (64 - (n%64));
    u64 a = (shift == 64) ? 0 : (0xffffffffffffffffULL << shift);

    if (n == 0) {
        copy_words(&res[0], &x[0], 4);
        return;
    } else if (n >= 256) {
        set_all_one(res);
        return;
    } else if (n >= 192) {
        srsh192(res, x);
        n -= 192;
        if (n >= 64) {
            res[0] = 0;
        } else if (n > 0) {
            res[0] = (res[0] >> n) | a;
        } else {
            res[0] |= a;
        }
        return;
    } else if (n >= 128) {
        srsh128(res, x);
        n -= 128;
        if (n > 0) {
            res[0] = (res[0] >> n) | (res[1] << (64-n));
            res[1] = (res[1] >> n) | a;
        } else {
            res[1] |= a;
        }
        return;
    } else if (n >= 64) {
        srsh64(res, x);
        n -= 64;
        if (n > 0) {
            res[0] = (res[0] >> n) | (res[1] << (64-n));
            res[1] = (res[1] >> n) | (res[2] << (64-n));
            res[2] = (res[2] >> n) | a;
        } else {
            res[2] |= a;
        }
        return;
    } else {
        copy_words(&res[0], &x[0], 4);
        if (n > 0) {
            res[0] = (res[0] >> n) | (res[1] << (64-n));
            res[1] = (res[1] >> n) | (res[2] << (64-n));
            res[2] = (res[2] >> n) | (res[3] << (64-n));
            res[3] = (res[3] >> n) | a;
        } else {
            res[3] |= a;
        }
    }
}

void reciprocal(u320 mu, u256 m) {
    if (m[3] == 0) {
        return;
    }

    u64 s = leading_zeros64(m[3]);
    int p = 255 - (int)s;

    if ((m[0] | m[1] | m[2] | (m[3] & (m[3]-1))) == 0) {
        u64 shift = (u64)(p & 63);
        mu[4] = (shift > 0) ? (~0ULL >> shift) : ~0ULL;
        mu[3] = ~0ULL;
        mu[2] = ~0ULL;
        mu[1] = ~0ULL;
        mu[0] = ~0ULL;
        return;
    }

    u256 y;
    clear_words(y, 4);
    lsh(y, m, s);

    u32 yh = (u32)((y[3] >> 32));
    u32 r0;
    if (yh == 0x80000000) {
        r0 = 0xffffffff;
    } else {
        u32 _;
        div32(&r0, &_, 0x80000000, 0, yh);
    }

    u64 _;
    u64 t1 = (u64)r0;
    t1 *= t1;
    mul64(&t1, &_, t1, y[3]);

    u64 r1 = ((u64)r0) << 32;
    r1 -= t1;
    r1 *= 2;

    if ((r1 | (y[3]<<1)) == 0) {
        r1 = ~0ULL;
    }

    u64 a2h, a2l;
    mul64(&a2h, &a2l, r1, r1);

    u64 b2h, c2h, c2l, d2h, d2l, e2h, e2l;
    mul64(&b2h, &_, a2l, y[2]);
    mul64(&c2h, &c2l, a2l, y[3]);
    mul64(&d2h, &d2l, a2h, y[2]);
    mul64(&e2h, &e2l, a2h, y[3]);

    u64 c;
    c = add64(&b2h, b2h, c2l, 0);
    c = add64(&e2l, e2l, c2h, c);
        add64(&e2h, e2h,   0, c);

    c = add64(  &_, b2h, d2l, 0);
    c = add64(&e2l, e2l, d2h, c);
        add64(&e2h, e2h,   0, c);

    u64 t2h, t2l, b;
    b = sub64(&t2l,  0, e2l, 0);
        sub64(&t2h, r1, e2h, b);

    u64 r2h, r2l;
    c = add64(&r2l, t2l, t2l, 0);
        add64(&r2h, t2h, t2h, c);

    if ((r2h | r2l | (y[3] << 1)) == 0) {
        r2h = ~0ULL;
        r2l = ~0ULL;
    }

    u64 a3h, a3l, b3h, b3l, c3h, c3l;
    mul64(&a3h, &a3l, r2l, r2l);
    mul64(&b3h, &b3l, r2l, r2h);
    mul64(&c3h, &c3l, r2h, r2h);

    c = add64(&a3h, a3h, b3l, 0);
    c = add64(&c3l, c3l, b3h, c);
        add64(&c3h, c3h,   0, c);

    c = add64(&a3h, a3h, b3l, 0);
    c = add64(&c3l, c3l, b3h, c);
        add64(&c3h, c3h,   0, c);

    u64 x0 = a3l;
    u64 x1 = a3h;
    u64 x2 = c3l;
    u64 x3 = c3h;

    u64 q0, q1, q2, q3, q4, t0;

    mul64(&q0, &_, x2, y[0]);
    mul64(&q1, &t0, x3, y[0]); c = add64(&q0, q0, t0, 0);     add64(&q1, q1,  0, c);

    mul64(&t1,  &_, x1, y[1]); c = add64(&q0, q0, t1, 0);
    mul64(&q2, &t0, x3, y[1]); c = add64(&q1, q1, t0, c);     add64(&q2, q2,  0, c);

    mul64(&t1, &t0, x2, y[1]); c = add64(&q0, q0, t0, 0); c = add64(&q1, q1, t1, c); add64(&q2, q2, 0, c);

    mul64(&t1, &t0, x1, y[2]); c = add64(&q0, q0, t0, 0); c = add64(&q1, q1, t1, c);
    mul64(&q3, &t0, x3, y[2]); c = add64(&q2, q2, t0, c);     add64(&q3, q3,  0, c);

    mul64(&t1,  &_, x0, y[2]); c = add64(&q0, q0, t1, 0);
    mul64(&t1, &t0, x2, y[2]); c = add64(&q1, q1, t0, c); c = add64(&q2, q2, t1, c); add64(&q3, q3, 0, c);

    mul64(&t1, &t0, x1, y[3]); c = add64(&q1, q1, t0, 0); c = add64(&q2, q2, t1, c);
    mul64(&q4, &t0, x3, y[3]); c = add64(&q3, q3, t0, c);     add64(&q4, q4,  0, c);

    mul64(&t1, &t0, x0, y[3]); c = add64(&q0, q0, t0, 0); c = add64(&q1, q1, t1, c);
    mul64(&t1, &t0, x2, y[3]); c = add64(&q2, q2, t0, c); c = add64(&q3, q3, t1, c); add64(&q4, q4, 0, c);

    u64 t3h, t3m, t3l;
    b = sub64(  &_,   0, q0, 0);
    b = sub64(  &_,   0, q1, b);
    b = sub64(&t3l,   0, q2, b);
    b = sub64(&t3m, r2l, q3, b);
        sub64(&t3h, r2h, q4, b);

    u64 r3h, r3m, r3l;
    c = add64(&r3l, t3l, t3l, 0);
    c = add64(&r3m, t3m, t3m, c);
        add64(&r3h, t3h, t3h, c);

    u64 a4h, a4l, b4h, b4l, c4h, c4l;
    u64 d4h, d4l, e4h, e4l, f4h, f4l;

    mul64(&a4h, &a4l, r3l, r3l);
    mul64(&b4h, &b4l, r3l, r3m);
    mul64(&c4h, &c4l, r3l, r3h);
    mul64(&d4h, &d4l, r3m, r3m);
    mul64(&e4h, &e4l, r3m, r3h);
    mul64(&f4h, &f4l, r3h, r3h);

    c = add64(&b4h, b4h, c4l, 0);
    c = add64(&e4l, e4l, c4h, c);
        add64(&e4h, e4h,   0, c);

    c = add64(&a4h, a4h, b4l, 0);
    c = add64(&d4l, d4l, b4h, c);
    c = add64(&d4h, d4h, e4l, c);
    c = add64(&f4l, f4l, e4h, c);
        add64(&f4h, f4h,   0, c);

    c = add64(&a4h, a4h, b4l, 0);
    c = add64(&d4l, d4l, b4h, c);
    c = add64(&d4h, d4h, e4l, c);
    c = add64(&f4l, f4l, e4h, c);
        add64(&f4h, f4h,   0, c);

    mul64(&x1, &x0, d4h, y[0]);
    mul64(&x3, &x2, f4h, y[0]);
    mul64(&t1, &t0, f4l, y[0]); c = add64(&x1, x1, t0, 0); c = add64(&x2, x2, t1, c);
                                    add64(&x3, x3,  0, c);

    u64 x4;
    mul64(&t1, &t0, d4h, y[1]); c = add64(&x1, x1, t0, 0); c = add64(&x2, x2, t1, c);
    mul64(&x4, &t0, f4h, y[1]); c = add64(&x3, x3, t0, c);     add64(&x4, x4,  0, c);
    mul64(&t1, &t0, d4l, y[1]); c = add64(&x0, x0, t0, 0); c = add64(&x1, x1, t1, c);
    mul64(&t1, &t0, f4l, y[1]); c = add64(&x2, x2, t0, c); c = add64(&x3, x3, t1, c);
                                                               add64(&x4, x4,  0, c);

    u64 x5;
    mul64(&t1, &t0, a4h, y[2]); c = add64(&x0, x0, t0, 0); c = add64(&x1, x1, t1, c);
    mul64(&t1, &t0, d4h, y[2]); c = add64(&x2, x2, t0, c); c = add64(&x3, x3, t1, c);
    mul64(&x5, &t0, f4h, y[2]); c = add64(&x4, x4, t0, c);     add64(&x5, x5,  0, c);
    mul64(&t1, &t0, d4l, y[2]); c = add64(&x1, x1, t0, 0); c = add64(&x2, x2, t1, c);
    mul64(&t1, &t0, f4l, y[2]); c = add64(&x3, x3, t0, c); c = add64(&x4, x4, t1, c);
                                    add64(&x5, x5,  0, c);

    u64 x6;
    mul64(&t1, &t0, a4h, y[3]); c = add64(&x1, x1, t0, 0); c = add64(&x2, x2, t1, c);
    mul64(&t1, &t0, d4h, y[3]); c = add64(&x3, x3, t0, c); c = add64(&x4, x4, t1, c);
    mul64(&x6, &t0, f4h, y[3]); c = add64(&x5, x5, t0, c);     add64(&x6, x6,  0, c);
    mul64(&t1, &t0, a4l, y[3]); c = add64(&x0, x0, t0, 0); c = add64(&x1, x1, t1, c);
    mul64(&t1, &t0, d4l, y[3]); c = add64(&x2, x2, t0, c); c = add64(&x3, x3, t1, c);
    mul64(&t1, &t0, f4l, y[3]); c = add64(&x4, x4, t0, c); c = add64(&x5, x5, t1, c);
                                                               add64(&x6, x6,  0, c);

    u64 r4l, r4k, r4j, r4i, r4h;

    b = sub64(  &_,   0, x0, 0);
    b = sub64(  &_,   0, x1, b);
    b = sub64(&r4l,   0, x2, b);
    b = sub64(&r4k,   0, x3, b);
    b = sub64(&r4j, r3l, x4, b);
    b = sub64(&r4i, r3m, x5, b);
        sub64(&r4h, r3h, x6, b);

    x0 = r4l;
    x1 = r4k;
    x2 = r4j;
    x3 = r4i;
    x4 = r4h;

    u64 q5;
    mul64(&q1, &q0, x0, y[0]);
    mul64(&q3, &q2, x2, y[0]);
    mul64(&q5, &q4, x4, y[0]);

    mul64(&t1, &t0, x1, y[0]); c = add64(&q1, q1, t0, 0); c = add64(&q2, q2, t1, c);
    mul64(&t1, &t0, x3, y[0]); c = add64(&q3, q3, t0, c); c = add64(&q4, q4, t1, c); add64(&q5, q5, 0, c);

    u64 q6;
    mul64(&t1, &t0, x0, y[1]); c = add64(&q1, q1, t0, 0); c = add64(&q2, q2, t1, c);
    mul64(&t1, &t0, x2, y[1]); c = add64(&q3, q3, t0, c); c = add64(&q4, q4, t1, c);
    mul64(&q6, &t0, x4, y[1]); c = add64(&q5, q5, t0, c);     add64(&q6, q6,  0, c);

    mul64(&t1, &t0, x1, y[1]); c = add64(&q2, q2, t0, 0); c = add64(&q3, q3, t1, c);
    mul64(&t1, &t0, x3, y[1]); c = add64(&q4, q4, t0, c); c = add64(&q5, q5, t1, c); add64(&q6, q6, 0, c);

    u64 q7;
    mul64(&t1, &t0, x0, y[2]); c = add64(&q2, q2, t0, 0); c = add64(&q3, q3, t1, c);
    mul64(&t1, &t0, x2, y[2]); c = add64(&q4, q4, t0, c); c = add64(&q5, q5, t1, c);
    mul64(&q7, &t0, x4, y[2]); c = add64(&q6, q6, t0, c);     add64(&q7, q7,  0, c);

    mul64(&t1, &t0, x1, y[2]); c = add64(&q3, q3, t0, 0); c = add64(&q4, q4, t1, c);
    mul64(&t1, &t0, x3, y[2]); c = add64(&q5, q5, t0, c); c = add64(&q6, q6, t1, c); add64(&q7, q7, 0, c);

    u64 q8;
    mul64(&t1, &t0, x0, y[3]); c = add64(&q3, q3, t0, 0); c = add64(&q4, q4, t1, c);
    mul64(&t1, &t0, x2, y[3]); c = add64(&q5, q5, t0, c); c = add64(&q6, q6, t1, c);
    mul64(&q8, &t0, x4, y[3]); c = add64(&q7, q7, t0, c);     add64(&q8, q8,  0, c);

    mul64(&t1, &t0, x1, y[3]); c = add64(&q4, q4, t0, 0); c = add64(&q5, q5, t1, c);
    mul64(&t1, &t0, x3, y[3]); c = add64(&q6, q6, t0, c); c = add64(&q7, q7, t1, c); add64(&q8, q8, 0, c);

    b = sub64(&_, 0, q0, 0);
    b = sub64(&_, 0, q1, b);
    b = sub64(&_, 0, q2, b);
    b = sub64(&_, 0, q3, b);
    b = sub64(&_, 0, q4, b);
    b = sub64(&_, 0, q5, b);
    b = sub64(&_, 0, q6, b);
    b = sub64(&_, 0, q7, b);
    b = sub64(&_, (u64)1 << 62, q8, b);

    u64 t;
    t = sub64(&x0, r4l, 1, 0);
    t = sub64(&x1, r4k, 0, t);
    t = sub64(&x2, r4j, 0, t);
    t = sub64(&x3, r4i, 0, t);
        sub64(&x4, r4h, 0, t);

    if (b != 0) {
        r4h = x4;
        r4i = x3;
        r4j = x2;
        r4k = x1;
        r4l = x0;
    }

    p = (p & 63) - 1; // -1 <= p <= 62
    c = add64(&x0, r4l, r4l, 0);
    c = add64(&x1, r4k, r4k, c);
    c = add64(&x2, r4j, r4j, c);
    c = add64(&x3, r4i, r4i, c);
        add64(&x4, r4h, r4h, c);

    if (p < 0) {
        r4h = x4;
        r4i = x3;
        r4j = x2;
        r4k = x1;
        r4l = x0;
        p = 0;
    }

    u64 r = (u64)p; // 0 <= r <= 62
    u64 l = 64 - r; // 2 <= l <= 64
    if (l == 64) {
        x0 = r4l;
        x1 = r4k;
        x2 = r4j;
        x3 = r4i;
        x4 = r4h;
    } else {
        x0 = (r4l >> r) | (r4k << l);
        x1 = (r4k >> r) | (r4j << l);
        x2 = (r4j >> r) | (r4i << l);
        x3 = (r4i >> r) | (r4h << l);
        x4 = (r4h >> r);
    }

    if (p > 0) {
        r4h = x4;
        r4i = x3;
        r4j = x2;
        r4k = x1;
        r4l = x0;
    }

    mu[0] = r4l;
    mu[1] = r4k;
    mu[2] = r4j;
    mu[3] = r4i;
    mu[4] = r4h;
}

void reduce4(u256 res, u512 x, u256 m, u64 *mu) {
    u64 x0 = x[3];
    u64 x1 = x[4];
    u64 x2 = x[5];
    u64 x3 = x[6];
    u64 x4 = x[7];

    u64 q0, q1, q2, q3, q4, q5, t0, t1, c, _;

    mul64(&q0,  &_, x3, mu[0]);
    mul64(&q1, &t0, x4, mu[0]); c = add64(&q0, q0, t0, 0);     add64(&q1, q1,  0, c);

    mul64(&t1,  &_, x2, mu[1]); c = add64(&q0, q0, t1, 0);
    mul64(&q2, &t0, x4, mu[1]); c = add64(&q1, q1, t0, c);     add64(&q2, q2,  0, c);

    mul64(&t1, &t0, x3, mu[1]); c = add64(&q0, q0, t0, 0); c = add64(&q1, q1, t1, c); add64(&q2, q2, 0, c);

    mul64(&t1, &t0, x2, mu[2]); c = add64(&q0, q0, t0, 0); c = add64(&q1, q1, t1, c);
    mul64(&q3, &t0, x4, mu[2]); c = add64(&q2, q2, t0, c);     add64(&q3, q3,  0, c);

    mul64(&t1,  &_, x1, mu[2]); c = add64(&q0, q0, t1, 0);
    mul64(&t1, &t0, x3, mu[2]); c = add64(&q1, q1, t0, c); c = add64(&q2, q2, t1, c); add64(&q3, q3, 0, c);

    mul64(&t1,  &_, x0, mu[3]); c = add64(&q0, q0, t1, 0);
    mul64(&t1, &t0, x2, mu[3]); c = add64(&q1, q1, t0, c); c = add64(&q2, q2, t1, c);
    mul64(&q4, &t0, x4, mu[3]); c = add64(&q3, q3, t0, c);     add64(&q4, q4,  0, c);

    mul64(&t1, &t0, x1, mu[3]); c = add64(&q0, q0, t0, 0); c = add64(&q1, q1, t1, c);
    mul64(&t1, &t0, x3, mu[3]); c = add64(&q2, q2, t0, c); c = add64(&q3, q3, t1, c); add64(&q4, q4, 0, c);

    mul64(&t1, &t0, x0, mu[4]); c = add64( &_, q0, t0, 0); c = add64(&q1, q1, t1, c);
    mul64(&t1, &t0, x2, mu[4]); c = add64(&q2, q2, t0, c); c = add64(&q3, q3, t1, c);
    mul64(&q5, &t0, x4, mu[4]); c = add64(&q4, q4, t0, c);     add64(&q5, q5,  0, c);

    mul64(&t1, &t0, x1, mu[4]); c = add64(&q1, q1, t0, 0); c = add64(&q2, q2, t1, c);
    mul64(&t1, &t0, x3, mu[4]); c = add64(&q3, q3, t0, c); c = add64(&q4, q4, t1, c); add64(&q5, q5, 0, c);

    q0 = q1;
    q1 = q2;
    q2 = q3;
    q3 = q4;
    q4 = q5;

    x0 = x[0];
    x1 = x[1];
    x2 = x[2];
    x3 = x[3];
    x4 = x[4];

    u64 r0, r1, r2, r3, r4;

    mul64(&r4, &r3, q0, m[3]);
    mul64( &_, &t0, q1, m[3]); add64(&r4, r4, t0, 0);

    mul64(&t1, &r2, q0, m[2]);    c = add64(&r3, r3, t1, 0);
    mul64( &_, &t0, q2, m[2]);      add64(&r4, r4, t0, c);

    mul64(&t1, &t0, q1, m[2]); c = add64(&r3, r3, t0, 0); add64(&r4, r4, t1, c);

    mul64(&t1, &r1, q0, m[1]); c = add64(&r2, r2, t1, 0);
    mul64(&t1, &t0, q2, m[1]); c = add64(&r3, r3, t0, c); add64(&r4, r4, t1, c);

    mul64(&t1, &t0, q1, m[1]); c = add64(&r2, r2, t0, 0); c = add64(&r3, r3, t1, c);
    mul64(&_,  &t0, q3, m[1]);     add64(&r4, r4, t0, c);

    mul64(&t1, &r0, q0, m[0]); c = add64(&r1, r1, t1, 0);
    mul64(&t1, &t0, q2, m[0]); c = add64(&r2, r2, t0, c); c = add64(&r3, r3, t1, c);
    mul64( &_, &t0, q4, m[0]);     add64(&r4, r4, t0, c);

    mul64(&t1, &t0, q1, m[0]); c = add64(&r1, r1, t0, 0); c = add64(&r2, r2, t1, c);
    mul64(&t1, &t0, q3, m[0]); c = add64(&r3, r3, t0, c);     add64(&r4, r4, t1, c);


    u64 b;

    b = sub64(&r0, x0, r0, 0);
    b = sub64(&r1, x1, r1, b);
    b = sub64(&r2, x2, r2, b);
    b = sub64(&r3, x3, r3, b);
    b = sub64(&r4, x4, r4, b);

    if (b != 0) {
        c = add64(&r0, r0, m[0], 0);
        c = add64(&r1, r1, m[1], c);
        c = add64(&r2, r2, m[2], c);
        c = add64(&r3, r3, m[3], c);
            add64(&r4, r4,    0, c);
    }

    while (true) {
        b = sub64(&q0, r0, m[0], 0);
        b = sub64(&q1, r1, m[1], b);
        b = sub64(&q2, r2, m[2], b);
        b = sub64(&q3, r3, m[3], b);
        b = sub64(&q4, r4,    0, b);

        if (b != 0) {
            break;
        }

        r4 = q4;
        r3 = q3;
        r2 = q2;
        r1 = q1;
        r0 = q0;
    }

    res[3] = r3;
    res[2] = r2;
    res[1] = r1;
    res[0] = r0;
}
