#include "consensus/posyg_engine.hpp"
#include <omp.h>
#include <cstdlib>
#include <random>

void Participant::update_synergy() {
    if (behavior == PARTICIPANT_HONEST && !slashed) {
        synergy += 10.0 * economic_activity;
        reward += REWARD_INCREMENT * economic_activity;
    } else if (!slashed) {
        synergy -= 10.0 * economic_activity;
        penalty += PENALTY_INCREMENT * economic_activity;
        if (detect_suspicious_behavior()) {
            penalty += 10.0;
            apply_slash();
        }
    }
    if (synergy < 0.0) {
        synergy = 0.0;
    }
}

bool Participant::detect_suspicious_behavior() {
    return economic_activity > 4 && governance_activity > 2;
}

void Participant::apply_slash() {
    if (!slashed) {
        slashed = true;
        penalty += SLASH_PENALTY;
        synergy = 0.0;
    }
}

void Participant::restore_after_slash() {
    if (slashed) {
        slashed = false;
        synergy = INITIAL_RESTORE_SYNERGY;
    }
}

void Participant::update_economic_activity(double contribution) {
    economic_contribution += contribution;
    economic_activity = std::min(static_cast<int>(contribution / 10.0), MAX_ECONOMIC_ACTIVITY);
}

PoSygEngine::PoSygEngine(size_t num_participants)
    : num_participants(num_participants), 
      dynamic_synergy_gain(10.0), 
      dynamic_penalty_increment(PENALTY_INCREMENT),
      dynamic_conversion_rate(0.1),
      slash_penalty(SLASH_PENALTY),
      total_economic_activity(0.0) {
    participants.resize(num_participants);
    #pragma omp parallel for
    for (size_t i = 0; i < num_participants; i++) {
        participants[i] = Participant(i, PARTICIPANT_HONEST);
    }
}

PoSygEngine::~PoSygEngine() {
    participants.clear();
}

void PoSygEngine::adjust_network_parameters() {
    int honest_count = 0, dishonest_count = 0;

    #pragma omp parallel for reduction(+:honest_count, dishonest_count)
    for (size_t i = 0; i < num_participants; i++) {
        if (participants[i].behavior == PARTICIPANT_HONEST) {
            honest_count++;
        } else {
            dishonest_count++;
        }
    }

    double dishonest_ratio = static_cast<double>(dishonest_count) / num_participants;

    if (dishonest_ratio > 0.5) {
        dynamic_penalty_increment *= 1.1;
        dynamic_synergy_gain *= 0.9;
    } else {
        dynamic_penalty_increment *= 0.95;
        dynamic_synergy_gain *= 1.05;
    }

    dynamic_conversion_rate = 0.1 + dishonest_ratio * 0.05;
}

int PoSygEngine::run_cycle() {
    adjust_network_parameters();
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);

    #pragma omp parallel for
    for (size_t i = 0; i < num_participants; i++) {
        int rand_val = dis(gen);
        if (rand_val < 3) {
            participants[i].behavior = PARTICIPANT_DISHONEST;
        } else {
            participants[i].behavior = PARTICIPANT_HONEST;
        }
        participants[i].update_synergy();
    }

    process_slashing();
    distribute_rewards();
    return 0;
}

void PoSygEngine::process_slashing() {
    #pragma omp parallel for
    for (size_t i = 0; i < num_participants; i++) {
        if (participants[i].violations_count > 3 && !participants[i].slashed) {
            participants[i].apply_slash();
        }
    }
}

void PoSygEngine::distribute_rewards() {
    double total_synergy = 0.0;
    #pragma omp parallel for reduction(+:total_synergy)
    for (size_t i = 0; i < num_participants; i++) {
        if (!participants[i].slashed) {
            total_synergy += participants[i].synergy;
        }
    }

    if (total_synergy > 0.0) {
        #pragma omp parallel for
        for (size_t i = 0; i < num_participants; i++) {
            if (!participants[i].slashed) {
                participants[i].reward += (participants[i].synergy / total_synergy) * total_economic_activity;
            }
        }
    }
}

void PoSygEngine::get_statistics(Stats &stats) {
    Stats local_stats = {0, 0, 0.0, 0.0, 0, 0.0};  // Initialize local stats

    #pragma omp parallel
    {
        Stats thread_stats = {0, 0, 0.0, 0.0, 0, 0.0};

        #pragma omp for nowait
        for (size_t i = 0; i < num_participants; i++) {
            if (participants[i].behavior == PARTICIPANT_HONEST) {
                thread_stats.honest_count++;
            } else {
                thread_stats.dishonest_count++;
            }
            thread_stats.total_rewards += participants[i].reward;
            thread_stats.total_penalties += participants[i].penalty;
            if (participants[i].slashed) {
                thread_stats.slashed_participants++;
            }
        }

        // Critical section to merge thread-specific stats into global stats
        #pragma omp critical
        {
            local_stats.honest_count += thread_stats.honest_count;
            local_stats.            dishonest_count += thread_stats.dishonest_count;
            local_stats.total_rewards += thread_stats.total_rewards;
            local_stats.total_penalties += thread_stats.total_penalties;
            local_stats.slashed_participants += thread_stats.slashed_participants;
        }
    }

    stats.honest_count = local_stats.honest_count;
    stats.dishonest_count = local_stats.dishonest_count;
    stats.total_rewards = local_stats.total_rewards;
    stats.total_penalties = local_stats.total_penalties;
    stats.slashed_participants = local_stats.slashed_participants;
}

void PoSygEngine::convert_synergy_to_tokens(double conversion_rate, double &total_tokens) {
    total_tokens = 0.0;

    #pragma omp parallel for reduction(+:total_tokens)
    for (size_t i = 0; i < num_participants; i++) {
        if (!participants[i].slashed) {
            double tokens = participants[i].synergy * conversion_rate;
            participants[i].synergy = 0.0;
            total_tokens += tokens;
        }
    }
}

Participant& PoSygEngine::get_participant(size_t participant_id) {
    return participants.at(participant_id);
}

void PoSygEngine::apply_slashing_mechanism() {
    process_slashing();
}
