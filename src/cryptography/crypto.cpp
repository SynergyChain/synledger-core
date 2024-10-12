#include "cryptography/crypto.hpp"
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/sha.h>
#include <stdexcept>
#include <sstream>
#include <iomanip>

std::string Crypto::hash(const std::string& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();

    if (!mdctx) {
        throw std::runtime_error("Failed to create EVP_MD_CTX");
    }

    if (1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL)) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Failed to initialize EVP digest");
    }

    if (1 != EVP_DigestUpdate(mdctx, data.c_str(), data.size())) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Failed to update digest");
    }

    if (1 != EVP_DigestFinal_ex(mdctx, hash, NULL)) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Failed to finalize digest");
    }

    EVP_MD_CTX_free(mdctx);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}

std::string Crypto::sign(const std::string& message, const std::string& private_key) {
    EVP_PKEY* key = nullptr;
    EVP_MD_CTX* mdctx = nullptr;
    size_t sig_len = 0;
    unsigned char* sig = nullptr;
    std::string signature;

    BIO* bio = BIO_new_mem_buf(private_key.c_str(), -1);
    key = PEM_read_bio_PrivateKey(bio, nullptr, nullptr, nullptr);
    BIO_free(bio);

    if (!key) {
        throw std::runtime_error("Failed to read private key");
    }

    mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        EVP_PKEY_free(key);
        throw std::runtime_error("Failed to create MD context");
    }

    if (EVP_DigestSignInit(mdctx, nullptr, EVP_sha256(), nullptr, key) != 1) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(key);
        throw std::runtime_error("Failed to initialize DigestSign");
    }

    if (EVP_DigestSignUpdate(mdctx, message.c_str(), message.size()) != 1) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(key);
        throw std::runtime_error("Failed to update DigestSign");
    }

    if (EVP_DigestSignFinal(mdctx, nullptr, &sig_len) != 1) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(key);
        throw std::runtime_error("Failed to determine signature length");
    }

    sig = (unsigned char*)OPENSSL_malloc(sig_len);
    if (!sig) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(key);
        throw std::runtime_error("Failed to allocate memory for signature");
    }

    if (EVP_DigestSignFinal(mdctx, sig, &sig_len) != 1) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(key);
        OPENSSL_free(sig);
        throw std::runtime_error("Failed to finalize signature");
    }

    std::stringstream ss;
    for (size_t i = 0; i < sig_len; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)sig[i];
    }

    EVP_MD_CTX_free(mdctx);
    EVP_PKEY_free(key);
    OPENSSL_free(sig);

    return ss.str();
}

bool Crypto::verify_signature(const std::string& message, const std::string& signature, const std::string& public_key) {
    EVP_PKEY* key = nullptr;
    EVP_MD_CTX* mdctx = nullptr;
    unsigned char* sig = nullptr;
    bool verified = false;
    size_t sig_len = signature.length() / 2;

    sig = (unsigned char*)OPENSSL_malloc(sig_len);
    if (!sig) {
        throw std::runtime_error("Failed to allocate memory for signature");
    }

    for (size_t i = 0; i < sig_len; ++i) {
        sscanf(&signature[i * 2], "%2hhx", &sig[i]);
    }

    BIO* pub_bio = BIO_new_mem_buf(public_key.c_str(), -1);
    key = PEM_read_bio_PUBKEY(pub_bio, nullptr, nullptr, nullptr);
    BIO_free(pub_bio);

    if (!key) {
        OPENSSL_free(sig);
        throw std::runtime_error("Failed to read public key");
    }

    mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        EVP_PKEY_free(key);
        OPENSSL_free(sig);
        throw std::runtime_error("Failed to create MD context");
    }

    if (EVP_DigestVerifyInit(mdctx, nullptr, EVP_sha256(), nullptr, key) != 1) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(key);
        OPENSSL_free(sig);
        throw std::runtime_error("Failed to initialize DigestVerify");
    }

    if (EVP_DigestVerifyUpdate(mdctx, message.c_str(), message.size()) != 1) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(key);
        OPENSSL_free(sig);
        throw std::runtime_error("Failed to update DigestVerify");
    }

    verified = (EVP_DigestVerifyFinal(mdctx, sig, sig_len) == 1);

    EVP_MD_CTX_free(mdctx);
    EVP_PKEY_free(key);
    OPENSSL_free(sig);

    return verified;
}
