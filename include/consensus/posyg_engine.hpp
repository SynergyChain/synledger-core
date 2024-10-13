/**
 * @file posyg_engine.hpp
 * @brief Proof of Synergy (PoSyg) engine module for SynLedger.
 *
 * This header defines the PoSygEngine class, which implements the core logic of the Proof of Synergy consensus mechanism. 
 * It manages participants, their synergy scores, penalties, rewards, and overall network health. This module plays a 
 * crucial role in ensuring fair and secure behavior by incentivizing honest participants while applying penalties to malicious actors.
 * Key elements include dynamic adjustments to synergy and slashing mechanisms to enhance network security and decentralization.
 */

#ifndef POSYG_ENGINE_HPP
#define POSYG_ENGINE_HPP

#include <vector>        // For storing and managing participants in the consensus.
#include <cstddef>       // For size_t types used for participant IDs.

// Constants defining participant behaviors and economic incentives.
#define PARTICIPANT_HONEST 1
#define PARTICIPANT_DISHONEST 0

// Initial and dynamic economic parameters for the consensus mechanism.
const double INITIAL_SYNERGY = 100.0;        ///< Initial synergy score assigned to each participant.
const double PENALTY_INCREMENT = 5.0;        ///< Increment applied to penalties for dishonest behavior.
const double REWARD_INCREMENT = 5.0;         ///< Reward increase for honest behavior.
const double SLASH_PENALTY = 100.0;          ///< Penalty applied when a participant is slashed.
const double INITIAL_RESTORE_SYNERGY = 50.0; ///< Synergy restored after a participant is slashed.
const int MAX_ECONOMIC_ACTIVITY = 10;        ///< Maximum allowed economic activity for a participant.

/**
 * @struct Participant
 * @brief Represents an individual validator or participant in the PoSyg consensus process.
 *
 * The Participant struct stores data related to individual participants, including synergy, rewards, 
 * penalties, behavior, and governance activity. It manages functions like synergy updates, detection of 
 * malicious behavior, and the slashing mechanism that penalizes dishonest participants.
 */
struct Participant {
    size_t id;                        ///< Unique ID for the participant.
    double synergy;                   ///< Current synergy score of the participant.
    double reward;                    ///< Accumulated rewards based on honest activity.
    double penalty;                   ///< Penalty incurred for dishonest actions.
    int violations_count;             ///< Number of violations by the participant.
    int behavior;                     ///< Current behavior: PARTICIPANT_HONEST or PARTICIPANT_DISHONEST.
    int economic_activity;            ///< Participant's level of economic activity.
    int governance_activity;          ///< Participant's involvement in governance activities.
    bool slashed;                     ///< Whether the participant has been slashed for misconduct.
    double economic_contribution;     ///< Participant's contribution to the economic health of the network.

    /**
     * @brief Constructor for a default participant.
     * Initializes a participant with default synergy, no penalties or rewards, and an honest behavior.
     */
    Participant() 
        : id(0), synergy(INITIAL_SYNERGY), reward(0.0), penalty(0.0), 
          violations_count(0), behavior(PARTICIPANT_HONEST), economic_activity(1),
          governance_activity(1), slashed(false), economic_contribution(0.0) {}

    /**
     * @brief Constructor for a participant with a specified ID and behavior.
     * Initializes a participant with the provided ID and behavior status.
     * 
     * @param id Participant's unique ID.
     * @param behavior Participant's initial behavior (honest or dishonest).
     */
    Participant(size_t id, int behavior)
        : id(id), synergy(INITIAL_SYNERGY), reward(0.0), penalty(0.0),
          violations_count(0), behavior(behavior), economic_activity(1),
          governance_activity(1), slashed(false), economic_contribution(0.0) {}

    void update_synergy();                     ///< Updates the participant's synergy score.
    bool detect_suspicious_behavior();         ///< Detects if the participant is engaging in malicious activities.
    void apply_slash();                        ///< Applies the slashing penalty to the participant.
    void restore_after_slash();                ///< Restores some synergy after a slashed participant recovers.
    void update_economic_activity(double contribution); ///< Updates the participant's economic activity score.
};

/**
 * @struct Stats
 * @brief Aggregates statistics about the consensus process.
 *
 * The Stats struct tracks key metrics, such as the number of honest and dishonest participants, 
 * total rewards distributed, and total penalties applied, providing insights into the network's health.
 */
struct Stats {
    int honest_count;                  ///< Number of honest participants.
    int dishonest_count;               ///< Number of dishonest participants.
    double total_rewards;              ///< Total rewards distributed in the network.
    double total_penalties;            ///< Total penalties applied for dishonest behavior.
    double slashed_participants;       ///< Number of participants that have been slashed.
    double total_economic_contribution;///< Total economic contribution from all participants.
};

/**
 * @class PoSygEngine
 * @brief Core engine implementing the Proof of Synergy consensus mechanism.
 *
 * The PoSygEngine manages the behavior of participants in the consensus network, adjusting their synergy scores 
 * dynamically based on their behavior and economic activities. It ensures network security by slashing dishonest 
 * participants, distributing rewards to honest ones, and adjusting network parameters for fairness and performance.
 */
class PoSygEngine {
private:
    size_t num_participants;                      ///< Total number of participants in the network.
    std::vector<Participant> participants;        ///< List of all participants in the network.
    double dynamic_synergy_gain;                  ///< Synergy gain based on network conditions.
    double dynamic_penalty_increment;             ///< Increment for penalties based on network health.
    double dynamic_conversion_rate;               ///< Conversion rate for synergy-to-token conversions.
    double slash_penalty;                         ///< Penalty applied for slashing a participant.
    double total_economic_activity;               ///< Total economic activity in the network.

    void adjust_network_parameters();             ///< Adjusts network parameters dynamically based on conditions.
    void process_slashing();                      ///< Processes slashing for all dishonest participants.
    void distribute_rewards();                    ///< Distributes rewards to honest participants.

public:
    /**
     * @brief Constructor for the PoSygEngine.
     * 
     * Initializes the engine with a specific number of participants, preparing the network for consensus cycles.
     * 
     * @param num_participants Number of participants in the network.
     */
    PoSygEngine(size_t num_participants);

    /**
     * @brief Destructor for the PoSygEngine.
     */
    ~PoSygEngine();

    /**
     * @brief Runs a single consensus cycle.
     * 
     * Simulates one full round of the consensus process, updating participant states and ensuring security.
     * 
     * @return An integer representing the result of the cycle.
     */
    int run_cycle();

    /**
     * @brief Retrieves network statistics.
     * 
     * Provides an aggregate summary of key metrics, such as total rewards and penalties.
     * 
     * @param stats Reference to a Stats struct that will hold the retrieved data.
     */
    void get_statistics(Stats &stats);

    /**
     * @brief Converts participant synergy scores to tokens.
     * 
     * Converts synergy points accumulated by participants into network tokens based on the dynamic conversion rate.
     * 
     * @param conversion_rate The conversion rate for synergy points to tokens.
     * @param total_tokens Reference to a variable to store the total number of tokens converted.
     */
    void convert_synergy_to_tokens(double conversion_rate, double &total_tokens);

    /**
     * @brief Retrieves a specific participant by ID.
     * 
     * Provides access to a participant's data based on their unique ID.
     * 
     * @param participant_id The ID of the participant.
     * @return Reference to the requested Participant.
     */
    Participant& get_participant(size_t participant_id);

    /**
     * @brief Applies the slashing mechanism across the network.
     * 
     * Identifies and penalizes dishonest participants by applying the slashing mechanism.
     */
    void apply_slashing_mechanism();
};

#endif // POSYG_ENGINE_HPP

/**
 * @file posyg_engine.hpp
 *
 * This module is a cornerstone of the PoSyg consensus, balancing incentives and penalties to maintain network integrity. 
 * It dynamically adjusts network conditions based on behavior and contributions, leveraging cryptographic slashing and rewards 
 * to enforce honesty and resilience. The use of adaptive parameters ensures that the network is secure, scalable, and fair under varying conditions.
 */
