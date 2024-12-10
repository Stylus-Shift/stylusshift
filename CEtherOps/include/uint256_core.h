#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint64_t u256[4];
typedef uint64_t u320[5];
typedef uint64_t u512[8];

#define MAX_U64 0xffffffffffffffffULL

void clear_words(u64 *dest, int num_words);
void copy_words(u64 *dest, u64 *src, int num_words);

u64 add64(u64 *res, const u64 x, const u64 y, const u64 carry);
bool add_overflow(u256 res, const u256 x, const u256 y);
void mul64(u64 *hi, u64 *lo, const u64 x, const u64 y);
u64 sub64(u64 *res, const u64 x, const u64 y, const u64 borrow);
void div32(u32 *quo, u32 *rem, const u32 hi, const u32 lo, const u32 y);
void div64(u64 *quo, u64 *rem, u64 hi, u64 lo, u64 y);

bool is_uint64(u256 x);
bool less_than(u256 x, u256 y);
bool greater_than(u256 x, u256 y);
bool signed_less_than(u256 x, u256 y);
bool signed_greater_than(u256 x, u256 y);
bool eq(u256 x, u256 y);
bool is_zero(u256 x);

bool uint64_with_overflow(u64 *n, u256 x);
void set_all_one(u256 x);
int sign(u256 x);
bool less_than_uint64(u256 x, u64 n);
bool greater_than_uint64(u256 x, u64 n);
int bit_len(u256 x);

u64 reciprocal2by1(const u64 d);
void udivrem2by1(u64 *quot, u64 *rem, const u64 uh, const u64 ul, const u64 d,
                 const u64 recip);
u64 udivrem_by1(u64 * quot, u64 * u, const int len_u, const u64 d);
u64 add_to(u64 *x, u64 *y, const int len_y);
u64 sub_mul_to(u64 *x, u64 *y, const int len_y, const u64 multiplier);
void udivrem_knuth(u64 *quot, u64 *u, int len_u, u64 *d, int len_d);
void udivrem(u64 *quot, u64 *u, const int len_u, const u256 d, u256 rem);
void umul_hop(u64 *hi, u64 *lo, const u64 z, const u64 x, const u64 y);
void umul_step(u64 *hi, u64 *lo, const u64 z, const u64 x, const u64 y,
               u64 carry);
void umul(u512 res, const u256 x, const u256 y);
void squared(u256 x);

// void lsh192(u256 res, u256 x);
// void lsh128(u256 res, u256 x);
// void lsh64(u256 res, u256 x);

void lsh(u256 res, u256 x, u64 n);
void rsh(u256 res, u256 x, u64 n);
void srsh(u256 res, u256 x, u64 n);

void reciprocal(u320 mu, u256 m);
void reduce4(u256 res, u512 x, u256 m, u64 *mu);