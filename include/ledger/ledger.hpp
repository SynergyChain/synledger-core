/**
 * @file ledger.hpp
 * @brief Ledger module for SynLedger's blockchain.
 *
 * This header defines the Ledger class, responsible for managing the entire blockchain, including the main chain 
 * and potential forks. It maintains the integrity of the blockchain by validating new blocks, handling forks, 
 * and confirming transactions. The ledger also supports rollback functionality in case of reorganization and 
 * maintains a pool of pending transactions.
 */

#ifndef LEDGER_HPP
#define LEDGER_HPP

#include <vector>
#include <string>
#include <map>
#include "block.hpp"  // Includes the block structure for managing the blockchain.

/**
 * @class Ledger
 * @brief Manages the blockchain, forks, and transaction pool.
 *
 * The Ledger class represents the core of the blockchain, handling blocks, forks, and transaction validation. 
 * It ensures that the longest valid chain is maintained, allowing for fork resolution, block confirmation, 
 * and handling of pending transactions. This class is essential for maintaining the integrity and security 
 * of the blockchain.
 */
class Ledger {
private:
    std::vector<Block> chain;                        ///< The main blockchain, a sequence of validated blocks.
    size_t difficulty;                               ///< The difficulty level for mining/validation.
    size_t current_block_number;                     ///< The current height of the blockchain.
    std::map<std::string, std::vector<Block>> forks; ///< Tracks alternate chains (forks) in case of disagreements.
    std::string current_chain_tip_hash;              ///< The hash of the latest block in the main chain.
    std::map<std::string, size_t> fork_lengths;      ///< The length of each fork chain.
    std::map<std::string, size_t> fork_difficulties; ///< The difficulty levels for each fork chain.
    std::map<std::string, bool> confirmed_blocks;    ///< Tracks which blocks have been confirmed.
    std::vector<Transaction> transaction_pool;       ///< A pool of pending transactions awaiting inclusion in a block.

    /**
     * @brief Calculates the genesis block's hash.
     * 
     * Generates the cryptographic hash for the first block in the blockchain, ensuring a secure foundation.
     * 
     * @return The genesis block hash.
     */
    std::string calculate_genesis_block_hash();

    /**
     * @brief Calculates the Merkle root of the transactions in a block.
     * 
     * The Merkle root ensures the integrity of all transactions in a block. It provides a summary 
     * hash derived from all the transactions.
     * 
     * @param transactions The list of transactions in the block.
     * @return The Merkle root of the transactions.
     */
    std::string calculate_merkle_root(const std::vector<Transaction>& transactions);

    /**
     * @brief Prunes outdated or invalid forks.
     * 
     * Removes forks that are no longer viable, helping to keep the ledger lean and focused on valid chains.
     */
    void prune_forks();

public:
    /**
     * @brief Constructs a Ledger with an initial difficulty level.
     * 
     * Initializes the ledger and sets the starting difficulty for block validation.
     * 
     * @param initial_difficulty The difficulty level for block validation.
     */
    Ledger(size_t initial_difficulty);

    /**
     * @brief Adds a block to the main chain.
     * 
     * Validates and appends a new block to the blockchain.
     * 
     * @param block The block to be added.
     */
    void add_block(const Block& block);

    /**
     * @brief Adds a block to a fork.
     * 
     * In the case of a chain split, adds a block to the appropriate fork.
     * 
     * @param fork_tip The hash of the tip of the fork where the block will be added.
     * @param block The block to add to the fork.
     */
    void add_fork_block(const std::string& fork_tip, const Block& block);

    /**
     * @brief Retrieves the latest block in the main chain.
     * 
     * @return The latest block.
     */
    const Block& get_latest_block() const;

    /**
     * @brief Returns the entire main chain.
     * 
     * Provides access to the sequence of blocks that form the current main blockchain.
     * 
     * @return A vector of blocks representing the main chain.
     */
    const std::vector<Block>& get_chain() const;

    /**
     * @brief Retrieves the list of forks.
     * 
     * @return A map of forks, each represented by a chain of blocks.
     */
    const std::map<std::string, std::vector<Block>>& get_forks() const;

    /**
     * @brief Validates the integrity of the main blockchain.
     * 
     * Ensures that all blocks in the main chain are valid and that the chain has not been tampered with.
     * 
     * @return True if the chain is valid, false otherwise.
     */
    bool validate_chain() const;

    /**
     * @brief Validates a specific fork.
     * 
     * Ensures that the blocks in the fork are valid and consistent.
     * 
     * @param fork_tip The hash of the tip of the fork to validate.
     * @return True if the fork is valid, false otherwise.
     */
    bool validate_fork(const std::string& fork_tip) const;

    /**
     * @brief Rolls back the main chain by a specific number of blocks.
     * 
     * Useful in the case of a fork or to undo a recent chain reorganization.
     * 
     * @param blocks_to_rollback The number of blocks to remove from the main chain.
     * @return True if the rollback was successful, false otherwise.
     */
    bool rollback_chain(size_t blocks_to_rollback);

    /**
     * @brief Confirms a block as part of the finalized chain.
     * 
     * Marks a block as confirmed, meaning it is part of the final, trusted chain.
     * 
     * @param block The block to confirm.
     * @return True if the block was successfully confirmed.
     */
    bool confirm_block(const Block& block);

    /**
     * @brief Returns the current length of the blockchain.
     * 
     * @return The length of the main chain.
     */
    size_t get_blockchain_length() const;

    /**
     * @brief Logs the current state of the blockchain.
     * 
     * Outputs the state of the chain and any forks, useful for debugging or auditing.
     */
    void log_chain_state() const;

    /**
     * @brief Switches the main chain to a selected fork.
     * 
     * If a fork has outpaced the main chain, this method allows the ledger to switch to the fork.
     * 
     * @param fork_tip The hash of the fork tip to switch to.
     * @return True if the switch was successful, false otherwise.
     */
    bool select_fork(const std::string& fork_tip);

    /**
     * @brief Sets a block's confirmation status.
     * 
     * Updates whether a block is marked as confirmed.
     * 
     * @param block_hash The hash of the block.
     * @param confirmed The confirmation status (true for confirmed, false for unconfirmed).
     */
    void set_block_confirmation(const std::string& block_hash, bool confirmed);

    /**
     * @brief Checks if a block is confirmed.
     * 
     * @param block_hash The hash of the block.
     * @return True if the block is confirmed, false otherwise.
     */
    bool is_block_confirmed(const std::string& block_hash) const;

    /**
     * @brief Adds a transaction to the pool of pending transactions.
     * 
     * Stores a transaction until it is included in a block.
     * 
     * @param tx The transaction to add to the pool.
     */
    void add_transaction(const Transaction& tx);

    /**
     * @brief Checks if there are pending transactions in the pool.
     * 
     * @return True if there are pending transactions, false otherwise.
     */
    bool has_pending_transactions() const;

    /**
     * @brief Retrieves the list of pending transactions.
     * 
     * Returns all transactions currently awaiting inclusion in a block.
     * 
     * @return A vector of pending transactions.
     */
    std::vector<Transaction> get_pending_transactions() const;
};

#endif // LEDGER_HPP

/**
 * @file ledger.hpp
 * 
 * This module defines the Ledger class, which is responsible for managing the blockchain's state, 
 * including forks and pending transactions. The ledger ensures that blocks are validated, 
 * manages potential forks, and maintains the integrity of the blockchain through confirmation 
 * and validation processes. It provides rollback capabilities to handle reorganizations and 
 * maintains a transaction pool for future block inclusion.
 */
