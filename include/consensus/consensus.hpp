#ifndef CONSENSUS_HPP
#define CONSENSUS_HPP

#include <vector>
#include <cstddef>
#include <chrono>
#include <atomic>
#include <mutex>
#include "../ledger/block.hpp"
#include "../network/p2p_protocol.hpp"
#include "posyg_engine.hpp"
#include "../ledger/ledger.hpp"

class Consensus {
protected:
    size_t num_validators;
    std::vector<size_t> validators;
    Block current_block;
    P2PProtocol& p2p_network;
    PoSygEngine& posyg_engine;
    Ledger& ledger;
    double slashing_penalty;
    double reward_for_validators;

    void slash_validator(size_t validator_id);
    void distribute_rewards();
    void handle_multisig(const Block& block);
    void dynamic_network_management();

public:
    Consensus(size_t num_validators, P2PProtocol& network, PoSygEngine& posyg_engine, Ledger& ledger);
    ~Consensus();

    void initiate_consensus();
    Block create_new_block();
    bool validate_block(const Block& block);
    void finalize_block(const Block& block);
    void validate_and_slash();
};

#endif  // CONSENSUS_HPP
