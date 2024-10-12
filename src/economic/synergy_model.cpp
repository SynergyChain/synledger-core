#include "economic/synergy_model.hpp"
#include <algorithm>

double SynergyModel::calculate_synergy(double initial_synergy, double economic_activity, double governance_activity) {
    double synergy_gain = (economic_activity * 0.6) + (governance_activity * 0.4);
    return std::max(initial_synergy + synergy_gain, 0.0);
}

double SynergyModel::apply_penalty(double synergy, double penalty) {
    return std::max(synergy - penalty, 0.0);
}

double SynergyModel::convert_synergy_to_tokens(double synergy, double conversion_rate) {
    return synergy * conversion_rate;
}

double SynergyModel::adjust_conversion_rate(double current_rate, double network_conditions) {
    // Adjust the conversion rate based on network conditions (e.g., activity level, synergy distribution)
    return current_rate * (1.0 + network_conditions * 0.05);
}
