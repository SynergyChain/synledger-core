#include "cryptography/ecdsa.hpp"
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/ec.h>
#include <openssl/sha.h>
#include <openssl/err.h>
#include <stdexcept>
#include <sstream>
#include <iomanip>

std::pair<std::string, std::string> ECDSA::generate_key_pair() {
    EVP_PKEY* pkey = nullptr;
    EC_KEY* eckey = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if (!eckey || EC_KEY_generate_key(eckey) != 1) {
        EC_KEY_free(eckey);
        throw std::runtime_error("Failed to generate EC key");
    }

    pkey = EVP_PKEY_new();
    if (!pkey || EVP_PKEY_assign_EC_KEY(pkey, eckey) != 1) {
        EC_KEY_free(eckey);
        EVP_PKEY_free(pkey);
        throw std::runtime_error("Failed to assign EC key");
    }

    BIO* pri_bio = BIO_new(BIO_s_mem());
    if (!pri_bio || PEM_write_bio_PrivateKey(pri_bio, pkey, nullptr, nullptr, 0, nullptr, nullptr) != 1) {
        BIO_free(pri_bio);
        EVP_PKEY_free(pkey);
        throw std::runtime_error("Failed to write private key to BIO");
    }
    char* pri_key_data;
    long pri_key_len = BIO_get_mem_data(pri_bio, &pri_key_data);
    std::string private_key(pri_key_data, pri_key_len);

    BIO* pub_bio = BIO_new(BIO_s_mem());
    if (!pub_bio || PEM_write_bio_PUBKEY(pub_bio, pkey) != 1) {
        BIO_free(pri_bio);
        BIO_free(pub_bio);
        EVP_PKEY_free(pkey);
        throw std::runtime_error("Failed to write public key to BIO");
    }
    char* pub_key_data;
    long pub_key_len = BIO_get_mem_data(pub_bio, &pub_key_data);
    std::string public_key(pub_key_data, pub_key_len);

    BIO_free(pri_bio);
    BIO_free(pub_bio);
    EVP_PKEY_free(pkey);

    return { private_key, public_key };
}

std::string ECDSA::sign_message(const std::string& message, const std::string& private_key) {
    EVP_PKEY* pkey = nullptr;
    EVP_MD_CTX* mdctx = nullptr;
    std::string signature;
    unsigned char* sig = nullptr;
    size_t sig_len = 0;

    BIO* pri_bio = BIO_new_mem_buf(private_key.c_str(), -1);
    pkey = PEM_read_bio_PrivateKey(pri_bio, nullptr, nullptr, nullptr);
    BIO_free(pri_bio);

    if (!pkey) {
        throw std::runtime_error("Failed to read private key");
    }

    mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        EVP_PKEY_free(pkey);
        throw std::runtime_error("Failed to create MD context");
    }

    if (EVP_DigestSignInit(mdctx, nullptr, EVP_sha256(), nullptr, pkey) != 1) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(pkey);
        throw std::runtime_error("Failed to initialize DigestSign");
    }

    if (EVP_DigestSignUpdate(mdctx, message.c_str(), message.size()) != 1) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(pkey);
        throw std::runtime_error("Failed to update DigestSign");
    }

    if (EVP_DigestSignFinal(mdctx, nullptr, &sig_len) != 1) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(pkey);
        throw std::runtime_error("Failed to determine signature length");
    }

    sig = (unsigned char*)OPENSSL_malloc(sig_len);
    if (!sig) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(pkey);
        throw std::runtime_error("Failed to allocate memory for signature");
    }

    if (EVP_DigestSignFinal(mdctx, sig, &sig_len) != 1) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(pkey);
        OPENSSL_free(sig);
        throw std::runtime_error("Failed to finalize signature");
    }

    std::stringstream ss;
    for (size_t i = 0; i < sig_len; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)sig[i];
    }

    EVP_MD_CTX_free(mdctx);
    EVP_PKEY_free(pkey);
    OPENSSL_free(sig);

    return ss.str();
}

bool ECDSA::verify_signature(const std::string& message, const std::string& signature, const std::string& public_key) {
    EVP_PKEY* pkey = nullptr;
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
    pkey = PEM_read_bio_PUBKEY(pub_bio, nullptr, nullptr, nullptr);
    BIO_free(pub_bio);

    if (!pkey) {
        OPENSSL_free(sig);
        throw std::runtime_error("Failed to read public key");
    }

    mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        EVP_PKEY_free(pkey);
        OPENSSL_free(sig);
        throw std::runtime_error("Failed to create MD context");
    }

    if (EVP_DigestVerifyInit(mdctx, nullptr, EVP_sha256(), nullptr, pkey) != 1) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(pkey);
        OPENSSL_free(sig);
        throw std::runtime_error("Failed to initialize DigestVerify");
    }

    if (EVP_DigestVerifyUpdate(mdctx, message.c_str(), message.size()) != 1) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(pkey);
        OPENSSL_free(sig);
        throw std::runtime_error("Failed to update DigestVerify");
    }

    verified = (EVP_DigestVerifyFinal(mdctx, sig, sig_len) == 1);

    EVP_MD_CTX_free(mdctx);
    EVP_PKEY_free(pkey);
    OPENSSL_free(sig);

    return verified;
}
