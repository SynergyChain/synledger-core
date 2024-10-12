#ifndef P2P_PROTOCOL_HPP
#define P2P_PROTOCOL_HPP

#include <string>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <netinet/in.h>

class P2PProtocol {
private:
    size_t node_id;
    std::string network_address;
    std::vector<size_t> peers;
    std::map<size_t, std::string> peer_addresses;
    int listen_socket;
    mutable std::shared_mutex peer_mutex;
    bool stop_flag;
    std::thread incoming_thread;

    void handle_incoming_connections();
    void handle_peer_connection(int peer_socket);
    void setup_listen_socket(int port);

public:
    P2PProtocol(size_t node_id, const std::string& network_address);
    ~P2PProtocol();
    void initialize(int port);
    void send_message(size_t peer_node_id, const std::string& message);
    void add_peer(size_t peer_node_id, const std::string& peer_address);
    std::vector<size_t> get_active_peers() const;
    std::string get_peer_address(size_t peer_node_id) const;
    void shutdown();
};

#endif  // P2P_PROTOCOL_HPP
