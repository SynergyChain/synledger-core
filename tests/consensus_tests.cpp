#include <iostream>
#include "../include/consensus/consensus.hpp"
#include "../include/consensus/posyg_engine.hpp"

int main() {
    try {
        PoSygEngine posyg_engine(10);
        Consensus consensus(5, /*network=*/nullptr, posyg_engine, /*ledger=*/nullptr);

        consensus.initiate_consensus();
        std::cout << "Consensus tests passed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Consensus tests failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}


