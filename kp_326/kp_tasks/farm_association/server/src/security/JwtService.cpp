#include <security/JwtService.hpp>

#include <controllers/dto/User.hpp>
#include <security/Encoding.hpp>

#include <openssl/evp.h>
#include <openssl/hmac.h>

#include <chrono>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

#include <nlohmann/json.hpp>

namespace fasc::server::security {

using fasc::server::controllers::dto::UserDto;

namespace {

std::string hmacSha256(std::string_view secret, std::string_view data) {
  unsigned int digest_size = 0;
  unsigned char digest[EVP_MAX_MD_SIZE]{};

  if (HMAC(EVP_sha256(), secret.data(), static_cast<int>(secret.size()),
           reinterpret_cast<const unsigned char*>(data.data()), data.size(), digest,
           &digest_size) == nullptr) {
    throw std::runtime_error{"Failed to sign JWT"};
  }

  return base64UrlEncode(digest, digest_size);
}

bool secureEquals(std::string_view left, std::string_view right) {
  if (left.size() != right.size()) {
    return false;
  }

  unsigned char diff = 0;
  for (std::size_t i = 0; i < left.size(); ++i) {
    diff |= static_cast<unsigned char>(left[i] ^ right[i]);
  }
  return diff == 0;
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
      {"sub", user.login},
      {"role", user.role},
      {"iat", issued_at},
      {"exp", expires_at},
  };

  const std::string encoded_header = base64UrlEncode(header.dump());
  const std::string encoded_payload = base64UrlEncode(payload.dump());
  const std::string signing_input = encoded_header + "." + encoded_payload;

  return signing_input + "." + hmacSha256(secret_, signing_input);
}

std::optional<UserDto> JwtService::verify(std::string_view token) const {
  const std::size_t first_dot = token.find('.');
  if (first_dot == std::string_view::npos) {
    return std::nullopt;
  }

  const std::size_t second_dot = token.find('.', first_dot + 1);
  if (second_dot == std::string_view::npos ||
      token.find('.', second_dot + 1) != std::string_view::npos) {
    return std::nullopt;
  }

  const std::string_view encoded_header = token.substr(0, first_dot);
  const std::string_view encoded_payload =
      token.substr(first_dot + 1, second_dot - first_dot - 1);
  const std::string_view encoded_signature = token.substr(second_dot + 1);
  const std::string signing_input{token.substr(0, second_dot)};

  if (!secureEquals(encoded_signature, hmacSha256(secret_, signing_input))) {
    return std::nullopt;
  }

  try {
    const auto header = nlohmann::json::parse(base64UrlDecode(encoded_header));
    if (header.value("alg", std::string{}) != "HS256") {
      return std::nullopt;
    }

    const auto payload = nlohmann::json::parse(base64UrlDecode(encoded_payload));
    if (payload.value("iss", std::string{}) != issuer_) {
      return std::nullopt;
    }

    const auto expires_at = payload.at("exp").get<long long>();
    const auto now = std::chrono::duration_cast<std::chrono::seconds>(
                         std::chrono::system_clock::now().time_since_epoch())
                         .count();
    if (expires_at < now) {
      return std::nullopt;
    }

    UserDto user;
    user.login = payload.at("sub").get<std::string>();
    user.role = payload.at("role").get<std::string>();
    if (user.login.empty() || user.role.empty()) {
      return std::nullopt;
    }

    return user;
  } catch (const std::exception&) {
    return std::nullopt;
  }
}

} // namespace fasc::server::security
