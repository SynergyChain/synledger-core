#include "cryptography/zk_proofs.hpp"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <stdexcept>

std::string ZKProofs::generate_proof(const std::string& statement, const std::string& witness) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;

    if (SHA256_Init(&sha256) != 1) {
        throw std::runtime_error("Failed to initialize SHA256");
    }

    if (SHA256_Update(&sha256, statement.c_str(), statement.size()) != 1) {
        throw std::runtime_error("Failed to update SHA256 with statement");
    }

    if (SHA256_Update(&sha256, witness.c_str(), witness.size()) != 1) {
        throw std::runtime_error("Failed to update SHA256 with witness");
    }

    if (SHA256_Final(hash, &sha256) != 1) {
        throw std::runtime_error("Failed to finalize SHA256");
    }

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

bool ZKProofs::verify_proof(const std::string& statement, const std::string& proof, const std::string& witness) {
    try {
        std::string expected_proof = generate_proof(statement, witness);
        return proof == expected_proof;
    } catch (const std::runtime_error&) {
        return false;
    }
}

std::string ZKProofs::example_usage() {
    std::string statement = "I know a secret number";
    std::string witness = "42";

    try {
        std::string proof = generate_proof(statement, witness);
        bool is_valid = verify_proof(statement, proof, witness);

        std::stringstream result;
        result << "Generated proof: " << proof << "\n";
        result << "Proof verification: " << (is_valid ? "valid" : "invalid") << "\n";
        return result.str();
    } catch (const std::runtime_error& e) {
        return std::string("Error during proof generation or verification: ") + e.what();
    }
}
