#include "ledger/block.hpp"
#include "cryptography/crypto.hpp"
#include <stdexcept>
#include <sstream>

Transaction::Transaction(const std::string& sender, const std::string& receiver, double amount, const std::string& signature, TransactionType type, const std::string& data)
    : sender(sender), receiver(receiver), amount(amount), signature(signature), type(type), data(data) {}

bool Transaction::verify_transaction() const {
    return Crypto::verify_signature(sender, signature, sender);
}

std::string Transaction::serialize() const {
    std::ostringstream oss;
    oss << sender << "|" << receiver << "|" << amount << "|" << signature << "|" << static_cast<int>(type) << "|" << data;
    return oss.str();
}

Transaction Transaction::deserialize(const std::string& serialized_transaction) {
    std::istringstream iss(serialized_transaction);
    std::string sender, receiver, signature, data;
    double amount;
    int type;
    char delimiter;

    std::getline(iss, sender, '|');
    std::getline(iss, receiver, '|');
    iss >> amount >> delimiter;
    std::getline(iss, signature, '|');
    iss >> type >> delimiter;
    std::getline(iss, data);

    return Transaction(sender, receiver, amount, signature, static_cast<TransactionType>(type), data);
}

Block::Block() : block_number(0), previous_block_hash(""), timestamp(std::time(nullptr)), required_signatures(0) {}

Block::Block(size_t block_number, const std::string& previous_block_hash, size_t required_signatures)
    : block_number(block_number), previous_block_hash(previous_block_hash), timestamp(std::time(nullptr)), required_signatures(required_signatures) {}

void Block::add_transaction(const Transaction& tx) {
    if (tx.verify_transaction()) {
        transactions.push_back(tx);
        calculate_block_hash();
    } else {
        throw std::invalid_argument("Invalid transaction signature");
    }
}

const std::string& Block::calculate_block_hash() const {
    std::string block_content = previous_block_hash + std::to_string(timestamp);

    for (const auto& tx : transactions) {
        block_content += tx.serialize();
    }

    block_hash = Crypto::hash(block_content);
    return block_hash;
}

bool Block::sign_block(const std::string& validator_signature) {
    if (validator_signatures.size() < required_signatures) {
        validator_signatures.push_back(validator_signature);
        return true;
    }
    return false;
}

bool Block::verify_signatures() const {
    return validator_signatures.size() >= required_signatures;
}

std::string Block::serialize() const {
    std::ostringstream oss;
    oss << block_number << "|" << previous_block_hash << "|" << timestamp << "|" << required_signatures << "|";
    for (const auto& tx : transactions) {
        oss << tx.serialize() << "#";
    }
    return oss.str();
}

Block Block::deserialize(const std::string& serialized_block) {
    std::istringstream iss(serialized_block);
    size_t block_number, required_signatures;
    std::string previous_block_hash, tx_serialized;
    std::time_t timestamp;
    char delimiter;

    iss >> block_number >> delimiter;
    std::getline(iss, previous_block_hash, '|');
    iss >> timestamp >> delimiter >> required_signatures >> delimiter;

    Block block(block_number, previous_block_hash, required_signatures);
    block.timestamp = timestamp;

    while (std::getline(iss, tx_serialized, '#')) {
        if (!tx_serialized.empty()) {
            block.transactions.push_back(Transaction::deserialize(tx_serialized));
        }
    }

    block.calculate_block_hash();
    return block;
}

const std::vector<Transaction>& Block::get_transactions() const {
    return transactions;
}

const std::string& Block::get_block_hash() const {
    return block_hash;
}

const std::string& Block::get_previous_block_hash() const {
    return previous_block_hash;
}

size_t Block::get_block_number() const {
    return block_number;
}

size_t Block::get_signature_count() const {
    return validator_signatures.size();
}
