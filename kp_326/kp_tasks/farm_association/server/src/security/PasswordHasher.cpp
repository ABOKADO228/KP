#include <security/PasswordHasher.hpp>

#include <security/Encoding.hpp>

#include <openssl/evp.h>
#include <openssl/rand.h>

#include <array>
#include <charconv>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace fasc::server::security {
namespace {

std::vector<std::string_view> split(std::string_view value, char delimiter) {
  std::vector<std::string_view> parts;
  std::size_t start = 0;

  for (;;) {
    const std::size_t position = value.find(delimiter, start);
    if (position == std::string_view::npos) {
      parts.push_back(value.substr(start));
      return parts;
    }

    parts.push_back(value.substr(start, position - start));
    start = position + 1;
  }
}

int parse_iterations(std::string_view value) {
  int iterations = 0;
  const auto result = std::from_chars(value.data(), value.data() + value.size(), iterations);
  if (result.ec != std::errc{} || result.ptr != value.data() + value.size() || iterations <= 0) {
    throw std::invalid_argument{"Invalid password hash iterations"};
  }

  return iterations;
}

std::array<unsigned char, PasswordHasher::kHashSize>
derive_hash(std::string_view password, const std::string& salt, int iterations) {
  std::array<unsigned char, PasswordHasher::kHashSize> hash{};
  if (PKCS5_PBKDF2_HMAC(password.data(), static_cast<int>(password.size()),
                        reinterpret_cast<const unsigned char*>(salt.data()),
                        static_cast<int>(salt.size()), iterations, EVP_sha256(),
                        static_cast<int>(hash.size()), hash.data()) != 1) {
    throw std::runtime_error{"Failed to derive password hash"};
  }

  return hash;
}

bool constant_time_equals(const std::string& left, const std::string& right) {
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

std::string PasswordHasher::hash(std::string_view password) const {
  std::array<unsigned char, kSaltSize> salt{};
  if (RAND_bytes(salt.data(), static_cast<int>(salt.size())) != 1) {
    throw std::runtime_error{"Failed to generate password salt"};
  }

  const std::string salt_encoded = base64_encode(salt.data(), salt.size());
  const std::string salt_raw = base64_decode(salt_encoded);
  const auto derived = derive_hash(password, salt_raw, kIterations);
  const std::string hash_encoded = base64_encode(derived.data(), derived.size());

  return "pbkdf2_sha256$" + std::to_string(kIterations) + "$" + salt_encoded + "$" +
         hash_encoded;
}

bool PasswordHasher::verify(std::string_view password, std::string_view encoded_hash) const {
  const std::vector<std::string_view> parts = split(encoded_hash, '$');
  if (parts.size() != 4 || parts[0] != "pbkdf2_sha256") {
    return false;
  }

  try {
    const int iterations = parse_iterations(parts[1]);
    const std::string salt = base64_decode(parts[2]);
    const auto derived = derive_hash(password, salt, iterations);
    const std::string actual = base64_encode(derived.data(), derived.size());
    return constant_time_equals(actual, std::string{parts[3]});
  } catch (const std::exception&) {
    return false;
  }
}

} // namespace fasc::server::security
