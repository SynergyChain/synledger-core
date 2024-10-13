/**
 * @file subnet_manager.hpp
 * @brief Subnet management for distributing nodes in the SynLedger network.
 *
 * This header defines the `SubnetManager` class, which is responsible for assigning nodes to subnets 
 * within the decentralized SynLedger network. It ensures load balancing by evenly distributing nodes 
 * across subnets and provides functionality to rebalance the subnets dynamically as network conditions change.
 */

#ifndef SUBNET_MANAGER_HPP
#define SUBNET_MANAGER_HPP

#include <vector>
#include <string>
#include <map>
#include <mutex>

/**
 * @class SubnetManager
 * @brief Manages subnets and distributes nodes within subnets for the SynLedger network.
 *
 * The `SubnetManager` class is responsible for assigning nodes to specific subnets, tracking node assignments, 
 * and ensuring balanced load distribution across the subnets. This class provides methods to dynamically rebalance 
 * subnets and query subnet membership for specific nodes.
 */
class SubnetManager {
public:
    /**
     * @brief Constructs a SubnetManager with a specified number of subnets.
     * 
     * Initializes the subnet manager, setting up the necessary subnets for node assignment.
     * 
     * @param total_subnets The total number of subnets in the network.
     */
    SubnetManager(size_t total_subnets);

    /**
     * @brief Assigns a node to a subnet.
     * 
     * Randomly assigns a node to one of the subnets, ensuring a balanced distribution of nodes across all subnets.
     * 
     * @param node_id The unique identifier of the node to be assigned.
     */
    void assign_node_to_subnet(size_t node_id);

    /**
     * @brief Retrieves the subnet ID for a specific node.
     * 
     * Returns the subnet to which a given node is assigned.
     * 
     * @param node_id The unique identifier of the node.
     * @return The ID of the subnet to which the node is assigned.
     */
    size_t get_node_subnet(size_t node_id) const;

    /**
     * @brief Retrieves the list of nodes in a specific subnet.
     * 
     * Returns all the nodes assigned to a given subnet.
     * 
     * @param subnet_id The ID of the subnet.
     * @return A vector containing the node IDs assigned to the subnet.
     */
    std::vector<size_t> get_subnet_nodes(size_t subnet_id) const;

    /**
     * @brief Rebalances the subnets to ensure even distribution of nodes.
     * 
     * Dynamically redistributes nodes across subnets to ensure that no subnet becomes overloaded.
     */
    void rebalance_subnets();

private:
    size_t total_subnets;                                ///< Total number of subnets in the network.
    std::map<size_t, size_t> node_to_subnet_map;         ///< Maps each node to its assigned subnet.
    std::map<size_t, std::vector<size_t>> subnet_nodes;  ///< Stores the nodes assigned to each subnet.
    mutable std::mutex subnet_mutex;                     ///< Mutex for thread-safe access to subnet data.

    /**
     * @brief Finds the least loaded subnet.
     * 
     * Determines which subnet currently has the fewest nodes, allowing new nodes to be assigned there.
     * 
     * @return The ID of the least loaded subnet.
     */
    size_t find_least_loaded_subnet() const;
};

#endif // SUBNET_MANAGER_HPP

/**
 * @file subnet_manager.hpp
 * 
 * This module defines the logic for managing subnets in the SynLedger network, providing efficient 
 * load balancing by distributing nodes evenly across subnets. It supports node assignment, querying subnet membership, 
 * and rebalancing to ensure a scalable and efficient network.
 */
