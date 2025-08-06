#include "util/file.h"

#include "util/string.h"

#include <openssl/evp.h>

constexpr std::size_t BUFFER_SIZE = 4096;

namespace allay_launcher::util::file {

std::string calc_sha256(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw IOException(path.string(), "unable to open file.");
    }

    auto* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        throw CryptoException("EVP_MD_CTX_new");
    }

    if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(ctx);
        throw CryptoException("EVP_DigestInit_ex");
    }

    char buffer[BUFFER_SIZE];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        if (EVP_DigestUpdate(ctx, buffer, file.gcount()) != 1) {
            EVP_MD_CTX_free(ctx);
            throw CryptoException("EVP_DigestUpdate");
        }
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int  len = 0;
    if (EVP_DigestFinal_ex(ctx, hash, &len) != 1) {
        EVP_MD_CTX_free(ctx);
        throw CryptoException("EVP_DigestFinal_ex");
    }

    EVP_MD_CTX_free(ctx);

    return string::uchar_array_to_hex(hash, len);
}

} // namespace allay_launcher::util::file
