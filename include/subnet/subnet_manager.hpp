#ifndef SUBNET_MANAGER_HPP
#define SUBNET_MANAGER_HPP

#include <vector>
#include <string>
#include <map>
#include <mutex>

class SubnetManager {
public:
    SubnetManager(size_t total_subnets);
    
    void assign_node_to_subnet(size_t node_id);
    size_t get_node_subnet(size_t node_id) const;
    std::vector<size_t> get_subnet_nodes(size_t subnet_id) const;
    void rebalance_subnets();

private:
    size_t total_subnets;
    std::map<size_t, size_t> node_to_subnet_map;
    std::map<size_t, std::vector<size_t>> subnet_nodes;
    mutable std::mutex subnet_mutex;

    size_t find_least_loaded_subnet() const;
};

#endif // SUBNET_MANAGER_HPP
