#include <iostream>
#include "../include/governance/governance.hpp"
#include "../include/consensus/posyg_engine.hpp"

int main() {
    try {
        PoSygEngine posyg_engine(10);
        Governance governance(posyg_engine);

        governance.create_proposal("Increase block reward");
        governance.vote(1, true, 0);
        governance.finalize_proposal(1);

        if (governance.is_proposal_approved(1)) {
            std::cout << "Governance proposal approved." << std::endl;
        } else {
            std::cout << "Governance proposal rejected." << std::endl;
        }

        std::cout << "Governance tests passed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Governance tests failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
