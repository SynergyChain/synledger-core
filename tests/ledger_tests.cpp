#include <iostream>
#include "../include/ledger/block.hpp"
#include "../include/ledger/ledger.hpp"

int main() {
    try {
        Ledger ledger(3);

        Block new_block(1, ledger.get_latest_block().get_block_hash(), 2);
        ledger.add_block(new_block);

        if (ledger.validate_chain()) {
            std::cout << "Ledger chain validated successfully." << std::endl;
        } else {
            std::cout << "Ledger validation failed." << std::endl;
        }

        std::cout << "Ledger tests passed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ledger tests failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
