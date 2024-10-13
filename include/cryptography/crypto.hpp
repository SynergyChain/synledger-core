/**
 * @file crypto.hpp
 * @brief Cryptographic utilities for hashing, signing, and signature verification in SynLedger.
 *
 * This header defines the `Crypto` class, which provides cryptographic primitives crucial for maintaining 
 * data integrity and security in the SynLedger blockchain. Functions include hashing for data integrity, 
 * digital signatures for transaction validation, and signature verification for authentication. These cryptographic 
 * operations ensure that data cannot be altered or forged, supporting the secure and trustless nature of the network.
 */

#ifndef CRYPTO_HPP
#define CRYPTO_HPP

#include <string>

/**
 * @class Crypto
 * @brief Provides core cryptographic functionalities used in SynLedger.
 *
 * The `Crypto` class offers static methods to perform essential cryptographic operations such as hashing data, 
 * generating digital signatures, and verifying the authenticity of signatures. These functions are integral to 
 * ensuring that all transactions and communications in the blockchain remain secure, tamper-proof, and verifiable.
 */
class Crypto {
public:
    /**
     * @brief Generates a cryptographic hash of the given data.
     * 
     * This method computes a secure hash (typically using SHA-256 or similar) to ensure data integrity. 
     * Hashing is crucial for verifying that data (such as block contents) have not been tampered with.
     * 
     * @param data The input data to be hashed.
     * @return A string representing the hexadecimal hash of the input data.
     */
    static std::string hash(const std::string& data);

    /**
     * @brief Signs a message using a private key.
     * 
     * This method generates a digital signature for the given message using the provided private key. 
     * The signature can be used to verify the authenticity and integrity of the message, ensuring that it originated 
     * from the holder of the corresponding private key and has not been altered.
     * 
     * @param message The message to be signed.
     * @param private_key The private key used to sign the message.
     * @return A string representing the digital signature.
     */
    static std::string sign(const std::string& message, const std::string& private_key);

    /**
     * @brief Verifies the authenticity of a signature.
     * 
     * This method checks whether a given signature is valid for the provided message and public key. 
     * Signature verification ensures that the message was signed by the owner of the corresponding private key 
     * and has not been tampered with, providing authenticity and integrity guarantees.
     * 
     * @param message The original message that was signed.
     * @param signature The digital signature to verify.
     * @param public_key The public key corresponding to the private key that signed the message.
     * @return True if the signature is valid, false otherwise.
     */
    static bool verify_signature(const std::string& message, const std::string& signature, const std::string& public_key);
};

#endif // CRYPTO_HPP

/**
 * @file crypto.hpp
 * 
 * This module implements fundamental cryptographic operations critical to the security of the SynLedger network. 
 * By employing standard techniques like hashing, digital signatures, and signature verification, it ensures 
 * that all transactions and data exchanges are tamper-resistant and authentic. The secure design guarantees that 
 * data integrity is preserved across the decentralized environment, preventing fraud and unauthorized access.
 */
