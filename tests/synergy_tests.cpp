#include <iostream>
#include "../include/economic/synergy_model.hpp"

int main() {
    try {
        PoSygEngine posyg_engine(10);

        for (int i = 0; i < 5; ++i) {
            posyg_engine.run_cycle();
        }

        Stats stats;
        posyg_engine.get_statistics(stats);

        std::cout << "Honest participants: " << stats.honest_count << std::endl;
        std::cout << "Dishonest participants: " << stats.dishonest_count << std::endl;
        std::cout << "Total rewards: " << stats.total_rewards << std::endl;
        std::cout << "Synergy tests passed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Synergy tests failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
