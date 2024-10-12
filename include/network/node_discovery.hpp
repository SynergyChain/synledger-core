#ifndef NODE_DISCOVERY_HPP
#define NODE_DISCOVERY_HPP

#include <string>
#include <vector>
#include <unordered_map>

class NodeDiscovery {
public:
    NodeDiscovery(size_t node_id, const std::string& network_address);
    void discover_nodes();
    void add_node(size_t node_id, const std::string& address);
    std::vector<size_t> get_known_nodes() const;
    std::string get_node_address(size_t node_id) const;

private:
    size_t node_id;
    std::string network_address;
    std::unordered_map<size_t, std::string> known_nodes;
};

#endif // NODE_DISCOVERY_HPP
