const fs = require('fs');
const ethers = require('ethers');


const endpoint = process.env.ENDPOINT;
const private_key = process.env.PRIVATE_KEY;

async function main() {
    // contract metadata
    const iface = JSON.parse(fs.readFileSync('build/interface.json'));
    const abi = iface.contracts.uint256_test.Uint256Test.abi;
    const bytecode = '0x'+iface.contracts.uint256_test.Uint256Test.evm.bytecode.object;

    // signer / provider
    const provider = new ethers.JsonRpcProvider(endpoint);
    const wallet = new ethers.Wallet(private_key, provider);

    // contract factory
    const factory = new ethers.ContractFactory(abi, bytecode, wallet);

    // deploy contract
    const contract = await factory.deploy();

    const data = { address: contract.target };
    console.log(data);
    fs.writeFileSync('./test/test_address.json', JSON.stringify(data));
}

main()
