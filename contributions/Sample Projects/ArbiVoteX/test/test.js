const { expect } = require("chai");
const { ethers } = require("hardhat");

describe("VotingSystem", function () {
    let VotingSystem, votingSystem, owner, voter1, voter2;

    beforeEach(async function () {
        [owner, voter1, voter2] = await ethers.getSigners();
        VotingSystem = await ethers.getContractFactory("VotingSystem");
        votingSystem = await VotingSystem.deploy();
        await votingSystem.deployed();
    });

    it("should create a proposal", async function () {
        await votingSystem.createProposal("Proposal 1");
        const proposal = await votingSystem.getProposal(0);
        expect(proposal.description).to.equal("Proposal 1");
        expect(proposal.voteCount).to.equal(0);
        expect(proposal.isFinalized).to.equal(false);
    });

    it("should allow voting on a proposal", async function () {
        await votingSystem.createProposal("Proposal 1");
        await votingSystem.connect(voter1).vote(0);
        const proposal = await votingSystem.getProposal(0);
        expect(proposal.voteCount).to.equal(1);
    });

    it("should finalize a proposal", async function () {
        await votingSystem.createProposal("Proposal 1");
        await votingSystem.finalizeProposal(0);
        const proposal = await votingSystem.getProposal(0);
        expect(proposal.isFinalized).to.equal(true);
    });

    it("should not allow double voting", async function () {
        await votingSystem.createProposal("Proposal 1");
        await votingSystem.connect(voter1).vote(0);
        await expect(votingSystem.connect(voter1).vote(0)).to.be.revertedWith("Already voted.");
    });
});
