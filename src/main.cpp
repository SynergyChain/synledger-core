#include "network/p2p_protocol.hpp"
#include "network/node_discovery.hpp"
#include "consensus/posyg_engine.hpp"
#include "ledger/ledger.hpp"
#include "governance/governance.hpp"
#include "subnet/subnet_manager.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char* argv[]) {
    size_t node_id = 1;
    int port = 8080;

    // Handle command-line arguments
    if (argc > 2) {
        node_id = std::stoul(argv[1]);
        port = std::stoi(argv[2]);
    } else {
        std::cerr << "Usage: " << argv[0] << " <node_id> <port>" << std::endl;
        return 1;
    }

    try {
        // Initialize PoSygEngine with 10 participants
        PoSygEngine posyg_engine(10);

        // Initialize P2P Protocol
        std::string network_address = "127.0.0.1";
        P2PProtocol p2p_protocol(node_id, network_address);
        p2p_protocol.initialize(port);

        // Initialize Node Discovery
        NodeDiscovery node_discovery(node_id, network_address);
        node_discovery.discover_nodes();
        for (size_t known_node_id : node_discovery.get_known_nodes()) {
            p2p_protocol.add_peer(known_node_id, node_discovery.get_node_address(known_node_id));
        }

        // Initialize Ledger
        size_t initial_difficulty = 3;
        Ledger ledger(initial_difficulty);

        // Initialize Governance
        Governance governance(posyg_engine);

        // Initialize SubnetManager
        SubnetManager subnet_manager(5);
        subnet_manager.assign_node_to_subnet(node_id);

        // Main loop for node operation
        while (true) {
            // Run consensus cycle
            posyg_engine.run_cycle();

            // Generate a new block
            Block new_block = ledger.get_latest_block();
            if (ledger.validate_chain()) {
                ledger.add_block(new_block);
                p2p_protocol.send_message(2, "New block added to the chain: " + new_block.get_block_hash());
            }

            // Governance process example
            governance.create_proposal("Increase block reward");
            governance.vote(1, true, 0);  // Assuming participant 0 votes in favor
            governance.finalize_proposal(1);

            // Log the state of the chain
            ledger.log_chain_state();

            // Perform subnet rebalancing periodically
            subnet_manager.rebalance_subnets();

            // Wait before the next cycle
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
