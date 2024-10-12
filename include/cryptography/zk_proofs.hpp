#ifndef ZK_PROOFS_HPP
#define ZK_PROOFS_HPP

#include <string>

class ZKProofs {
public:
    static std::string generate_proof(const std::string& statement, const std::string& witness);
    static bool verify_proof(const std::string& statement, const std::string& proof, const std::string& witness);
    static std::string example_usage();
};

#endif // ZK_PROOFS_HPP
