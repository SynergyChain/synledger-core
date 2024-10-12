#include <iostream>
#include "../include/subnet/subnet_manager.hpp"

int main() {
    try {
        SubnetManager subnet_manager(3);
        subnet_manager.assign_node_to_subnet(1);
        subnet_manager.assign_node_to_subnet(2);

        size_t subnet = subnet_manager.get_node_subnet(1);
        std::cout << "Node 1 assigned to subnet " << subnet << std::endl;

        subnet_manager.rebalance_subnets();
        std::cout << "Subnet rebalancing completed." << std::endl;

        std::cout << "Subnet tests passed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Subnet tests failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
