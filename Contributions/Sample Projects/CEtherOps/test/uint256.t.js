const fs = require("fs");
const ethers = require("ethers");
const crypto = require("crypto");


const NUM_TESTS = 60;

const endpoint = process.env.ENDPOINT;

function randomBigInt() {
    let b = crypto.randomBytes(32);
    return BigInt('0x' + b.toString('hex'));
}

const opcodes = [
    'Add', 'Mul', 'Sub', 'Div', 'SDiv', 'Mod', 'SMod', 'AddMod', 'MulMod',
    'Exp', 'SignExtend', 'Lt', 'Gt', 'Slt', 'Sgt', 'Eq', 'IsZero', 'And', 'Or',
    'Xor', 'Not', 'Byte', 'Shl', 'Shr', 'Sar'
];

const bitmasks = [];
for (let i = 0; i < 25; i++) {
    bitmasks[i] = 1n << BigInt(i);
}

// commenting out any of these values removes them from the test call
const opcodes_to_test = [
    'Add',
    'Mul',
    'Sub',
    'Div',
    'SDiv',
    'Mod',
    'SMod',
    'AddMod',
    'MulMod',
    'Exp',
    'SignExtend',
    'Lt',
    'Gt',
    'Slt',
    'Sgt',
    'Eq',
    'IsZero',
    'And',
    'Or',
    'Xor',
    'Not',
    'Byte',
    'Shl',
    'Shr',
    'Sar'
];

var tests_map = 0n;
for (let i = 0; i < opcodes_to_test.length; i++) {
    const bitmask_index = opcodes.indexOf(opcodes_to_test[i]);
    tests_map |= BigInt(bitmasks[bitmask_index]);
}

async function main() {
    const provider = new ethers.JsonRpcProvider(endpoint)

    const data = JSON.parse(fs.readFileSync('./test/test_address.json'));
    const addr = data.address;
    console.log("address: ", addr);

    const contract_data = JSON.parse(fs.readFileSync('build/interface.json'));
    const abi = contract_data.contracts.uint256_test.Uint256Test.abi;
    const contract = new ethers.Contract(addr, abi, provider);

    const values = [];
    for (let i = 0; i<NUM_TESTS*3; i++) {
        values[i] = randomBigInt();
    }
    // the tests work by using the three inputs:
    // x:values[0], y:values[1], z:values[2]
    // x:values[3], y:values[4], z:values[5]
    // and so on...
    // the expected result is determined by the appropriate EVM opcode, e.g.
    // want := addmod(x, y, z) or want := add(x, y)
    // z is ignore for tests that only take two arguments.

    // restrict some of the y values to be less than 255 and 30, because some
    // tests are basically meaningless outside of this range. in particular,
    // shl, shr, sar, byte

    const first_sixth = NUM_TESTS/6;
    // first 1/6 of tests are 255 limit
    for (let i = 0; i < first_sixth; i++) {
        values[i*3 + 1] %= 255n;
    }
    // second 1/6 of tests are 30 limit
    for (let i = first_sixth; i < first_sixth*2; i++) {
        values[i*3 + 1] %= 30n;
    }

    const test_result = await contract.test_values(values, tests_map);

    const [passing, opcode, have, want, x, y, z] = test_result;
    if (passing) {
        console.log('Test passed!');
    } else {
        console.log("Test failed. Here's the data:");
        console.log(`Opcode: ${opcodes[opcode]}`);
        console.log(`have: ${have}`);
        console.log(`want: ${want}`);
        console.log(`x: ${x}`);
        console.log(`y: ${y}`);
        console.log(`z: ${z}`);
    }
}

main()
