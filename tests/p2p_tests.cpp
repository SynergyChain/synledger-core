#include <iostream>
#include "../include/network/p2p_protocol.hpp"
#include "../include/network/node_discovery.hpp"

int main() {
    try {
        P2PProtocol p2p_protocol(1, "127.0.0.1");
        p2p_protocol.initialize(8080);

        NodeDiscovery node_discovery(1, "127.0.0.1");
        node_discovery.discover_nodes();

        for (size_t node_id : node_discovery.get_known_nodes()) {
            p2p_protocol.add_peer(node_id, node_discovery.get_node_address(node_id));
        }

        std::cout << "P2P network initialized successfully." << std::endl;
        std::cout << "P2P tests passed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "P2P tests failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
