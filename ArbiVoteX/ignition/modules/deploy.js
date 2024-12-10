const { ethers } = require("hardhat");
const fs = require("fs");

async function main() {
    // Deploy Solidity contract
    const Voting = await ethers.getContractFactory("VotingSystem");
    const voting = await Voting.deploy();
    await voting.deployed();
    console.log("VotingSystem deployed to:", voting.address);

    // Deploy the Wasm contract
    const wasmFile = fs.readFileSync("../path/to/your/deployment/folder/quadratic_voting.wasm");
    const wasmContract = await ethers.getContractFactory("YourWasmContract"); // Replace with the appropriate contract factory
    const wasmContractInstance = await wasmContract.deploy(wasmFile);
    await wasmContractInstance.deployed();
    console.log("QuadraticVoting contract deployed to:", wasmContractInstance.address);
}

main()
    .then(() => process.exit(0))
    .catch((error) => {
        console.error(error);
        process.exit(1);
    });
