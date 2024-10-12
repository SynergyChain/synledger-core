#include "network/p2p_protocol.hpp"
#include <iostream>
#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>

P2PProtocol::P2PProtocol(size_t node_id, const std::string& network_address)
    : node_id(node_id), network_address(network_address), listen_socket(-1), stop_flag(false) {}

P2PProtocol::~P2PProtocol() {
    shutdown();
    if (incoming_thread.joinable()) {
        incoming_thread.join();
    }
}

void P2PProtocol::initialize(int port) {
    setup_listen_socket(port);
    incoming_thread = std::thread(&P2PProtocol::handle_incoming_connections, this);
    std::cout << "Node " << node_id << " initialized on " << network_address << ":" << port << std::endl;
}

void P2PProtocol::setup_listen_socket(int port) {
    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_socket < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    int opt = 1;
    if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        close(listen_socket);
        throw std::runtime_error("Failed to set socket options");
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(listen_socket);
        throw std::runtime_error("Failed to bind socket");
    }

    if (listen(listen_socket, 10) < 0) {
        close(listen_socket);
        throw std::runtime_error("Failed to listen on socket");
    }
}

void P2PProtocol::handle_incoming_connections() {
    while (!stop_flag) {
        if (listen_socket < 0) break;
        
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(listen_socket, (struct sockaddr*)&client_addr, &client_len);

        if (client_socket >= 0) {
            std::thread(&P2PProtocol::handle_peer_connection, this, client_socket).detach();
        } else if (listen_socket != -1) {
            perror("Failed to accept connection");
        }
    }
}

void P2PProtocol::handle_peer_connection(int peer_socket) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    int bytes_read = recv(peer_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        std::string message(buffer);
        std::cout << "Received message: " << message << std::endl;
    } else if (bytes_read < 0) {
        perror("Failed to read from peer socket");
    }

    close(peer_socket);
}

void P2PProtocol::send_message(size_t peer_node_id, const std::string& message) {
    std::shared_lock<std::shared_mutex> lock(peer_mutex);
    auto it = peer_addresses.find(peer_node_id);
    if (it == peer_addresses.end()) {
        throw std::runtime_error("Peer address not found");
    }
    std::string peer_address = it->second;

    sockaddr_in peer_addr{};
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(8080);
    if (inet_pton(AF_INET, peer_address.c_str(), &peer_addr.sin_addr) <= 0) {
        throw std::runtime_error("Invalid peer address");
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    if (connect(sock, (struct sockaddr*)&peer_addr, sizeof(peer_addr)) < 0) {
        close(sock);
        throw std::runtime_error("Failed to connect to peer");
    }

    if (send(sock, message.c_str(), message.length(), 0) < 0) {
        close(sock);
        throw std::runtime_error("Failed to send message to peer");
    }

    close(sock);
}

void P2PProtocol::add_peer(size_t peer_node_id, const std::string& peer_address) {
    std::lock_guard<std::shared_mutex> lock(peer_mutex);
    if (peer_addresses.find(peer_node_id) == peer_addresses.end()) {
        peers.push_back(peer_node_id);
        peer_addresses[peer_node_id] = peer_address;
    }
}

std::vector<size_t> P2PProtocol::get_active_peers() const {
    std::shared_lock<std::shared_mutex> lock(peer_mutex);
    return peers;
}

std::string P2PProtocol::get_peer_address(size_t peer_node_id) const {
    std::shared_lock<std::shared_mutex> lock(peer_mutex);
    auto it = peer_addresses.find(peer_node_id);
    if (it != peer_addresses.end()) {
        return it->second;
    }
    return "Unknown";
}

void P2PProtocol::shutdown() {
    stop_flag = true;
    if (listen_socket >= 0) {
        close(listen_socket);
        listen_socket = -1;
    }
}
