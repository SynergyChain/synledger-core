#ifndef ECDSA_HPP
#define ECDSA_HPP

#include <string>
#include <utility>

class ECDSA {
public:
    static std::pair<std::string, std::string> generate_key_pair();
    static std::string sign_message(const std::string& message, const std::string& private_key);
    static bool verify_signature(const std::string& message, const std::string& signature, const std::string& public_key);
};

#endif // ECDSA_HPP
