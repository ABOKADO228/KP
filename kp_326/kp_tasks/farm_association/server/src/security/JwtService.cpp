#include <security/JwtService.hpp>

#include <controllers/dto/User.hpp>
#include <security/Encoding.hpp>

#include <openssl/evp.h>
#include <openssl/hmac.h>

#include <chrono>
#include <stdexcept>
#include <string>

#include <nlohmann/json.hpp>

namespace fasc::server::security {
namespace {

std::string hmac_sha256(std::string_view secret, std::string_view data) {
  unsigned int digest_size = 0;
  unsigned char digest[EVP_MAX_MD_SIZE]{};

  if (HMAC(EVP_sha256(), secret.data(), static_cast<int>(secret.size()),
           reinterpret_cast<const unsigned char*>(data.data()), data.size(), digest,
           &digest_size) == nullptr) {
    throw std::runtime_error{"Failed to sign JWT"};
  }

  return base64_url_encode(digest, digest_size);
}

} // namespace

JwtService::JwtService(std::string secret, std::string issuer, std::chrono::seconds ttl)
    : secret_(std::move(secret)), issuer_(std::move(issuer)), ttl_(ttl) {
  if (secret_.empty()) {
    throw std::invalid_argument{"JWT secret must not be empty"};
  }
}

std::string JwtService::issue(const UserDto& user) const {
  const auto now = std::chrono::system_clock::now();
  const auto issued_at = std::chrono::duration_cast<std::chrono::seconds>(
                             now.time_since_epoch())
                             .count();
  const auto expires_at = issued_at + ttl_.count();

  const nlohmann::json header = {
      {"alg", "HS256"},
      {"typ", "JWT"},
  };

  const nlohmann::json payload = {
      {"iss", issuer_},
      {"sub", std::to_string(user.id)},
      {"name", user.name},
      {"iat", issued_at},
      {"exp", expires_at},
  };

  const std::string encoded_header = base64_url_encode(header.dump());
  const std::string encoded_payload = base64_url_encode(payload.dump());
  const std::string signing_input = encoded_header + "." + encoded_payload;

  return signing_input + "." + hmac_sha256(secret_, signing_input);
}

} // namespace fasc::server::security
