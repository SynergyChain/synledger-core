#include "consensus/consensus.hpp"
#include "ledger/block.hpp"
#include "network/p2p_protocol.hpp"
#include <iostream>
#include <omp.h>
#include <mutex>
#include <atomic>
#include <chrono>

Consensus::Consensus(size_t num_validators, P2PProtocol& network, PoSygEngine& posyg_engine, Ledger& ledger)
    : num_validators(num_validators), current_block(0, std::string(""), 2), 
      p2p_network(network), posyg_engine(posyg_engine), ledger(ledger),
      slashing_penalty(100.0), reward_for_validators(50.0) {
    for (size_t i = 0; i < num_validators; ++i) {
        validators.push_back(i);
    }
}

Consensus::~Consensus() {}

void Consensus::initiate_consensus() {
    std::cout << "Initiating consensus with " << num_validators << " validators." << std::endl;
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    dynamic_network_management();

    Block new_block = create_new_block();

    if (validate_block(new_block)) {
        handle_multisig(new_block);
        finalize_block(new_block);
    } else {
        std::cout << "Block validation failed!" << std::endl;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> block_time = end_time - start_time;
    std::cout << "Block time: " << block_time.count() << " seconds" << std::endl;

    validate_and_slash();
    distribute_rewards();
}

Block Consensus::create_new_block() {
    Block new_block(ledger.get_blockchain_length(), ledger.get_latest_block().get_block_hash(), 2);
    std::cout << "Creating new block: " << new_block.get_block_number() << std::endl;

    return new_block;
}

bool Consensus::validate_block(const Block& block) {
    std::cout << "Validating block: " << block.get_block_number() << std::endl;

    if (block.get_previous_block_hash().empty()) {
        std::cout << "Invalid block: empty previous block hash!" << std::endl;
        return false;
    }

    if (block.get_block_hash().empty()) {
        std::cout << "Invalid block: empty block hash!" << std::endl;
        return false;
    }

    return true;
}

void Consensus::handle_multisig(const Block& block) {
    Block block_copy = block;
    std::mutex mtx;
    std::atomic<bool> block_signed(false);

    std::cout << "Starting signature collection for block: " << block_copy.get_block_number() << std::endl;

    #pragma omp parallel for
    for (size_t i = 0; i < num_validators; ++i) {
        if (!block_signed.load()) {
            std::string validator_signature = "Signature_" + std::to_string(i);

            {
                std::lock_guard<std::mutex> lock(mtx);
                bool is_signed = block_copy.sign_block(validator_signature);
                if (!is_signed) {
                    std::cout << "Block already has enough signatures. Breaking loop." << std::endl;
                    block_signed.store(true);
                }
            }
        }
    }

    std::cout << "Signature collection completed for block: " << block_copy.get_block_number() << std::endl;

    if (block_copy.verify_signatures()) {
        std::cout << "Block " << block_copy.get_block_number() << " verified with enough signatures." << std::endl;
    } else {
        std::cout << "Block " << block_copy.get_block_number() << " does not have enough signatures." << std::endl;
    }
}

void Consensus::finalize_block(const Block& block) {
    current_block = block;
    p2p_network.send_message(0, "Finalized block with hash: " + block.get_block_hash());
    std::cout << "Finalized block: " << block.get_block_number() << " with hash: " << block.get_block_hash() << std::endl;
}

void Consensus::slash_validator(size_t validator_id) {
    Participant& participant = posyg_engine.get_participant(validator_id);
    participant.apply_slash();
    std::cout << "Validator " << validator_id << " slashed." << std::endl;
}

void Consensus::validate_and_slash() {
    #pragma omp parallel for
    for (size_t i = 0; i < num_validators; ++i) {
        Participant& participant = posyg_engine.get_participant(validators[i]);
        if (participant.detect_suspicious_behavior()) {
            slash_validator(validators[i]);
        }
    }
}

void Consensus::distribute_rewards() {
    std::cout << "Distributing rewards to validators." << std::endl;

    #pragma omp parallel for
    for (size_t i = 0; i < num_validators; ++i) {
        Participant& participant = posyg_engine.get_participant(validators[i]);
        participant.reward += reward_for_validators;
    }
}

void Consensus::dynamic_network_management() {
    std::cout << "Adjusting network parameters dynamically." << std::endl;
    slashing_penalty *= 1.05;
    reward_for_validators *= 1.02;
}
