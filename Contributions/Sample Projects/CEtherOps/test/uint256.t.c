#include <stdio.h>
#include <assert.h>
#include <uint256.h>
#include "libuint256testgen.h"


#define NUM_TESTS 10000


/*
    Test helper functions
*/
// Print each 64 bit word of the 256 integer. Zero index is least significant
void print_uint(const char *label, u256 x) {
    printf("%s[0]: %lu\n%s[1]: %lu\n%s[2]: %lu\n%s[3]: %lu\n",
           label, x[0], label, x[1], label, x[2], label, x[3]);
}

// Compare each word of two 256 bit integers
void assert_eq(u256 have, u256 want) {
    assert(have[0] == want[0]);
    assert(have[1] == want[1]);
    assert(have[2] == want[2]);
    assert(have[3] == want[3]);
}

// Check for eqality and print a friendly message then trigger assertion error
void verbose_assert_eq(u256 have, u256 want, char name[], char msg[],
                       bool verbose) {
    if ((have[0] != want[0]) || (have[1] != want[1])
     || (have[2] != want[2]) || (have[3] != want[3]))
    {
        printf("%s failed: %s\n", name, msg);
        print_uint("want", want);
        print_uint("have", have);
        assert_eq(have, want);
    } else {
        if (verbose) {
            printf("%s succeeded: %s\n", name, msg);
        }
    }
}

// Check a boolean and print a friendly message then trigger assertion error
void verbose_assert_bool(bool have, bool want, char name[], char msg[],
                         bool verbose) {
    if (have != want)
    {
        printf("%s failed: %s\n", name, msg);
        printf("want %s", want ? "true" : "false");
        printf("have %s", have ? "true" : "false");
        assert(have == want);
    } else {
        if (verbose) {
            printf("%s succeeded: %s\n", name, msg);
        }
    }
}

/*
    Arithmetic tests
    ---
    Many of these tests are generated using random values and running them
    through the Golang implementation of uint256. I tried to include "edge"
    cases where I could think of them, since any of the comprehensible
    arithmetic operations would only involve the least significant words, ie.
    2 * 2 = 4 only involves x[0] and y[0], which is not a good test, but if x
    is 188272676318726381273681726319 then its representation in four words
    won't be recognizable.
*/
void test_add() {
    /*
        Test that each addition carries into the next significant word
    */
    u256 x = {MAX_U64, 0, 0, 0};
    u256 y = {1, 0, 0, 0};
    u256 want = {0, 1, 0, 0};

    u256 have;
    u256_add(have, x, y);

    verbose_assert_eq(have, want, "Add", "Should overflow/wrap correctly",
                      true);

    x[1] = MAX_U64;
    want[1] = 0;
    want[2] = 1;
    u256_add(have, x, y);

    verbose_assert_eq(have, want, "Add", "Should overflow/wrap correctly",
                      true);

    x[2] = MAX_U64;
    want[2] = 0;
    want[3] = 1;
    u256_add(have, x, y);

    verbose_assert_eq(have, want, "Add", "Should overflow/wrap correctly",
                      true);

    x[3] = MAX_U64;
    want[3] = 0;
    u256_add(have, x, y);

    verbose_assert_eq(have, want, "Add", "Should overflow/wrap correctly",
                      true);
}

void test_mul() {
    u256 x = {2, 0, 0, 0};
    u256 y = {2, 0, 0, 0};
    u256 want = {4, 0, 0, 0};

    u256 have;
    u256_add(have, x, y);

    verbose_assert_eq(have, want, "Mul", "Product should be correct",
                      true);
}

void test_sub() {
    u256 x = {4, 0, 0, 0};
    u256 y = {2, 0, 0, 0};
    u256 want = {2, 0, 0, 0};

    u256 have;
    u256_sub(have, x, y);

    verbose_assert_eq(have, want, "Sub", "Difference should be correct",
                      true);
}

void test_div() {
    u256 x = {4, 0, 0, 0};
    u256 y = {2, 0, 0, 0};
    u256 want = {2, 0, 0, 0};

    u256 have;
    u256_div(have, x, y);

    verbose_assert_eq(have, want, "Div", "Division should be correct", true);

    y[0] = 4;
    want[0] = 1;
    u256_div(have, x, y);
    verbose_assert_eq(have, want, "Div", "A number divided by itself is one",
                      true);

    y[0] = 0;
    want[0] = 0;
    u256_div(have, x, y);
    verbose_assert_eq(have, want, "Div", "Division by zero is zero",
                      true);
}

void test_sdiv() {
    u256 x = {10, 0, 0, 0};
    u256 y = {2, 0, 0, 0};
    u256 want = {5, 0, 0, 0};

    u256 have;
    u256_sdiv(have, x, y);

    verbose_assert_eq(have, want, "SDiv", "Should succeed", true);
}

void test_mod() {
    u256 x = {7, 0, 0, 0};
    u256 m = {2, 0, 0, 0};
    u256 want = {7%2, 0, 0, 0};

    u256 have;
    u256_mod(have, x, m);

    verbose_assert_eq(have, want, "Mod", "Should succeed", true);
}

void test_smod() {
    u256 x = { 7, 0, 0, 0 };
    u256 m = { MAX_U64-1, MAX_U64, MAX_U64, MAX_U64 }; // -2
    u256 want = {7%2, 0, 0, 0};

    u256 have;
    u256_smod(have, x, m);

    verbose_assert_eq(have, want, "SMod",
                      "Should succeed with negative m", true);

    x[0] = MAX_U64 - 6; // -7
    x[1] = MAX_U64;
    x[2] = MAX_U64;
    x[3] = MAX_U64;
    want[0] = MAX_U64; // -1
    want[1] = MAX_U64;
    want[2] = MAX_U64;
    want[3] = MAX_U64;

    u256_smod(have, x, m);

    verbose_assert_eq(have, want, "SMod",
                      "Should preserve negative sign of x", true);
}

void test_add_mod() {
    u256 x = {7, 0, 0, 0};
    u256 y = {10, 0, 0, 0};
    u256 m = {5, 0, 0, 0};
    u256 want = {17%5, 0, 0, 0};

    u256 have;
    u256_add_mod(have, x, y, m);

    verbose_assert_eq(have, want, "AddMod", "Should succeed", true);
}

void test_mul_mod() {
    u256 x = {7, 0, 0, 0};
    u256 y = {10, 0, 0, 0};
    u256 m = {5, 0, 0, 0};
    u256 want = {70%5, 0, 0, 0};

    u256 have;
    u256_mul_mod(have, x, y, m);

    verbose_assert_eq(have, want, "MulMod", "Should succeed", true);
}

void test_exp() {
    u256 x = {7, 0, 0, 0};
    u256 y = {3, 0, 0, 0};
    u256 want = {343, 0, 0, 0};

    u256 have;
    u256_exp(have, x, y);

    verbose_assert_eq(have, want, "Exp", "Should succeed", true);
}

void test_sign_extend() {
    u256 x = {
        0b1111111111111111111111111111000000000000000000000000000000000111,
        0b0000000000000000000000000000000000000000000000000000000000000000,
        0b0000000000000000000000000000000000000000000000000000000000000000,
        0b0000000000000000000000000000000000000000000000000000000000000000
    };
    u256 y = {7, 0, 0, 0}; // 8th byte
    u256 want = {
        0b1111111111111111111111111111000000000000000000000000000000000111,
        0b1111111111111111111111111111111111111111111111111111111111111111,
        0b1111111111111111111111111111111111111111111111111111111111111111,
        0b1111111111111111111111111111111111111111111111111111111111111111
    };

    u256 have;
    u256_sign_extend(have, x, y);

    verbose_assert_eq(have, want, "SignExtend",
                      "Should extend the sign of a negative uint64", true);

    x[0] = 0b1111111111111111111111111111000000000000000000000000000000000111;
    x[1] = 0b0000000000000000000000000000000000000000000000000000000000000000;
    x[2] = 0b0000000000000000000000000000000000000000000000000000000000000000;
    x[3] = 0b0000000000000000000000000000000000000000000000000000000000000000;
    y[0] = 15; // 16th byte
    want[0] = 0b1111111111111111111111111111000000000000000000000000000000000111;
    want[1] = 0b0000000000000000000000000000000000000000000000000000000000000000;
    want[2] = 0b0000000000000000000000000000000000000000000000000000000000000000;
    want[3] = 0b0000000000000000000000000000000000000000000000000000000000000000;

    u256_sign_extend(have, x, y);

    verbose_assert_eq(have, want, "SignExtend",
                      "Should preserve the sign of a positive uint64", true);

    x[0] = 0b0000000000000000000000000000000000000000000000000000000010000111;
    x[1] = 0b0000000000000000000000000000000000000000000000000000000000000000;
    x[2] = 0b0000000000000000000000000000000000000000000000000000000000000000;
    x[3] = 0b0000000000000000000000000000000000000000000000000000000000000000;
    y[0] = 0; // 1st byte
    want[0] = 0b1111111111111111111111111111111111111111111111111111111110000111;
    want[1] = 0b1111111111111111111111111111111111111111111111111111111111111111;
    want[2] = 0b1111111111111111111111111111111111111111111111111111111111111111;
    want[3] = 0b1111111111111111111111111111111111111111111111111111111111111111;

    u256_sign_extend(have, x, y);

    verbose_assert_eq(have, want, "SignExtend",
                      "Should extend the sign of a negative uint8", true);

    x[0] = 0b1111111111111111111111111111000000000000000000000000000000000111;
    x[1] = 0b0000000000000000000000000000000000000000000000000000000000000000;
    x[2] = 0b1000000000000000000000000000000000000000000000000000000000000000;
    x[3] = 0b0000000000000000000000000000000000000000000000000000000000000000;
    y[0] = 23; // 24th byte (uint192)
    want[0] = 0b1111111111111111111111111111000000000000000000000000000000000111;
    want[1] = 0b0000000000000000000000000000000000000000000000000000000000000000;
    want[2] = 0b1000000000000000000000000000000000000000000000000000000000000000;
    want[3] = 0b1111111111111111111111111111111111111111111111111111111111111111;

    u256_sign_extend(have, x, y);

    verbose_assert_eq(have, want, "SignExtend",
                      "Should extend the sign of a negative uint192", true);
}

/*
    Comparison tests
    ---
    I handpicked most of the cases for the comparison tests.
*/
void test_lt() {
    u256 x = {1, 0, 0, 0};
    u256 y = {1, 0, 0, 0};
    bool want = false;

    bool have = u256_lt(x, y);

    verbose_assert_bool(have, want, "Lt", "Should be false when equal", true);

    x[0] = 10;
    have = u256_lt(x, y);

    verbose_assert_bool(have, want, "Lt", "Should be false when greater than",
                        true);

    x[0] = 0;
    want = true;
    have = u256_lt(x, y);

    verbose_assert_bool(have, want, "Lt", "Should be true when less than",
                        true);

}

void test_gt() {
    u256 x = {1, 0, 0, 0};
    u256 y = {1, 0, 0, 0};
    bool want = false;

    bool have = u256_gt(x, y);

    verbose_assert_bool(have, want, "Gt", "Should be false when equal", true);

    x[0] = 0;
    have = u256_gt(x, y);

    verbose_assert_bool(have, want, "Gt", "Should be false when less than",
                        true);

    x[0] = 2;
    want = true;
    have = u256_gt(x, y);

    verbose_assert_bool(have, want, "Gt", "Should be true when greater than",
                        true);
}

void test_slt() {
    u256 x = {MAX_U64, MAX_U64, MAX_U64, MAX_U64}; // x = -1
    u256 y = {1, 0, 0, 0};                         // y = 1
    bool want = true;

    bool have = u256_slt(x, y);

    verbose_assert_bool(have, want, "Slt",
        "Negative number should be less than a positive number", true);

    y[0] = 0;                                      // y = 0
    have = u256_slt(x, y);

    verbose_assert_bool(have, want, "Slt",
        "Negative number should be less than zero", true);

    copy_words(&y[0], &x[0], 4);                   // y = -1
    x[0] -=1;                                      // x = -2

    have = u256_slt(x, y);
    verbose_assert_bool(have, want, "Slt",
        "More negative number should be less than less negative number", true);
}

void test_sgt() {
    u256 x = {1, 0, 0, 0};                         // x = 1
    u256 y = {MAX_U64, MAX_U64, MAX_U64, MAX_U64}; // y = -1
    bool want = true;

    bool have = u256_sgt(x, y);

    verbose_assert_bool(have, want, "Sgt",
        "Positive number should be greater than a negative number", true);

    clear_words(&y[0], 4);                         // y = 0
    have = u256_sgt(x, y);

    verbose_assert_bool(have, want, "Sgt",
        "Positive number should be greater than zero", true);

    copy_words(&y[0], &x[0], 4);                   // y = 1
    x[0] +=1;                                      // x = 2

    have = u256_sgt(x, y);
    verbose_assert_bool(have, want, "Sgt",
        "More positive number should be greater than less positive number",
        true);
}

void test_eq() {
    u256 x = {1, 0, 0, 0};
    u256 y = {1, 0, 0, 0};
    bool want = true;

    bool have = u256_eq(x, y);

    verbose_assert_bool(have, want, "Eq", "Should be true when equal", true);

    x[0] = 0;
    want = false;
    have = u256_eq(x, y);

    verbose_assert_bool(have, want, "Eq", "Should be false when not equal",
                        true);
}

void test_is_zero() {
    u256 x = {1, 0, 0, 0};
    bool want = false;

    bool have = u256_is_zero(x);

    verbose_assert_bool(have, want, "IsZero", "Should be false when nonzero",
                        true);

    x[0] = 0;
    want = true;
    have = u256_is_zero(x);

    verbose_assert_bool(have, want, "IsZero", "Should be true when zero", true);
}

/*
    Bitwise tests
    ---
    For many of the bitwise operations tests, I arbitrarily chose the bits.
    Where applicable, I wrote the integers in binary notation so that the
    expected result is easy to understand from reading alone.
*/
void test_and() {
    u256 x = {
        0b1111111111111111111111111111000000000000000000000000000000000111,
        0b1111111111111111111111111111000000000000000000000000000000000111,
        0b1111111111111111111111111111000000000000000000000000000000000111,
        0b1111111111111111111111111111000000000000000000000000000000000111
    };
    u256 y = {
        0b1110000000000000000001111111111111111111111111111111111111111111,
        0b1110000000000000000001111111111111111111111111111111111111111111,
        0b1110000000000000000001111111111111111111111111111111111111111111,
        0b1110000000000000000001111111111111111111111111111111111111111111
    };
    u256 want = {
        0b1110000000000000000001111111000000000000000000000000000000000111,
        0b1110000000000000000001111111000000000000000000000000000000000111,
        0b1110000000000000000001111111000000000000000000000000000000000111,
        0b1110000000000000000001111111000000000000000000000000000000000111
    };

    u256 have;
    u256_and(have, x, y);

    verbose_assert_eq(have, want, "And", "Should succeed", true);
}

void test_or() {
    u256 x = {
        0b1111111111111111111111111111000000000000000000000000000000000111,
        0b1111111111111111111111111111000000000000000000000000000000000111,
        0b1111111111111111111111111111000000000000000000000000000000000111,
        0b1111111111111111111111111111000000000000000000000000000000000111
    };
    u256 y = {
        0b1110000000000000000001111111111111111111111111111111111111111111,
        0b1110000000000000000001111111111111111111111111111111111111111111,
        0b1110000000000000000001111111111111111111111111111111111111111111,
        0b1110000000000000000001111111111111111111111111111111111111111111
    };
    u256 want = {
        0b1111111111111111111111111111111111111111111111111111111111111111,
        0b1111111111111111111111111111111111111111111111111111111111111111,
        0b1111111111111111111111111111111111111111111111111111111111111111,
        0b1111111111111111111111111111111111111111111111111111111111111111
    };

    u256 have;
    u256_or(have, x, y);

    verbose_assert_eq(have, want, "Or", "Should succeed", true);
}

void test_xor() {
    u256 x = {
        0b1111111111111111111111111111000000000000000000000000000000000111,
        0b1111111111111111111111111111000000000000000000000000000000000111,
        0b1111111111111111111111111111000000000000000000000000000000000111,
        0b1111111111111111111111111111000000000000000000000000000000000111
    };
    u256 y = {
        0b1110000000000000000001111111111111111111111111111111111111111111,
        0b1110000000000000000001111111111111111111111111111111111111111111,
        0b1110000000000000000001111111111111111111111111111111111111111111,
        0b1110000000000000000001111111111111111111111111111111111111111111
    };
    u256 want = {
        0b0001111111111111111110000000111111111111111111111111111111111000,
        0b0001111111111111111110000000111111111111111111111111111111111000,
        0b0001111111111111111110000000111111111111111111111111111111111000,
        0b0001111111111111111110000000111111111111111111111111111111111000
    };

    u256 have;
    u256_xor(have, x, y);

    verbose_assert_eq(have, want, "Xor", "Should succeed", true);
}

void test_not() {
    u256 x = {
        0b1111111111111111111111111111000000000000000000000000000000000111,
        0b1111111111111111111111111111000000000000000000000000000000000111,
        0b1111111111111111111111111111000000000000000000000000000000000111,
        0b1111111111111111111111111111000000000000000000000000000000000111
    };
    u256 want = {
        0b0000000000000000000000000000111111111111111111111111111111111000,
        0b0000000000000000000000000000111111111111111111111111111111111000,
        0b0000000000000000000000000000111111111111111111111111111111111000,
        0b0000000000000000000000000000111111111111111111111111111111111000
    };

    u256 have;
    u256_not(have, x);

    verbose_assert_eq(have, want, "Not", "Should succeed", true);
}

void test_byte() {
    /*
        Check each index in this test. Note that `byte` indexes the byte as if
        the number was big-endian, even though we store them in little-endian
        order in the u256 type.
    */
    u256 x = {
        0x00000000000000ee, // use a diff byte for each u64 word
        0x00000000000000dd,
        0x00000000000000cc,
        0x00000000000000bb
    };
    u256 i = {32, 0, 0, 0}; // start above 31 because i[0] -= 1 in loop.
    u256 want = {
        0x00000000000000ee,
        0x0000000000000000,
        0x0000000000000000,
        0x0000000000000000
    };
    u256 have;    // result of operation
    char msg[32]; // test message depends on which byte is being checked

    // check each word
    for (int word = 0; word < 4; word++) {
        if (word == 1) {
            want[0] = 0x00000000000000dd;
        }
        if (word == 2) {
            want[0] = 0x00000000000000cc;
        }
        if (word == 3) {
            want[0] = 0x00000000000000bb;
        }

        // check each byte in each word
        for (int j = 0; j < 8; j++) {
            // first byte is already set in each word of x
            if (j > 0) {
                x[word] <<= 8;
            }
            i[0] -= 1;
            u256_byte(have, x, i);
            sprintf(msg, "Should retrieve byte %lu", i[0]);
            verbose_assert_eq(have, want, "Byte", msg, false);
        }
    }
}

void test_shl() {
    u256 x = {
        0b1110000000000000000001111111000000000000000000000000000000000111,
        0b1110000000000000000001111111000000000000000000000000000000000111,
        0b1110000000000000000001111111000000000000000000000000000000000111,
        0b1110000000000000000001111111000000000000000000000000000000000111
    };
    u256 shift = {3, 0, 0, 0};
    // Note: shift left zero pads on right side of least significant word
    u256 want = {
        0b0000000000000000001111111000000000000000000000000000000000111000,
        0b0000000000000000001111111000000000000000000000000000000000111111,
        0b0000000000000000001111111000000000000000000000000000000000111111,
        0b0000000000000000001111111000000000000000000000000000000000111111
    };

    u256 have;
    u256_shl(have, x, shift);

    verbose_assert_eq(have, want, "Shl", "Should left shift by 3 bits", true);

    shift[0] = 256;
    clear_words(&want[0], 4);
    u256_shl(have, x, shift);

    verbose_assert_eq(have, want, "Shl", "Maximum left shift should return 0",
                      true);
}

void test_shr() {
    u256 x = {
        0b1110001111111111111111111111111111111111111111111111111111000111,
        0b1110001111111111111111111111111111111111111111111111111111000111,
        0b1110001111111111111111111111111111111111111111111111111111000111,
        0b1110001111111111111111111111111111111111111111111111111111000111
    };
    u256 shift = {3, 0, 0, 0};
    // Note: shift right zero pads on left side of most significant word
    u256 want = {
        0b1111110001111111111111111111111111111111111111111111111111111000,
        0b1111110001111111111111111111111111111111111111111111111111111000,
        0b1111110001111111111111111111111111111111111111111111111111111000,
        0b0001110001111111111111111111111111111111111111111111111111111000
    };

    u256 have;
    u256_shr(have, x, shift);

    verbose_assert_eq(have, want, "Shr", "Should right shift by 3 bits", true);

    shift[0] = 256;
    clear_words(&want[0], 4);
    u256_shr(have, x, shift);

    verbose_assert_eq(have, want, "Shr", "Maximum right shift should return 0",
                      true);
}

void test_sar() {
    u256 x = {
        0b1110001111111111111111111111111111111111111111111111111111000111,
        0b1110001111111111111111111111111111111111111111111111111111000111,
        0b1110001111111111111111111111111111111111111111111111111111000111,
        0b1110001111111111111111111111111111111111111111111111111111000111
    };
    u256 shift = {3, 0, 0, 0};
    u256 want = {
        0b1111110001111111111111111111111111111111111111111111111111111000,
        0b1111110001111111111111111111111111111111111111111111111111111000,
        0b1111110001111111111111111111111111111111111111111111111111111000,
        0b1111110001111111111111111111111111111111111111111111111111111000
    };

    u256 have;
    u256_sar(have, x, shift);

    verbose_assert_eq(have, want, "Sar",
        "Shift right for negative value should fill left with 1s", true);

    x[3] = 0b0110001111111111111111111111111111111111111111111111111111000111;
    want[3] =
        0b0000110001111111111111111111111111111111111111111111111111111000;

    u256_sar(have, x, shift);

    verbose_assert_eq(have, want, "Sar",
        "Shift right for positive value should fill left with 0s", true);

    shift[0] = 256;
    clear_words(&want[0], 4);
    u256_sar(have, x, shift);

    verbose_assert_eq(have, want, "Sar",
        "Maximum shift right for positive value should return all 0s", true);

    x[3] = 0b1000000000000000000000000000000000000000000000000000000000000000;
    want[0] = MAX_U64; want[1] = MAX_U64; want[2] = MAX_U64; want[3] = MAX_U64;
    u256_sar(have, x, shift);

    verbose_assert_eq(have, want, "Sar",
        "Maximum shift right for negative value should return all 1s", true);
}

/*
    Randomized tests: arithmetic
*/
void test_add_random() {
    u256 x, y, have, want;

    printf("Testing Add\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenAddTest((char*)x, (char*)y, (char*)want);
        u256_add(have, x, y);
        verbose_assert_eq(have, want, "Add",
                          "Random addition should match Go implementation",
                          false);
    }
}

void test_mul_random() {
    u256 x, y, have, want;

    printf("Testing Mul\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenMulTest((char*)x, (char*)y, (char*)want);
        u256_mul(have, x, y);
        verbose_assert_eq(have, want, "Mul",
                          "Random multiply should match Go implementation",
                          false);
    }
}

void test_sub_random() {
    u256 x, y, have, want;

    printf("Testing Sub\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenSubTest((char*)x, (char*)y, (char*)want);
        u256_sub(have, x, y);
        verbose_assert_eq(have, want, "Sub",
                          "Random subtraction should match Go implementation",
                          false);
    }
}

void test_div_random() {
    u256 x, y, have, want;

    printf("Testing Div\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenDivTest((char*)x, (char*)y, (char*)want);
        u256_div(have, x, y);
        verbose_assert_eq(have, want, "Div",
                          "Random divide should match Go implementation",
                          false);
    }
}

void test_sdiv_random() {
    u256 x, y, have, want;

    printf("Testing SDiv\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenSDivTest((char*)x, (char*)y, (char*)want);
        u256_sdiv(have, x, y);
        verbose_assert_eq(have, want, "SDiv",
                          "Random signed divide should match Go implementation",
                          false);
    }
}

void test_mod_random() {
    u256 x, y, have, want;

    printf("Testing Mod\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenModTest((char*)x, (char*)y, (char*)want);
        u256_mod(have, x, y);
        verbose_assert_eq(have, want, "Mod",
                          "Random modulus should match Go implementation",
                          false);
    }
}

void test_smod_random() {
    u256 x, y, have, want;

    printf("Testing SMod\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenSModTest((char*)x, (char*)y, (char*)want);
        u256_smod(have, x, y);
        verbose_assert_eq(have, want, "SMod",
                        "Random signed modulus should match Go implementation",
                        false);
    }
}

void test_add_mod_random() {
    u256 x, y, m, have, want;

    printf("Testing AddMod\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenAddModTest((char*)x, (char*)y, (char*)m, (char*)want);
        u256_add_mod(have, x, y, m);
        verbose_assert_eq(have, want, "AddMod",
                        "Random u256_add u256_mod should match Go implementation",
                        false);
    }
}

void test_mul_mod_random() {
    u256 x, y, m, have, want;

    printf("Testing MulMod\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenMulModTest((char*)x, (char*)y, (char*)m, (char*)want);
        u256_mul_mod(have, x, y, m);
        verbose_assert_eq(have, want, "MulMod",
                        "Random mul u256_mod should match Go implementation",
                        false);
    }
}

void test_exp_random() {
    u256 x, y, have, want;

    printf("Testing Exp\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenExpTest((char*)x, (char*)y, (char*)want);
        u256_exp(have, x, y);
        verbose_assert_eq(have, want, "Exp",
                        "Random exponentiation should match Go implementation",
                        false);
    }
}

void test_sign_extend_random() {
    u256 x, y, have, want;

    printf("Testing SignExtend\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenSignExtendTest((char*)x, (char*)y, (char*)want);
        u256_sign_extend(have, x, y);
        verbose_assert_eq(have, want, "SignExtend",
                        "Random sign extension should match Go implementation",
                        false);
    }
}

/*
    Randomized tests: comparison
*/
void test_lt_random() {
    char r;
    char *result = &r;

    u256 x, y;

    printf("Testing Lt\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenLtTest((char*)x, (char*)y, result);

        bool want = r == 1 ? true : false;
        bool have = u256_lt(x, y);
        verbose_assert_bool(have, want, "Lt",
                        "Random less than should match Go implementation",
                        false);
    }
}

void test_gt_random() {
    char r;
    char *result = &r;

    u256 x, y;

    printf("Testing Gt\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenGtTest((char*)x, (char*)y, result);

        bool want = r == 1 ? true : false;
        bool have = u256_gt(x, y);
        verbose_assert_bool(have, want, "Gt",
                        "Random greater than should match Go implementation",
                        false);
    }
}

void test_slt_random() {
    char r;
    char *result = &r;

    u256 x, y;

    printf("Testing Slt\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenSltTest((char*)x, (char*)y, result);

        bool want = r == 1 ? true : false;
        bool have = u256_slt(x, y);
        verbose_assert_bool(have, want, "Slt",
                    "Random signed less than should match Go implementation",
                    false);
    }
}

void test_sgt_random() {
    char r;
    char *result = &r;

    u256 x, y;

    printf("Testing Sgt\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenSgtTest((char*)x, (char*)y, result);

        bool want = r == 1 ? true : false;
        bool have = u256_sgt(x, y);
        verbose_assert_bool(have, want, "Sgt",
                    "Random signed greater than should match Go implementation",
                    false);
    }
}

void test_eq_random() {
    char r;
    char *result = &r;

    u256 x, y;

    printf("Testing Eq\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenEqTest((char*)x, (char*)y, result);

        bool want = r == 1 ? true : false;
        bool have = u256_eq(x, y);
        verbose_assert_bool(have, want, "Eq",
                            "Random equals should match Go implementation",
                            false);
    }
}

void test_is_zero_random() {
    char r;
    char *result = &r;

    u256 x;

    printf("Testing IsZero\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenIsZeroTest((char*)x, result);

        bool want = r == 1 ? true : false;
        bool have = u256_is_zero(x);
        verbose_assert_bool(have, want, "IsZero",
                            "Random is zero should match Go implementation",
                            false);
    }
}

/*
    Randomized tests: bitwise
*/
void test_and_random() {
    u256 x, y, have, want;

    printf("Testing And\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenAndTest((char*)x, (char*)y, (char*)want);
        u256_and(have, x, y);
        verbose_assert_eq(have, want, "And",
                        "Random bitwise and should match Go implementation",
                        false);
    }
}

void test_or_random() {
    u256 x, y, have, want;

    printf("Testing Or\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenOrTest((char*)x, (char*)y, (char*)want);
        u256_or(have, x, y);
        verbose_assert_eq(have, want, "Or",
                        "Random bitwise or should match Go implementation",
                        false);
    }
}

void test_xor_random() {
    u256 x, y, have, want;

    printf("Testing Xor\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenXorTest((char*)x, (char*)y, (char*)want);
        u256_xor(have, x, y);
        verbose_assert_eq(have, want, "Xor",
                        "Random bitwise xor should match Go implementation",
                        false);
    }
}

void test_not_random() {
    u256 x, have, want;

    printf("Testing Not\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenNotTest((char*)x, (char*)want);
        u256_not(have, x);
        verbose_assert_eq(have, want, "Not",
                        "Random bitwise not should match Go implementation",
                        false);
    }
}

void test_byte_random() {
    u256 x, y, have, want;

    printf("Testing Byte\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenByteTest((char*)x, (char*)y, (char*)want);
        u256_byte(have, x, y);
        verbose_assert_eq(have, want, "Byte",
                        "Random byte index should match Go implementation",
                        false);
    }
}

void test_shl_random() {
    u256 x, y, have, want;

    printf("Testing Shl\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenShlTest((char*)x, (char*)y, (char*)want);
        u256_shl(have, x, y);
        verbose_assert_eq(have, want, "Shl",
                        "Random left shift should match Go implementation",
                        false);
    }
}

void test_shr_random() {
    u256 x, y, have, want;

    printf("Testing Shr\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenShrTest((char*)x, (char*)y, (char*)want);
        u256_shr(have, x, y);
        verbose_assert_eq(have, want, "Shl",
                        "Random right shift should match Go implementation",
                        false);
    }
}

void test_sar_random() {
    u256 x, y, have, want;

    printf("Testing Sar\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        GenSarTest((char*)x, (char*)y, (char*)want);
        u256_sar(have, x, y);
        verbose_assert_eq(have, want, "Sar",
                    "Random signed right shift should match Go implementation",
                    false);
    }
}


int main() {
    //////////////////////////// Arithmetic tests
    test_add();
    test_mul();
    test_sub();
    test_div();
    test_sdiv();
    test_mod();
    test_smod();
    test_add_mod();
    test_mul_mod();
    test_exp();
    test_sign_extend();

    //////////////////////////// Comparison tests
    test_lt();
    test_gt();
    test_slt();
    test_sgt();
    test_eq();
    test_is_zero();

    //////////////////////////// Bitwise tests
    test_and();
    test_or();
    test_xor();
    test_not();
    test_byte();
    test_shl();
    test_shr();
    test_sar();

    //////////////////////////// Random tests: Arithmetic
    printf("Running %i random tests!\n", NUM_TESTS);
    test_add_random();
    test_mul_random();
    test_sub_random();
    test_div_random();
    test_sdiv_random();
    test_mod_random();
    test_smod_random();
    test_add_mod_random();
    test_mul_mod_random();
    test_exp_random();
    test_sign_extend_random();

    //////////////////////////// Random tests: Comparison
    test_lt_random();
    test_gt_random();
    test_slt_random();
    test_sgt_random();
    test_eq_random();
    test_is_zero_random();

    //////////////////////////// Random tests: Bitwise
    test_and_random();
    test_or_random();
    test_xor_random();
    test_not_random();
    test_byte_random();
    test_shl_random();
    test_shr_random();
    test_sar_random();

    printf("All tests passed!\n");
}
