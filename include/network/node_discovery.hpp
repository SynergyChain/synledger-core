/**
 * @file node_discovery.hpp
 * @brief Node discovery module for SynLedger's peer-to-peer network.
 *
 * This header defines the `NodeDiscovery` class, which is responsible for discovering and managing peer nodes 
 * within the SynLedger network. The module facilitates peer-to-peer communication by maintaining a list of known 
 * nodes and their corresponding network addresses, enabling nodes to connect and exchange data securely.
 */

#ifndef NODE_DISCOVERY_HPP
#define NODE_DISCOVERY_HPP

#include <string>
#include <vector>
#include <unordered_map>

/**
 * @class NodeDiscovery
 * @brief Manages node discovery and peer connections in a decentralized network.
 *
 * The `NodeDiscovery` class is responsible for identifying and storing information about peer nodes in the network. 
 * It maintains a registry of known nodes, each identified by a unique node ID and associated with a network address. 
 * This facilitates node-to-node communication and ensures that peers can discover and connect to each other efficiently.
 */
class NodeDiscovery {
public:
    /**
     * @brief Constructs a NodeDiscovery object with the given node ID and network address.
     * 
     * Initializes the node discovery process by setting the node's unique identifier and its address in the network.
     * 
     * @param node_id The unique identifier for this node.
     * @param network_address The network address (IP or other identifier) for this node.
     */
    NodeDiscovery(size_t node_id, const std::string& network_address);

    /**
     * @brief Discovers new nodes in the network.
     * 
     * Initiates the node discovery process, finding new peers and adding them to the list of known nodes.
     * This function typically involves broadcasting requests to known nodes and receiving addresses of new peers.
     */
    void discover_nodes();

    /**
     * @brief Adds a new node to the known node list.
     * 
     * Registers a new peer node in the local discovery database, associating it with its unique node ID and address.
     * 
     * @param node_id The unique ID of the new node.
     * @param address The network address of the new node.
     */
    void add_node(size_t node_id, const std::string& address);

    /**
     * @brief Retrieves a list of known node IDs.
     * 
     * Provides a list of all nodes that this node is aware of, identified by their node IDs.
     * 
     * @return A vector containing the IDs of all known nodes.
     */
    std::vector<size_t> get_known_nodes() const;

    /**
     * @brief Retrieves the network address of a specific node.
     * 
     * Given a node ID, this function returns the corresponding network address, allowing for direct communication.
     * 
     * @param node_id The ID of the node whose address is requested.
     * @return The network address of the specified node.
     */
    std::string get_node_address(size_t node_id) const;

private:
    size_t node_id;  ///< The unique identifier for this node.
    std::string network_address;  ///< The network address of this node.
    std::unordered_map<size_t, std::string> known_nodes;  ///< A map of known nodes and their network addresses.
};

#endif // NODE_DISCOVERY_HPP

/**
 * @file node_discovery.hpp
 *
 * This module implements the node discovery mechanism for the SynLedger network. It is responsible for identifying 
 * and storing peer nodes and their network addresses, enabling efficient peer-to-peer communication. The discovery 
 * process ensures that new nodes can join the network, and existing nodes can maintain awareness of their peers, 
 * contributing to the decentralized and distributed nature of the system.
 */
