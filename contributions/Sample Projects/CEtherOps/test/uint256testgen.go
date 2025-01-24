package main

import "C"
import (
    "crypto/rand"
    "unsafe"
    "github.com/holiman/uint256"
)


// evm opcodes have some logic beyond uint256.go
func opSHL(res, x, y *uint256.Int) {
    if y.LtUint64(256) {
        res.Lsh(x, uint(y.Uint64()))
    } else {
        res.Clear()
    }
}

// evm opcodes have some logic beyond uint256.go
func opSHR(res, x, y *uint256.Int) {
    if y.LtUint64(256) {
        res.Rsh(x, uint(y.Uint64()))
    } else {
        res.Clear()
    }
}

// evm opcodes have some logic beyond uint256.go
func opSAR(res, value, shift *uint256.Int) {
    if shift.GtUint64(256) {
        if value.Sign() >= 0 {
            res.Clear()
        } else {
            res.SetAllOne()
        }
        return
    }
    n := uint(shift.Uint64())
    res.SRsh(value, n)
}

func bytes_to_uint(bytes_data []byte) *uint256.Int {
    // convert to *uint256.Int
    d := &uint256.Int{}

    // SetBytes interprets the data as big endian, but its little endian in C
    d_bytes_be := make([]byte, 32)
    for i, b := range bytes_data {
        d_bytes_be[31 - i] = b
    }
    d.SetBytes(d_bytes_be)

    return d
}

func randomize_to_uint(c_bytes []byte) *uint256.Int {
    // this overwrites random bytes to c_bytes
    if _, err := rand.Read(c_bytes); err != nil {
        panic(err)
    }

    return bytes_to_uint(c_bytes)
}

// some tests, like shift operations, need small integers
func randomize_to_small_int(c_bytes []byte, n byte) *uint256.Int {
    // this overwrites random bytes to c_bytes
    if _, err := rand.Read(c_bytes); err != nil {
        panic(err)
    }

    c_bytes[0] %= n
    for i := range c_bytes[1:] {
        c_bytes[i+1] = 0
    }

    return bytes_to_uint(c_bytes)
}

//export GenAddTest
func GenAddTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_uint(y_bytes)

    // compute result
    res_int := &uint256.Int{}
    res_int.Add(x_int, y_int)

    // write result as little endian bytes
    res_bytes := (*[32]byte)(unsafe.Pointer(res))[:32:32]
    for i, b := range res_int.Bytes32() {
        res_bytes[31-i] = b
    }
}

//export GenMulTest
func GenMulTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_uint(y_bytes)

    // compute result
    res_int := &uint256.Int{}
    res_int.Mul(x_int, y_int)

    // write result as little endian bytes
    res_bytes := (*[32]byte)(unsafe.Pointer(res))[:32:32]
    for i, b := range res_int.Bytes32() {
        res_bytes[31-i] = b
    }
}

//export GenSubTest
func GenSubTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_uint(y_bytes)

    // compute result
    res_int := &uint256.Int{}
    res_int.Sub(x_int, y_int)

    // write result as little endian bytes
    res_bytes := (*[32]byte)(unsafe.Pointer(res))[:32:32]
    for i, b := range res_int.Bytes32() {
        res_bytes[31-i] = b
    }
}

//export GenDivTest
func GenDivTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_uint(y_bytes)

    // compute result
    res_int := &uint256.Int{}
    res_int.Div(x_int, y_int)

    // write result as little endian bytes
    res_bytes := (*[32]byte)(unsafe.Pointer(res))[:32:32]
    for i, b := range res_int.Bytes32() {
        res_bytes[31-i] = b
    }
}

//export GenSDivTest
func GenSDivTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_uint(y_bytes)

    // compute result
    res_int := &uint256.Int{}
    res_int.SDiv(x_int, y_int)

    // write result as little endian bytes
    res_bytes := (*[32]byte)(unsafe.Pointer(res))[:32:32]
    for i, b := range res_int.Bytes32() {
        res_bytes[31 - i] = b
    }
}

//export GenModTest
func GenModTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_uint(y_bytes)

    // compute result
    res_int := &uint256.Int{}
    res_int.Mod(x_int, y_int)

    // write result as little endian bytes
    res_bytes := (*[32]byte)(unsafe.Pointer(res))[:32:32]
    for i, b := range res_int.Bytes32() {
        res_bytes[31 - i] = b
    }
}

//export GenSModTest
func GenSModTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_uint(y_bytes)

    // compute result
    res_int := &uint256.Int{}
    res_int.SMod(x_int, y_int)

    // write result as little endian bytes
    res_bytes := (*[32]byte)(unsafe.Pointer(res))[:32:32]
    for i, b := range res_int.Bytes32() {
        res_bytes[31 - i] = b
    }
}

//export GenAddModTest
func GenAddModTest(x, y, m, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_uint(y_bytes)

    // typecast m to byte slice
    m_bytes := (*[32]byte)(unsafe.Pointer(m))[:32:32]
    m_int := randomize_to_uint(m_bytes)

    // compute result
    res_int := &uint256.Int{}
    res_int.AddMod(x_int, y_int, m_int)

    // write result as little endian bytes
    res_bytes := (*[32]byte)(unsafe.Pointer(res))[:32:32]
    for i, b := range res_int.Bytes32() {
        res_bytes[31 - i] = b
    }
}

//export GenMulModTest
func GenMulModTest(x, y, m, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_uint(y_bytes)

    // typecast m to byte slice
    m_bytes := (*[32]byte)(unsafe.Pointer(m))[:32:32]
    m_int := randomize_to_uint(m_bytes)

    // compute result
    res_int := &uint256.Int{}
    res_int.MulMod(x_int, y_int, m_int)

    // write result as little endian bytes
    res_bytes := (*[32]byte)(unsafe.Pointer(res))[:32:32]
    for i, b := range res_int.Bytes32() {
        res_bytes[31 - i] = b
    }
}

//export GenExpTest
func GenExpTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_uint(y_bytes)

    // compute result
    res_int := &uint256.Int{}
    res_int.Exp(x_int, y_int)

    // write result as little endian bytes
    res_bytes := (*[32]byte)(unsafe.Pointer(res))[:32:32]
    for i, b := range res_int.Bytes32() {
        res_bytes[31 - i] = b
    }
}

//export GenSignExtendTest
func GenSignExtendTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    // choose double the condition (y > 30), to test all cases
    // see: https://github.com/holiman/uint256/blob/master/uint256.go#L1230
    y_int := randomize_to_small_int(y_bytes, 60)

    // compute result
    res_int := &uint256.Int{}
    res_int.ExtendSign(x_int, y_int)

    // write result as little endian bytes
    res_bytes := (*[32]byte)(unsafe.Pointer(res))[:32:32]
    for i, b := range res_int.Bytes32() {
        res_bytes[31 - i] = b
    }
}

//export GenLtTest
func GenLtTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_uint(y_bytes)

    // compute result
    result := x_int.Lt(y_int)

    // write result as little endian bytes
    res_bytes := (*[1]byte)(unsafe.Pointer(res))[:1:1]
    if result {
        res_bytes[0] = 1
    } else {
        res_bytes[0] = 0
    }
}

//export GenGtTest
func GenGtTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_uint(y_bytes)

    // compute result
    result := x_int.Gt(y_int)

    // write result as little endian bytes
    res_bytes := (*[1]byte)(unsafe.Pointer(res))[:1:1]
    if result {
        res_bytes[0] = 1
    } else {
        res_bytes[0] = 0
    }
}

//export GenSltTest
func GenSltTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_uint(y_bytes)

    // compute result
    result := x_int.Slt(y_int)

    // write result as little endian bytes
    res_bytes := (*[1]byte)(unsafe.Pointer(res))[:1:1]
    if result {
        res_bytes[0] = 1
    } else {
        res_bytes[0] = 0
    }
}

//export GenSgtTest
func GenSgtTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_uint(y_bytes)

    // compute result
    result := x_int.Sgt(y_int)

    // write result as little endian bytes
    res_bytes := (*[1]byte)(unsafe.Pointer(res))[:1:1]
    if result {
        res_bytes[0] = 1
    } else {
        res_bytes[0] = 0
    }
}

//export GenEqTest
func GenEqTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_uint(y_bytes)

    // compute result
    result := x_int.Eq(y_int)

    // write result as little endian bytes
    res_bytes := (*[1]byte)(unsafe.Pointer(res))[:1:1]
    if result {
        res_bytes[0] = 1
    } else {
        res_bytes[0] = 0
    }
}

//export GenIsZeroTest
func GenIsZeroTest(x, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // compute result
    result := x_int.IsZero()

    // write result as little endian bytes
    res_bytes := (*[1]byte)(unsafe.Pointer(res))[:1:1]
    if result {
        res_bytes[0] = 1
    } else {
        res_bytes[0] = 0
    }
}

//export GenAndTest
func GenAndTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_uint(y_bytes)

    // compute result
    res_int := &uint256.Int{}
    res_int.And(x_int, y_int)

    // write result as little endian bytes
    res_bytes := (*[32]byte)(unsafe.Pointer(res))[:32:32]
    for i, b := range res_int.Bytes32() {
        res_bytes[31 - i] = b
    }
}

//export GenOrTest
func GenOrTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_uint(y_bytes)

    // compute result
    res_int := &uint256.Int{}
    res_int.Or(x_int, y_int)

    // write result as little endian bytes
    res_bytes := (*[32]byte)(unsafe.Pointer(res))[:32:32]
    for i, b := range res_int.Bytes32() {
        res_bytes[31 - i] = b
    }
}

//export GenXorTest
func GenXorTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_uint(y_bytes)

    // compute result
    res_int := &uint256.Int{}
    res_int.Xor(x_int, y_int)

    // write result as little endian bytes
    res_bytes := (*[32]byte)(unsafe.Pointer(res))[:32:32]
    for i, b := range res_int.Bytes32() {
        res_bytes[31 - i] = b
    }
}

//export GenNotTest
func GenNotTest(x, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // compute result
    res_int := &uint256.Int{}
    res_int.Not(x_int)

    // write result as little endian bytes
    res_bytes := (*[32]byte)(unsafe.Pointer(res))[:32:32]
    for i, b := range res_int.Bytes32() {
        res_bytes[31 - i] = b
    }
}

//export GenByteTest
func GenByteTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    // choose double the length of a uint in bytes, to test all cases
    y_int := randomize_to_small_int(y_bytes, 64)

    // compute result
    res_int := &uint256.Int{x_int[0], x_int[1], x_int[2], x_int[3]}
    res_int.Byte(y_int);

    // write result as little endian bytes
    res_bytes := (*[32]byte)(unsafe.Pointer(res))[:32:32]
    for i, b := range res_int.Bytes32() {
        res_bytes[31 - i] = b
    }
}

//export GenShlTest
func GenShlTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_small_int(y_bytes, 255)

    // compute result
    res_int := &uint256.Int{}
    opSHL(res_int, x_int, y_int)

    // write result as little endian bytes
    res_bytes := (*[32]byte)(unsafe.Pointer(res))[:32:32]
    for i, b := range res_int.Bytes32() {
        res_bytes[31 - i] = b
    }
}

//export GenShrTest
func GenShrTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_small_int(y_bytes, 255)

    // compute result
    res_int := &uint256.Int{}
    opSHR(res_int, x_int, y_int)

    // write result as little endian bytes
    res_bytes := (*[32]byte)(unsafe.Pointer(res))[:32:32]
    for i, b := range res_int.Bytes32() {
        res_bytes[31 - i] = b
    }
}

//export GenSarTest
func GenSarTest(x, y, res *C.char) {
    // typecast x to byte slice
    x_bytes := (*[32]byte)(unsafe.Pointer(x))[:32:32]
    x_int := randomize_to_uint(x_bytes)

    // typecast y to byte slice
    y_bytes := (*[32]byte)(unsafe.Pointer(y))[:32:32]
    y_int := randomize_to_small_int(y_bytes, 255)

    // compute result
    res_int := &uint256.Int{}
    opSAR(res_int, x_int, y_int)

    // write result as little endian bytes
    res_bytes := (*[32]byte)(unsafe.Pointer(res))[:32:32]
    for i, b := range res_int.Bytes32() {
        res_bytes[31 - i] = b
    }
}

func main() {}
