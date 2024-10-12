#include "subnet/subnet_manager.hpp"
#include <algorithm>

SubnetManager::SubnetManager(size_t total_subnets) 
    : total_subnets(total_subnets) {}

void SubnetManager::assign_node_to_subnet(size_t node_id) {
    std::lock_guard<std::mutex> lock(subnet_mutex);
    size_t least_loaded_subnet = find_least_loaded_subnet();
    node_to_subnet_map[node_id] = least_loaded_subnet;
    subnet_nodes[least_loaded_subnet].push_back(node_id);
}

size_t SubnetManager::get_node_subnet(size_t node_id) const {
    std::lock_guard<std::mutex> lock(subnet_mutex);
    auto it = node_to_subnet_map.find(node_id);
    if (it != node_to_subnet_map.end()) {
        return it->second;
    }
    throw std::runtime_error("Node not found in any subnet");
}

std::vector<size_t> SubnetManager::get_subnet_nodes(size_t subnet_id) const {
    std::lock_guard<std::mutex> lock(subnet_mutex);
    auto it = subnet_nodes.find(subnet_id);
    if (it != subnet_nodes.end()) {
        return it->second;
    }
    return {};
}

void SubnetManager::rebalance_subnets() {
    std::lock_guard<std::mutex> lock(subnet_mutex);
    // Simple load balancing algorithm to rebalance nodes across subnets
    for (auto& [subnet_id, nodes] : subnet_nodes) {
        if (nodes.size() > total_subnets) {
            size_t excess_nodes = nodes.size() - total_subnets;
            for (size_t i = 0; i < excess_nodes; ++i) {
                size_t node_to_move = nodes.back();
                nodes.pop_back();
                size_t new_subnet = find_least_loaded_subnet();
                node_to_subnet_map[node_to_move] = new_subnet;
                subnet_nodes[new_subnet].push_back(node_to_move);
            }
        }
    }
}

size_t SubnetManager::find_least_loaded_subnet() const {
    size_t least_loaded_subnet = 0;
    size_t min_size = std::numeric_limits<size_t>::max();

    for (size_t subnet_id = 0; subnet_id < total_subnets; ++subnet_id) {
        size_t subnet_size = subnet_nodes.at(subnet_id).size();
        if (subnet_size < min_size) {
            min_size = subnet_size;
            least_loaded_subnet = subnet_id;
        }
    }

    return least_loaded_subnet;
}
