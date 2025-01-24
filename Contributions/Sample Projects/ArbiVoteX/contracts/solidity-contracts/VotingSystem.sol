// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

interface IQuadraticVotingWasm {
    function quadratic_vote_count(uint votes) external returns (uint);
}

contract VotingSystem {
    struct Proposal {
        uint id;
        string description;
        uint voteCount;
        bool isFinalized;
    }

    mapping(uint => Proposal) public proposals;
    uint public nextProposalId;
    mapping(uint => mapping(address => bool)) public hasVoted;

    IQuadraticVotingWasm quadraticVotingWasm;

    event ProposalCreated(uint id, string description);
    event Voted(uint proposalId, address voter);
    event ProposalFinalized(uint id, uint voteCount);

    constructor(address _wasmAddress) {
        quadraticVotingWasm = IQuadraticVotingWasm(_wasmAddress);
    }

    function createProposal(string memory _description) public {
        Proposal memory newProposal = Proposal({
            id: nextProposalId,
            description: _description,
            voteCount: 0,
            isFinalized: false
        });
        proposals[nextProposalId] = newProposal;
        emit ProposalCreated(nextProposalId, _description);
        nextProposalId++;
    }

    function vote(uint proposalId) public {
        require(proposals[proposalId].id == proposalId, "Proposal does not exist.");
        require(!hasVoted[proposalId][msg.sender], "Already voted.");
        require(!proposals[proposalId].isFinalized, "Proposal is finalized.");

        proposals[proposalId].voteCount++;
        hasVoted[proposalId][msg.sender] = true;
        emit Voted(proposalId, msg.sender);
    }

    function finalizeProposal(uint proposalId) public {
        require(proposals[proposalId].id == proposalId, "Proposal does not exist.");
        require(!proposals[proposalId].isFinalized, "Proposal is already finalized.");

        // Call the Wasm module's quadratic voting calculation
        proposals[proposalId].voteCount = quadraticVotingWasm.quadratic_vote_count(proposals[proposalId].voteCount);

        proposals[proposalId].isFinalized = true;
        emit ProposalFinalized(proposalId, proposals[proposalId].voteCount);
    }

    function getProposal(uint proposalId) public view returns (string memory description, uint voteCount, bool isFinalized) {
        Proposal memory proposal = proposals[proposalId];
        return (proposal.description, proposal.voteCount, proposal.isFinalized);
    }
}
