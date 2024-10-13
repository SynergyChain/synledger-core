/**
 * @file synergy_model.hpp
 * @brief Synergy model module for SynLedger's Proof of Synergy (PoSyg) consensus mechanism.
 *
 * This header defines the `SynergyModel` class, which provides methods for calculating and managing
 * synergy scores in the SynLedger blockchain. The synergy score reflects a participant's overall contribution 
 * to the network, based on economic activity, governance involvement, and penalties for dishonest behavior. 
 * The class also includes methods for converting synergy into tokens and adjusting the conversion rate dynamically.
 */

#ifndef SYNERGY_MODEL_HPP
#define SYNERGY_MODEL_HPP

/**
 * @class SynergyModel
 * @brief Implements synergy calculations and adjustments for participants in the PoSyg consensus.
 *
 * The `SynergyModel` class handles the calculation of synergy based on participant behavior, economic activity, 
 * and governance involvement. It provides methods to apply penalties for dishonest behavior, convert synergy into 
 * network tokens, and adjust the conversion rate dynamically based on network conditions. This model ensures that 
 * participants are rewarded fairly for their positive contributions while penalizing malicious actors.
 */
class SynergyModel {
public:
    /**
     * @brief Calculates a participant's updated synergy score.
     * 
     * This method calculates a participant's synergy based on their initial synergy, economic activity, 
     * and governance activity. A higher economic and governance contribution leads to an increase in synergy, 
     * incentivizing positive network behavior.
     * 
     * @param initial_synergy The starting synergy score of the participant.
     * @param economic_activity The participant's economic activity level.
     * @param governance_activity The participant's governance involvement.
     * @return The updated synergy score.
     */
    static double calculate_synergy(double initial_synergy, double economic_activity, double governance_activity);

    /**
     * @brief Applies a penalty to a participant's synergy score.
     * 
     * This method reduces a participant's synergy based on the penalties incurred from dishonest behavior 
     * or rule violations. The penalty is subtracted from the current synergy, discouraging malicious activity.
     * 
     * @param synergy The current synergy score.
     * @param penalty The penalty to be applied.
     * @return The new synergy score after the penalty is applied.
     */
    static double apply_penalty(double synergy, double penalty);

    /**
     * @brief Converts a participant's synergy into network tokens.
     * 
     * This method converts a participant's synergy score into tokens, which can be used for transactions, 
     * staking, or other economic activities. The conversion rate dynamically adjusts based on network conditions.
     * 
     * @param synergy The participant's current synergy score.
     * @param conversion_rate The current rate of conversion from synergy to tokens.
     * @return The equivalent number of tokens for the given synergy.
     */
    static double convert_synergy_to_tokens(double synergy, double conversion_rate);

    /**
     * @brief Adjusts the conversion rate dynamically based on network conditions.
     * 
     * This method recalculates the conversion rate for synergy-to-token conversion. If the network is 
     * experiencing high activity or inflationary risks, the rate may be reduced to balance the ecosystem.
     * 
     * @param current_rate The current conversion rate.
     * @param network_conditions A measure of the current network state (e.g., activity, token supply).
     * @return The adjusted conversion rate.
     */
    static double adjust_conversion_rate(double current_rate, double network_conditions);
};

#endif // SYNERGY_MODEL_HPP

/**
 * @file synergy_model.hpp
 * 
 * This module defines the core logic for calculating and managing synergy in the SynLedger blockchain. 
 * Synergy serves as a dynamic metric for rewarding participants based on economic activity, governance involvement, 
 * and overall contribution to the network. The model also includes mechanisms for penalizing dishonest behavior 
 * and adjusting conversion rates to ensure the long-term economic stability of the system.
 */
