/**
 * @file consensus.hpp
 * @brief Consensus module for the SynLedger Core.
 *
 * This header file defines the Consensus class, responsible for managing the consensus process
 * within the SynLedger blockchain network. The module coordinates validators to achieve agreement
 * on the validity of blocks and ensures network security through slashing dishonest actors and rewarding
 * honest participation. It integrates multiple subsystems, including P2P communication, the PoSyg
 * consensus engine, and the ledger. Security-critical functions such as validator slashing and multisig
 * handling are embedded to maintain the integrity and resilience of the network.
 *
 * Dependencies:
 * - Block class from the ledger module.
 * - P2PProtocol class for peer-to-peer communication.
 * - PoSygEngine for the Proof of Synergy consensus mechanism.
 * - Ledger for block finalization and storage.
 */

#ifndef CONSENSUS_HPP
#define CONSENSUS_HPP

#include <vector>        // To manage a collection of validators.
#include <cstddef>       // For size_t type, used for indexing.
#include <chrono>        // For time-related consensus operations.
#include <atomic>        // Atomic operations for thread-safe synchronization.
#include <mutex>         // Mutex for locking shared resources.
#include "../ledger/block.hpp"     // Block structure for consensus validation.
#include "../network/p2p_protocol.hpp"  // P2P communication protocol.
#include "posyg_engine.hpp"        // Proof of Synergy consensus engine.
#include "../ledger/ledger.hpp"    // Ledger for block storage and validation.

/**
 * @class Consensus
 * @brief Core class responsible for driving the consensus process in SynLedger.
 *
 * The Consensus class manages validators, facilitates block creation, and ensures that the network
 * maintains integrity by applying penalties (slashing) to malicious validators and distributing rewards
 * to honest ones. It also ensures that network parameters can adapt dynamically over time.
 */
class Consensus {
protected:
    size_t num_validators;              ///< Total number of validators participating in consensus.
    std::vector<size_t> validators;     ///< List of validator IDs involved in the current round.
    Block current_block;                ///< The current block under validation.
    P2PProtocol& p2p_network;           ///< Reference to the P2P communication protocol for validator coordination.
    PoSygEngine& posyg_engine;          ///< Reference to the Proof of Synergy consensus engine.
    Ledger& ledger;                     ///< Reference to the Ledger for block finalization.
    double slashing_penalty;            ///< Penalty for validators found to be malicious.
    double reward_for_validators;       ///< Reward for honest validators.

    /**
     * @brief Slashes the given validator for malicious behavior.
     * 
     * Applies penalties to dishonest or malicious validators by reducing their Synergy Score 
     * or potentially removing their validation privileges.
     * 
     * @param validator_id The ID of the validator to be slashed.
     */
    void slash_validator(size_t validator_id);

    /**
     * @brief Distributes rewards to honest validators.
     * 
     * After successful consensus, validators who participated honestly are rewarded 
     * based on their contributions to the consensus round.
     */
    void distribute_rewards();

    /**
     * @brief Handles multisignature verification for a block.
     * 
     * Ensures that the block has received sufficient validation signatures from the network's validators 
     * before finalization, enhancing security.
     * 
     * @param block The block being validated by the network.
     */
    void handle_multisig(const Block& block);

    /**
     * @brief Manages dynamic network adjustments.
     * 
     * Dynamically adjusts network parameters (e.g., slashing penalties, rewards) based on the state of 
     * the network, ensuring continued fairness and resistance to malicious behavior.
     */
    void dynamic_network_management();

public:
    /**
     * @brief Constructor for the Consensus class.
     * 
     * Initializes the consensus engine with the given number of validators, network protocol, consensus engine, 
     * and ledger. These components work in tandem to ensure that consensus is reached securely.
     * 
     * @param num_validators Number of validators for consensus.
     * @param network Reference to the P2P network protocol for communication.
     * @param posyg_engine Reference to the PoSyg consensus engine.
     * @param ledger Reference to the ledger for block storage and validation.
     */
    Consensus(size_t num_validators, P2PProtocol& network, PoSygEngine& posyg_engine, Ledger& ledger);

    /**
     * @brief Destructor for the Consensus class.
     * 
     * Handles any necessary cleanup when the consensus object is destroyed.
     */
    ~Consensus();

    /**
     * @brief Initiates the consensus process.
     * 
     * Coordinates the validators to begin a new round of consensus, validating the next block in the chain.
     */
    void initiate_consensus();

    /**
     * @brief Creates a new block for the blockchain.
     * 
     * Constructs a new block that will be proposed for validation by the validators.
     * 
     * @return The newly created Block.
     */
    Block create_new_block();

    /**
     * @brief Validates the given block.
     * 
     * Ensures the block adheres to network rules, has correct signatures, and has not been tampered with.
     * 
     * @param block The block to be validated.
     * @return true if the block is valid, false otherwise.
     */
    bool validate_block(const Block& block);

    /**
     * @brief Finalizes the block after successful consensus.
     * 
     * Once consensus is achieved, the block is added to the ledger and made immutable.
     * 
     * @param block The block to be finalized.
     */
    void finalize_block(const Block& block);

    /**
     * @brief Validates and slashes dishonest validators.
     * 
     * Reviews the behavior of validators during the consensus process. If any malicious activity is detected, 
     * the responsible validators are penalized through slashing.
     */
    void validate_and_slash();
};

#endif  // CONSENSUS_HPP

/**
 * @file consensus.hpp
 * 
 * This module is critical for maintaining the security and fairness of the SynLedger blockchain. 
 * It incorporates several important architectural decisions to prevent malicious behavior, such as validator 
 * slashing and reward distribution. The use of cryptographic multisig verification ensures integrity 
 * and prevents double-signing or other attacks. Dynamic network adjustments further reinforce the protocol's 
 * ability to adapt under different load and attack scenarios, making it robust and scalable.
 */
