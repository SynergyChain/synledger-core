#include "network/node_discovery.hpp"
#include <iostream>

NodeDiscovery::NodeDiscovery(size_t node_id, const std::string& network_address)
    : node_id(node_id), network_address(network_address) {}

void NodeDiscovery::discover_nodes() {
    std::cout << "Node " << node_id << " is discovering nodes on the network..." << std::endl;
    
    // Simulate node discovery (in a real scenario, this could involve network protocols like UDP for broadcasting)
    add_node(1, "192.168.1.1");
    add_node(2, "192.168.1.2");
    add_node(3, "192.168.1.3");

    std::cout << "Discovery complete. Known nodes: " << known_nodes.size() << std::endl;
}

void NodeDiscovery::add_node(size_t new_node_id, const std::string& address) {
    if (known_nodes.find(new_node_id) == known_nodes.end()) {
        known_nodes[new_node_id] = address;
        std::cout << "Node " << node_id << " added new node: " << new_node_id << " (" << address << ")" << std::endl;
    } else {
        std::cout << "Node " << new_node_id << " is already known." << std::endl;
    }
}

std::vector<size_t> NodeDiscovery::get_known_nodes() const {
    std::vector<size_t> nodes;
    for (const auto& node : known_nodes) {
        nodes.push_back(node.first);
    }
    return nodes;
}

std::string NodeDiscovery::get_node_address(size_t search_node_id) const {
    auto it = known_nodes.find(search_node_id);
    if (it != known_nodes.end()) {
        return it->second;
    }
    return "Unknown node";
}
