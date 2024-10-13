/**
 * @file p2p_protocol.hpp
 * @brief Peer-to-peer communication protocol for SynLedger's decentralized network.
 *
 * This header defines the P2PProtocol class, which manages peer-to-peer communication between nodes in the 
 * SynLedger network. It facilitates the discovery of peers, exchange of messages, and handling of incoming 
 * connections through a decentralized, secure, and scalable network protocol.
 */

#ifndef P2P_PROTOCOL_HPP
#define P2P_PROTOCOL_HPP

#include <string>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <netinet/in.h>  // For socket communication

/**
 * @class P2PProtocol
 * @brief Manages peer-to-peer communication in the SynLedger network.
 *
 * The P2PProtocol class handles communication between nodes by managing incoming and outgoing connections, 
 * sending messages, and maintaining a list of known peers. This decentralized protocol ensures that nodes can 
 * discover each other, share data, and communicate effectively, forming the backbone of the blockchain's distributed network.
 */
class P2PProtocol {
private:
    size_t node_id;                       ///< The unique identifier for this node.
    std::string network_address;          ///< Network address of this node.
    std::vector<size_t> peers;            ///< List of peer node IDs known to this node.
    std::map<size_t, std::string> peer_addresses; ///< Map of peer node IDs to their network addresses.
    int listen_socket;                    ///< Socket used to listen for incoming peer connections.
    mutable std::shared_mutex peer_mutex; ///< Mutex to protect access to the peer list.
    bool stop_flag;                       ///< Flag to signal when to stop incoming connections.
    std::thread incoming_thread;          ///< Thread handling incoming connections.

    /**
     * @brief Handles incoming connections from peers.
     * 
     * This method runs in a separate thread to continuously accept new peer connections 
     * and assign each connection to a handler.
     */
    void handle_incoming_connections();

    /**
     * @brief Handles communication with a connected peer.
     * 
     * Manages the interaction and message exchange between this node and the connected peer.
     * 
     * @param peer_socket The socket for the peer connection.
     */
    void handle_peer_connection(int peer_socket);

    /**
     * @brief Sets up the listening socket to accept incoming peer connections.
     * 
     * Configures the socket and binds it to the specified port for incoming network connections.
     * 
     * @param port The port to bind the listening socket to.
     */
    void setup_listen_socket(int port);

public:
    /**
     * @brief Constructs a P2PProtocol object.
     * 
     * Initializes the peer-to-peer protocol for the node with its ID and network address.
     * 
     * @param node_id The unique ID of this node.
     * @param network_address The network address of this node.
     */
    P2PProtocol(size_t node_id, const std::string& network_address);

    /**
     * @brief Destructor for P2PProtocol.
     * 
     * Cleans up resources and shuts down any ongoing peer connections.
     */
    ~P2PProtocol();

    /**
     * @brief Initializes the peer-to-peer protocol.
     * 
     * Sets up the listening socket and begins accepting incoming peer connections.
     * 
     * @param port The port to use for incoming connections.
     */
    void initialize(int port);

    /**
     * @brief Sends a message to a specified peer.
     * 
     * Sends a message to the given peer node using its ID. The message is transmitted over the network.
     * 
     * @param peer_node_id The ID of the peer to send the message to.
     * @param message The message to send.
     */
    void send_message(size_t peer_node_id, const std::string& message);

    /**
     * @brief Adds a new peer to the list of known peers.
     * 
     * Registers a peer by adding its node ID and network address to the list of known peers.
     * 
     * @param peer_node_id The unique ID of the peer node.
     * @param peer_address The network address of the peer node.
     */
    void add_peer(size_t peer_node_id, const std::string& peer_address);

    /**
     * @brief Retrieves a list of active peers.
     * 
     * Returns a vector of node IDs representing peers that this node is aware of and has active connections with.
     * 
     * @return A vector of peer node IDs.
     */
    std::vector<size_t> get_active_peers() const;

    /**
     * @brief Retrieves the network address of a peer.
     * 
     * Given the peer's node ID, returns the network address of that peer.
     * 
     * @param peer_node_id The ID of the peer.
     * @return The network address of the peer.
     */
    std::string get_peer_address(size_t peer_node_id) const;

    /**
     * @brief Shuts down the peer-to-peer protocol.
     * 
     * Stops accepting new connections and closes any active connections, gracefully shutting down the peer-to-peer protocol.
     */
    void shutdown();
};

#endif  // P2P_PROTOCOL_HPP

/**
 * @file p2p_protocol.hpp
 *
 * This module implements the peer-to-peer communication protocol for SynLedger, enabling nodes to 
 * connect, exchange messages, and manage peer relationships. The protocol ensures that the decentralized 
 * nature of the network is maintained through dynamic peer discovery and message transmission, forming the core 
 * of the distributed ledger system.
 */
