#ifndef LEDGER_HPP
#define LEDGER_HPP

#include <vector>
#include <string>
#include <map>
#include "block.hpp"

class Ledger {
private:
    std::vector<Block> chain;
    size_t difficulty;
    size_t current_block_number;
    std::map<std::string, std::vector<Block>> forks;
    std::string current_chain_tip_hash;
    std::map<std::string, size_t> fork_lengths;
    std::map<std::string, size_t> fork_difficulties;
    std::map<std::string, bool> confirmed_blocks;
    std::vector<Transaction> transaction_pool;

    std::string calculate_genesis_block_hash();
    std::string calculate_merkle_root(const std::vector<Transaction>& transactions);
    void prune_forks();

public:
    Ledger(size_t initial_difficulty);

    void add_block(const Block& block);
    void add_fork_block(const std::string& fork_tip, const Block& block);
    const Block& get_latest_block() const;
    const std::vector<Block>& get_chain() const;
    const std::map<std::string, std::vector<Block>>& get_forks() const;
    bool validate_chain() const;
    bool validate_fork(const std::string& fork_tip) const;
    bool rollback_chain(size_t blocks_to_rollback);
    bool confirm_block(const Block& block);
    size_t get_blockchain_length() const;
    void log_chain_state() const;
    bool select_fork(const std::string& fork_tip);
    void set_block_confirmation(const std::string& block_hash, bool confirmed);
    bool is_block_confirmed(const std::string& block_hash) const;

    void add_transaction(const Transaction& tx);
    bool has_pending_transactions() const;
    std::vector<Transaction> get_pending_transactions() const;
};

#endif // LEDGER_HPP
