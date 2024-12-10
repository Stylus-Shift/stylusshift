require("@nomicfoundation/hardhat-toolbox");
require("@nomicfoundation/hardhat-ethers");
require("dotenv").config();

const { ARBITRUM_URL, PRIVATE_KEY } = process.env


module.exports = {
  solidity: "0.8.0",
  networks: {
    arbitrumSepolia: {
      url: process.env.ARBITRUM_URL,
      chainId: 421614,
      accounts: [process.env.PRIVATE_KEY],
   },
   arbitrumStylus: {
    url: "https://sepolia-rollup.arbitrum.io/rpc",
    accounts: [process.env.PRIVATE_KEY],
  },
 },
}

