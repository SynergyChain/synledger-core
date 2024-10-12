#include "governance/governance.hpp"
#include <iostream>

Governance::Governance(PoSygEngine& posyg_engine) 
    : next_proposal_id(1), posyg_engine(posyg_engine) {}

void Governance::create_proposal(const std::string& description) {
    Proposal new_proposal;
    new_proposal.id = next_proposal_id++;
    new_proposal.description = description;
    new_proposal.votes_for = 0.0;
    new_proposal.votes_against = 0.0;
    new_proposal.is_active = true;
    proposals.push_back(new_proposal);
    std::cout << "Proposal created: " << new_proposal.description << " (ID: " << new_proposal.id << ")" << std::endl;
}

void Governance::vote(int proposal_id, bool vote_for, size_t participant_id) {
    Proposal* proposal = get_proposal_by_id(proposal_id);
    Participant& participant = posyg_engine.get_participant(participant_id);

    if (proposal && proposal->is_active && !participant.slashed) {
        double vote_weight = participant.synergy;

        if (vote_for) {
            proposal->votes_for += vote_weight;
        } else {
            proposal->votes_against += vote_weight;
        }

        std::cout << "Vote recorded for proposal ID " << proposal_id 
                  << " with weight: " << vote_weight << " (" << (vote_for ? "For" : "Against") << ")" << std::endl;
    } else {
        std::cerr << "Error: Proposal not found, voting has ended, or participant is slashed." << std::endl;
    }
}

void Governance::finalize_proposal(int proposal_id) {
    Proposal* proposal = get_proposal_by_id(proposal_id);

    if (proposal && proposal->is_active) {
        proposal->is_active = false;
        std::cout << "Voting closed for proposal ID " << proposal_id << std::endl;

        if (proposal->votes_for > proposal->votes_against) {
            std::cout << "Proposal " << proposal_id << " has been approved." << std::endl;
        } else {
            std::cout << "Proposal " << proposal_id << " has been rejected." << std::endl;
        }
    } else {
        std::cerr << "Error: Proposal not found or already finalized." << std::endl;
    }
}

std::vector<Governance::Proposal> Governance::get_active_proposals() const {
    std::vector<Proposal> active_proposals;
    for (const auto& proposal : proposals) {
        if (proposal.is_active) {
            active_proposals.push_back(proposal);
        }
    }
    return active_proposals;
}

bool Governance::is_proposal_approved(int proposal_id) const {
    const Proposal* proposal = get_proposal_by_id(proposal_id);
    if (proposal && !proposal->is_active) {
        return proposal->votes_for > proposal->votes_against;
    }
    return false;
}

Governance::Proposal* Governance::get_proposal_by_id(int proposal_id) {
    for (auto& proposal : proposals) {
        if (proposal.id == proposal_id) {
            return &proposal;
        }
    }
    return nullptr;
}

const Governance::Proposal* Governance::get_proposal_by_id(int proposal_id) const {
    for (const auto& proposal : proposals) {
        if (proposal.id == proposal_id) {
            return &proposal;
        }
    }
    return nullptr;
}
