#ifndef GOVERNANCE_HPP
#define GOVERNANCE_HPP

#include <string>
#include <vector>
#include "../consensus/posyg_engine.hpp"

class Governance {
public:
    struct Proposal {
        int id;
        std::string description;
        double votes_for;
        double votes_against;
        bool is_active;
    };

    Governance(PoSygEngine& posyg_engine);

    void create_proposal(const std::string& description);
    void vote(int proposal_id, bool vote_for, size_t participant_id);
    void finalize_proposal(int proposal_id);
    std::vector<Proposal> get_active_proposals() const;
    bool is_proposal_approved(int proposal_id) const;

    Proposal* get_proposal_by_id(int proposal_id);
    const Proposal* get_proposal_by_id(int proposal_id) const;

private:
    std::vector<Proposal> proposals;
    int next_proposal_id;
    PoSygEngine& posyg_engine;
};

#endif // GOVERNANCE_HPP
