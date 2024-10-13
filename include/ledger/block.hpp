/**
 * @file block.hpp
 * @brief Block and transaction structures for SynLedger's blockchain.
 *
 * This header defines the `Block` and `Transaction` classes, which represent the fundamental units 
 * of data in the blockchain. Blocks consist of multiple transactions and are signed by validators 
 * to ensure their integrity. Transactions capture interactions between participants, including payments, 
 * governance actions, and smart contract executions. This module ensures that all transactions and blocks 
 * are cryptographically secure and verifiable.
 */

#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <vector>
#include <string>
#include <ctime>
#include "../cryptography/crypto.hpp"  // For hashing and signature verification

/**
 * @enum TransactionType
 * @brief Represents different types of transactions in the SynLedger blockchain.
 *
 * Transactions can be standard payments, governance actions, or the execution of smart contracts.
 */
enum class TransactionType {
    STANDARD_PAYMENT,              ///< Simple value transfer between accounts.
    GOVERNANCE,                    ///< Governance-related transactions (e.g., voting).
    SMART_CONTRACT_EXECUTION       ///< Execution of smart contract code.
};

/**
 * @struct Transaction
 * @brief Represents a transaction in the blockchain.
 *
 * A transaction includes sender and receiver addresses, the amount transferred, and a digital signature
 * that ensures the authenticity and integrity of the transaction. It can also include additional data 
 * for governance or smart contract transactions.
 */
struct Transaction {
    std::string sender;            ///< The address of the transaction's sender.
    std::string receiver;          ///< The address of the transaction's receiver.
    double amount;                 ///< Amount of tokens transferred in the transaction.
    std::string signature;         ///< Digital signature of the transaction to verify its authenticity.
    TransactionType type;          ///< Type of the transaction (payment, governance, smart contract).
    std::string data;              ///< Additional data for governance or smart contract execution.

    /**
     * @brief Constructs a Transaction object.
     * 
     * Initializes a transaction with the necessary details like sender, receiver, amount, and signature.
     * Optionally includes additional data for non-standard transactions.
     * 
     * @param sender The sender's address.
     * @param receiver The receiver's address.
     * @param amount The amount to be transferred.
     * @param signature The digital signature of the transaction.
     * @param type The type of the transaction (STANDARD_PAYMENT, GOVERNANCE, SMART_CONTRACT_EXECUTION).
     * @param data Optional data for governance or smart contract transactions.
     */
    Transaction(const std::string& sender, const std::string& receiver, double amount, const std::string& signature, TransactionType type, const std::string& data = "");

    /**
     * @brief Verifies the validity of the transaction.
     * 
     * Checks if the transaction's signature is valid based on the sender's public key.
     * 
     * @return True if the transaction is valid, false otherwise.
     */
    bool verify_transaction() const;

    /**
     * @brief Serializes the transaction into a string for storage or transmission.
     * 
     * Converts the transaction data into a compact string format for easy storage or transmission.
     * 
     * @return A serialized string representation of the transaction.
     */
    std::string serialize() const;

    /**
     * @brief Deserializes a transaction from a string.
     * 
     * Recreates a transaction object from its serialized string form.
     * 
     * @param serialized_transaction The serialized string of the transaction.
     * @return The deserialized Transaction object.
     */
    static Transaction deserialize(const std::string& serialized_transaction);
};

/**
 * @class Block
 * @brief Represents a block in the SynLedger blockchain.
 *
 * The Block class contains multiple transactions, a hash of the previous block, and signatures from validators.
 * It ensures the integrity of the blockchain through cryptographic hashes and validator signatures, securing the
 * chain from tampering.
 */
class Block {
private:
    size_t block_number;                      ///< The block's position in the blockchain.
    std::string previous_block_hash;          ///< The hash of the previous block in the chain.
    std::time_t timestamp;                    ///< Timestamp of when the block was created.
    std::vector<Transaction> transactions;    ///< List of transactions contained in the block.
    mutable std::string block_hash;           ///< Cached hash of the block.
    std::vector<std::string> validator_signatures; ///< Validator signatures on the block.
    size_t required_signatures;               ///< Number of required validator signatures for block finalization.

public:
    /**
     * @brief Default constructor for an empty Block.
     */
    Block();

    /**
     * @brief Constructs a Block object with a specific block number, previous block hash, and required validator signatures.
     * 
     * @param block_number The block's number in the blockchain.
     * @param previous_block_hash The hash of the previous block.
     * @param required_signatures Number of validator signatures required to finalize the block.
     */
    Block(size_t block_number, const std::string& previous_block_hash, size_t required_signatures);

    /**
     * @brief Adds a transaction to the block.
     * 
     * Inserts a new transaction into the block's transaction list.
     * 
     * @param tx The transaction to add.
     */
    void add_transaction(const Transaction& tx);

    /**
     * @brief Calculates the block's hash.
     * 
     * Computes the cryptographic hash of the block's contents (transactions, previous hash, etc.).
     * 
     * @return The hash of the block.
     */
    const std::string& calculate_block_hash() const;

    /**
     * @brief Adds a validator's signature to the block.
     * 
     * Signs the block with a validator's signature, which contributes to the required number of signatures.
     * 
     * @param validator_signature The signature of the validator.
     * @return True if the signature was added, false otherwise.
     */
    bool sign_block(const std::string& validator_signature);

    /**
     * @brief Verifies that the block has sufficient valid signatures.
     * 
     * Checks that the block has enough validator signatures to be considered finalized.
     * 
     * @return True if the required number of signatures has been met, false otherwise.
     */
    bool verify_signatures() const;

    /**
     * @brief Serializes the block into a string format.
     * 
     * Converts the block and its contents into a string for storage or transmission.
     * 
     * @return A serialized string of the block.
     */
    std::string serialize() const;

    /**
     * @brief Deserializes a block from a string.
     * 
     * Recreates a Block object from its serialized form.
     * 
     * @param serialized_block The serialized string of the block.
     * @return The deserialized Block object.
     */
    static Block deserialize(const std::string& serialized_block);

    // Getters for block details
    const std::vector<Transaction>& get_transactions() const;   ///< Retrieves the list of transactions in the block.
    const std::string& get_block_hash() const;                  ///< Retrieves the block's hash.
    const std::string& get_previous_block_hash() const;         ///< Retrieves the hash of the previous block.
    size_t get_block_number() const;                            ///< Retrieves the block number.
    size_t get_signature_count() const;                         ///< Retrieves the count of validator signatures.
};

#endif  // BLOCK_HPP

/**
 * @file block.hpp
 *
 * This module defines the structures for blocks and transactions within the SynLedger blockchain. 
 * Blocks securely store transactions and are validated by multiple signatures from network validators, 
 * ensuring integrity and consensus. Each block is cryptographically linked to the previous one, forming 
 * an immutable chain of data that prevents tampering. Transactions within the block are also cryptographically 
 * signed, making this module a core component of blockchain security and transparency.
 */
