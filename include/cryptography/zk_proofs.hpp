/**
 * @file zk_proofs.hpp
 * @brief Zero-Knowledge Proofs module for SynLedger.
 *
 * This header defines the ZKProofs class, which provides functions for generating and verifying 
 * zero-knowledge proofs (ZKPs). Zero-knowledge proofs allow one party to prove to another that they 
 * know a value (the witness) without revealing any information about the value itself. This cryptographic 
 * technique ensures privacy and security in transactions while allowing verifiable integrity.
 */

#ifndef ZK_PROOFS_HPP
#define ZK_PROOFS_HPP

#include <string>

/**
 * @class ZKProofs
 * @brief Implements zero-knowledge proof generation and verification.
 *
 * The `ZKProofs` class provides the core functionality for generating and verifying zero-knowledge proofs, 
 * allowing parties to prove knowledge of a secret or witness without revealing it. These proofs are critical 
 * for maintaining privacy in blockchain systems while ensuring that the integrity of transactions or statements 
 * can still be verified.
 */
class ZKProofs {
public:
    /**
     * @brief Generates a zero-knowledge proof.
     * 
     * This method generates a zero-knowledge proof for a given statement and witness. The proof allows the 
     * prover to convince a verifier that they know the witness corresponding to the statement without revealing 
     * the actual witness value.
     * 
     * @param statement The public statement being proven.
     * @param witness The secret value (witness) known by the prover.
     * @return A string representing the generated zero-knowledge proof.
     */
    static std::string generate_proof(const std::string& statement, const std::string& witness);

    /**
     * @brief Verifies a zero-knowledge proof.
     * 
     * This method verifies the correctness of a zero-knowledge proof. It checks whether the proof corresponds 
     * to the provided statement and witness, ensuring that the prover indeed knows the witness without 
     * having revealed it.
     * 
     * @param statement The public statement being proven.
     * @param proof The zero-knowledge proof to verify.
     * @param witness The secret value (witness) that is being verified.
     * @return True if the proof is valid, false otherwise.
     */
    static bool verify_proof(const std::string& statement, const std::string& proof, const std::string& witness);

    /**
     * @brief Provides an example of zero-knowledge proof usage.
     * 
     * This function demonstrates how to generate and verify a zero-knowledge proof using the ZKProofs class.
     * It serves as a simple example for developers looking to understand the ZKP flow.
     * 
     * @return A string demonstrating an example usage of zero-knowledge proofs.
     */
    static std::string example_usage();
};

#endif // ZK_PROOFS_HPP

/**
 * @file zk_proofs.hpp
 * 
 * This module provides the core functionality for implementing zero-knowledge proofs (ZKPs), which are 
 * fundamental to ensuring privacy in decentralized systems like SynLedger. ZKPs enable secure transactions 
 * where parties can prove knowledge of secret information without revealing the information itself, ensuring 
 * both security and confidentiality. These proofs are particularly important in scenarios involving sensitive 
 * data or privacy-preserving computations.
 */
