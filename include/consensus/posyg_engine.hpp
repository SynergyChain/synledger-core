#ifndef POSYG_ENGINE_HPP
#define POSYG_ENGINE_HPP

#include <vector>
#include <cstddef>

#define PARTICIPANT_HONEST 1
#define PARTICIPANT_DISHONEST 0

const double INITIAL_SYNERGY = 100.0;
const double PENALTY_INCREMENT = 5.0;
const double REWARD_INCREMENT = 5.0;
const double SLASH_PENALTY = 100.0;
const double INITIAL_RESTORE_SYNERGY = 50.0;
const int MAX_ECONOMIC_ACTIVITY = 10;

struct Participant {
    size_t id;
    double synergy;
    double reward;
    double penalty;
    int violations_count;
    int behavior;
    int economic_activity;
    int governance_activity;
    bool slashed;
    double economic_contribution;

    Participant() 
        : id(0), synergy(INITIAL_SYNERGY), reward(0.0), penalty(0.0), 
          violations_count(0), behavior(PARTICIPANT_HONEST), economic_activity(1),
          governance_activity(1), slashed(false), economic_contribution(0.0) {}

    Participant(size_t id, int behavior)
        : id(id), synergy(INITIAL_SYNERGY), reward(0.0), penalty(0.0),
          violations_count(0), behavior(behavior), economic_activity(1),
          governance_activity(1), slashed(false), economic_contribution(0.0) {}

    void update_synergy();
    bool detect_suspicious_behavior();
    void apply_slash();
    void restore_after_slash();
    void update_economic_activity(double contribution);
};

struct Stats {
    int honest_count;
    int dishonest_count;
    double total_rewards;
    double total_penalties;
    double slashed_participants;
    double total_economic_contribution;
};

class PoSygEngine {
private:
    size_t num_participants;
    std::vector<Participant> participants;
    double dynamic_synergy_gain;
    double dynamic_penalty_increment;
    double dynamic_conversion_rate;
    double slash_penalty;
    double total_economic_activity;

    void adjust_network_parameters();
    void process_slashing();
    void distribute_rewards();

public:
    PoSygEngine(size_t num_participants);
    ~PoSygEngine();

    int run_cycle();
    void get_statistics(Stats &stats);
    void convert_synergy_to_tokens(double conversion_rate, double &total_tokens);
    Participant& get_participant(size_t participant_id);
    void apply_slashing_mechanism();
};

#endif // POSYG_ENGINE_HPP
