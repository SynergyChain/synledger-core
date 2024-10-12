#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <vector>
#include <string>
#include <ctime>
#include "../cryptography/crypto.hpp"

enum class TransactionType {
    STANDARD_PAYMENT,
    GOVERNANCE,
    SMART_CONTRACT_EXECUTION
};

struct Transaction {
    std::string sender;
    std::string receiver;
    double amount;
    std::string signature;
    TransactionType type;
    std::string data;

    Transaction(const std::string& sender, const std::string& receiver, double amount, const std::string& signature, TransactionType type, const std::string& data = "");

    bool verify_transaction() const;
    std::string serialize() const;
    static Transaction deserialize(const std::string& serialized_transaction);
};

class Block {
private:
    size_t block_number;
    std::string previous_block_hash;
    std::time_t timestamp;
    std::vector<Transaction> transactions;
    mutable std::string block_hash;
    std::vector<std::string> validator_signatures;
    size_t required_signatures;

public:
    Block();
    Block(size_t block_number, const std::string& previous_block_hash, size_t required_signatures);

    void add_transaction(const Transaction& tx);
    const std::string& calculate_block_hash() const;
    bool sign_block(const std::string& validator_signature);
    bool verify_signatures() const;

    std::string serialize() const;
    static Block deserialize(const std::string& serialized_block);

    const std::vector<Transaction>& get_transactions() const;
    const std::string& get_block_hash() const;
    const std::string& get_previous_block_hash() const;
    size_t get_block_number() const;
    size_t get_signature_count() const;
};

#endif  // BLOCK_HPP
