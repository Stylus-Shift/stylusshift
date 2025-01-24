#[no_mangle]
pub extern "C" fn quadratic_vote_count(votes: u32) -> u32 {
    votes * votes
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_vote_initialization() {
        let mut contract = QuadraticVoting::new();
        contract.create_proposal("Proposal 1".to_string());
        let proposal = contract.get_proposal(0).unwrap();

        assert_eq!(proposal.description, "Proposal 1");
        assert_eq!(proposal.vote_count, 0);
    }

    #[test]
    fn test_cast_vote() {
        let mut contract = QuadraticVoting::new();
        contract.create_proposal("Proposal 1".to_string());

        contract.cast_vote(0, 1);
        let proposal = contract.get_proposal(0).unwrap();

        assert_eq!(proposal.vote_count, 1);
    }

    #[test]
    fn test_cast_multiple_votes_quadratic() {
        let mut contract = QuadraticVoting::new();
        contract.create_proposal("Proposal 1".to_string());

        // Cast multiple votes by the same voter to test quadratic voting behavior
        contract.cast_vote(0, 2);
        let proposal = contract.get_proposal(0).unwrap();

        // With quadratic voting, 2 votes should have an effect of 4 on the proposal
        assert_eq!(proposal.vote_count, 4);
    }

    #[test]
    fn test_vote_limitations() {
        let mut contract = QuadraticVoting::new();
        contract.create_proposal("Proposal 1".to_string());

        // Ensure voters can only vote with positive, reasonable numbers
        let result = contract.cast_vote(0, 0);
        assert!(result.is_err(), "Expected an error for zero votes");

        let result = contract.cast_vote(0, u32::MAX);
        assert!(result.is_err(), "Expected an error for excessively large votes");
    }

    #[test]
    fn test_finalize_proposal() {
        let mut contract = QuadraticVoting::new();
        contract.create_proposal("Proposal 1".to_string());

        contract.finalize_proposal(0);
        let proposal = contract.get_proposal(0).unwrap();

        assert!(proposal.is_finalized, "Proposal should be finalized");
    }
}
