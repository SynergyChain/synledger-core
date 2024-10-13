/**
 * @file governance.hpp
 * @brief Governance module for managing decentralized decision-making in SynLedger.
 *
 * This header defines the Governance class, which allows participants to propose, vote on, and finalize
 * proposals within the SynLedger blockchain. It integrates closely with the PoSygEngine to ensure that
 * voting is linked to participant contributions and synergy scores. The governance system is designed
 * to enable decentralized, democratic control over the future development and rules of the network.
 */

#ifndef GOVERNANCE_HPP
#define GOVERNANCE_HPP

#include <string>
#include <vector>
#include "../consensus/posyg_engine.hpp"

/**
 * @struct Proposal
 * @brief Represents a governance proposal that can be voted on by network participants.
 *
 * Each proposal consists of a description, vote counts, and an active status. Participants can cast votes 
 * for or against the proposal, and the governance system will track these votes until the proposal is finalized.
 */
struct Proposal {
    int id;                  ///< Unique identifier for the proposal.
    std::string description; ///< A detailed description of the proposal.
    double votes_for;         ///< Number of votes in favor of the proposal.
    double votes_against;     ///< Number of votes against the proposal.
    bool is_active;           ///< Indicates whether the proposal is still active and open for voting.
};

/**
 * @class Governance
 * @brief Implements decentralized governance functionality for SynLedger.
 *
 * The Governance class allows participants to create proposals, vote on them, and finalize decisions. 
 * Each proposal is tracked and processed based on the votes cast by participants. This class ensures 
 * that network changes or updates are made democratically through a decentralized voting system.
 */
class Governance {
public:
    /**
     * @brief Constructs a Governance object linked to the PoSygEngine.
     * 
     * Initializes the governance system and prepares it to handle proposals and voting. The PoSygEngine 
     * is passed as a reference to access participant data for vote weighting and validation.
     * 
     * @param posyg_engine A reference to the PoSygEngine that tracks participant synergy and behavior.
     */
    Governance(PoSygEngine& posyg_engine);

    /**
     * @brief Creates a new proposal for the network.
     * 
     * Adds a new proposal with a given description to the list of active proposals. This allows 
     * participants to vote on important network decisions.
     * 
     * @param description The description or objective of the proposal.
     */
    void create_proposal(const std::string& description);

    /**
     * @brief Casts a vote for or against a specific proposal.
     * 
     * This method allows a participant to vote on an active proposal. The weight of the vote can be linked 
     * to the participant's synergy or contribution to the network, ensuring that higher-contributing members 
     * have more influence.
     * 
     * @param proposal_id The ID of the proposal being voted on.
     * @param vote_for A boolean indicating whether the vote is in favor of the proposal (true) or against it (false).
     * @param participant_id The ID of the participant casting the vote.
     */
    void vote(int proposal_id, bool vote_for, size_t participant_id);

    /**
     * @brief Finalizes a proposal once voting is complete.
     * 
     * Closes the voting process for a proposal and finalizes its outcome. Depending on the vote tally, 
     * the proposal may be approved or rejected, and the decision is implemented accordingly.
     * 
     * @param proposal_id The ID of the proposal to finalize.
     */
    void finalize_proposal(int proposal_id);

    /**
     * @brief Retrieves a list of all active proposals.
     * 
     * Returns the currently active proposals that are still open for voting.
     * 
     * @return A vector of active proposals.
     */
    std::vector<Proposal> get_active_proposals() const;

    /**
     * @brief Checks if a specific proposal has been approved.
     * 
     * Determines whether the proposal has passed based on the voting results.
     * 
     * @param proposal_id The ID of the proposal to check.
     * @return True if the proposal has been approved, false otherwise.
     */
    bool is_proposal_approved(int proposal_id) const;

    /**
     * @brief Retrieves a proposal by its ID.
     * 
     * Returns a pointer to the proposal with the specified ID. This version of the method allows 
     * for modification of the proposal.
     * 
     * @param proposal_id The ID of the proposal to retrieve.
     * @return A pointer to the Proposal if found, otherwise nullptr.
     */
    Proposal* get_proposal_by_id(int proposal_id);

    /**
     * @brief Retrieves a proposal by its ID (const version).
     * 
     * Returns a pointer to the proposal with the specified ID. This version of the method does not 
     * allow modification of the proposal.
     * 
     * @param proposal_id The ID of the proposal to retrieve.
     * @return A const pointer to the Proposal if found, otherwise nullptr.
     */
    const Proposal* get_proposal_by_id(int proposal_id) const;

private:
    std::vector<Proposal> proposals;   ///< List of all proposals in the governance system.
    int next_proposal_id;              ///< Tracks the next available proposal ID.
    PoSygEngine& posyg_engine;         ///< Reference to the PoSygEngine for accessing participant synergy and votes.
};

#endif // GOVERNANCE_HPP

/**
 * @file governance.hpp
 * 
 * This module implements decentralized governance for SynLedger, enabling participants to influence 
 * network changes democratically. The use of synergy-weighted voting ensures that participants who 
 * contribute more to the network have greater influence. The system also allows for the transparent 
 * creation and management of proposals, making the decision-making process fair and decentralized.
 */
