#include "ledger/ledger.hpp"
#include "cryptography/crypto.hpp"
#include <iostream>
#include <stdexcept>

Ledger::Ledger(size_t initial_difficulty) 
    : difficulty(initial_difficulty), current_block_number(0) {
    Block genesis_block(0, "0", 1);
    genesis_block.sign_block("Genesis Block Signature");
    genesis_block.calculate_block_hash();
    chain.push_back(genesis_block);
    current_chain_tip_hash = genesis_block.get_block_hash();
}

void Ledger::add_block(const Block& block) {
    if (block.get_previous_block_hash() == current_chain_tip_hash) {
        chain.push_back(block);
        current_block_number++;
        current_chain_tip_hash = block.get_block_hash();
    } else {
        throw std::invalid_argument("Block does not fit the current chain tip!");
    }
}

void Ledger::add_fork_block(const std::string& fork_tip, const Block& block) {
    if (forks.find(fork_tip) == forks.end()) {
        forks[fork_tip] = std::vector<Block>();
        fork_lengths[fork_tip] = 0;
        fork_difficulties[fork_tip] = 0;
    }

    forks[fork_tip].push_back(block);
    fork_lengths[fork_tip] = forks[fork_tip].size();
    fork_difficulties[fork_tip] += difficulty;
}

const Block& Ledger::get_latest_block() const {
    return chain.back();
}

const std::vector<Block>& Ledger::get_chain() const {
    return chain;
}

const std::map<std::string, std::vector<Block>>& Ledger::get_forks() const {
    return forks;
}

bool Ledger::validate_chain() const {
    for (size_t i = 1; i < chain.size(); ++i) {
        const Block& previous_block = chain[i - 1];
        const Block& current_block = chain[i];

        if (current_block.get_previous_block_hash() != previous_block.get_block_hash()) {
            std::cerr << "Block " << current_block.get_block_number() << " has invalid previous block hash!" << std::endl;
            return false;
        }

        if (current_block.get_block_hash() != current_block.calculate_block_hash()) {
            std::cerr << "Block " << current_block.get_block_number() << " has invalid block hash!" << std::endl;
            return false;
        }
    }
    return true;
}

bool Ledger::validate_fork(const std::string& fork_tip) const {
    if (forks.find(fork_tip) == forks.end()) {
        return false;
    }

    const std::vector<Block>& fork = forks.at(fork_tip);
    for (size_t i = 1; i < fork.size(); ++i) {
        const Block& previous_block = fork[i - 1];
        const Block& current_block = fork[i];

        if (current_block.get_previous_block_hash() != previous_block.get_block_hash()) {
            std::cerr << "Fork block " << current_block.get_block_number() << " has invalid previous block hash!" << std::endl;
            return false;
        }

        if (current_block.get_block_hash() != current_block.calculate_block_hash()) {
            std::cerr << "Fork block " << current_block.get_block_number() << " has invalid block hash!" << std::endl;
            return false;
        }
    }
    return true;
}

bool Ledger::rollback_chain(size_t blocks_to_rollback) {
    if (blocks_to_rollback >= chain.size()) {
        return false;
    }

    chain.resize(chain.size() - blocks_to_rollback);
    current_block_number -= blocks_to_rollback;
    current_chain_tip_hash = chain.back().get_block_hash();
    return true;
}

bool Ledger::confirm_block(const Block& block) {
    if (block.get_block_number() <= current_block_number && block.verify_signatures()) {
        std::cout << "Block " << block.get_block_number() << " confirmed." << std::endl;
        return true;
    }
    return false;
}

size_t Ledger::get_blockchain_length() const {
    return chain.size();
}

void Ledger::log_chain_state() const {
    std::cout << "Current blockchain length: " << chain.size() << std::endl;
    std::cout << "Current block number: " << current_block_number << std::endl;
    std::cout << "Current chain tip hash: " << current_chain_tip_hash << std::endl;

    for (const auto& block : chain) {
        std::cout << "Block #" << block.get_block_number() << " | Hash: " << block.get_block_hash() << std::endl;
    }
}

std::string Ledger::calculate_merkle_root(const std::vector<Transaction>& transactions) {
    if (transactions.empty()) {
        return "";
    }

    std::vector<std::string> hashes;
    for (const auto& tx : transactions) {
        hashes.push_back(Crypto::hash(tx.sender + tx.receiver + std::to_string(tx.amount) + tx.signature));
    }

    while (hashes.size() > 1) {
        if (hashes.size() % 2 != 0) {
            hashes.push_back(hashes.back());
        }

        std::vector<std::string> new_hashes;
        for (size_t i = 0; i < hashes.size(); i += 2) {
            new_hashes.push_back(Crypto::hash(hashes[i] + hashes[i + 1]));
        }
        hashes = new_hashes;
    }

    return hashes.front();
}

bool Ledger::select_fork(const std::string& fork_tip) {
    if (forks.find(fork_tip) != forks.end()) {
        const std::vector<Block>& fork_chain = forks[fork_tip];
        chain.insert(chain.end(), fork_chain.begin(), fork_chain.end());
        current_block_number = chain.size();
        current_chain_tip_hash = chain.back().get_block_hash();
        return true;
    }
    return false;
}

void Ledger::set_block_confirmation(const std::string& block_hash, bool confirmed) {
    confirmed_blocks[block_hash] = confirmed;
}

bool Ledger::is_block_confirmed(const std::string& block_hash) const {
    auto it = confirmed_blocks.find(block_hash);
    return it != confirmed_blocks.end() && it->second;
}

void Ledger::prune_forks() {
    for (auto it = forks.begin(); it != forks.end();) {
        if (fork_lengths[it->first] < current_block_number - 10) {
            it = forks.erase(it);
        } else {
            ++it;
        }
    }
}
