#ifndef CRYPTO_HPP
#define CRYPTO_HPP

#include <string>

class Crypto {
public:
    static std::string hash(const std::string& data);
    static std::string sign(const std::string& message, const std::string& private_key);
    static bool verify_signature(const std::string& message, const std::string& signature, const std::string& public_key);
};

#endif // CRYPTO_HPP
