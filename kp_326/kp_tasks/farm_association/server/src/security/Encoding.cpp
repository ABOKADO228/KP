#include <security/Encoding.hpp>

#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace fasc::server::security {
namespace {

struct BioDeleter {
  void operator()(BIO* bio) const {
    BIO_free_all(bio);
  }
};

using BioPtr = std::unique_ptr<BIO, BioDeleter>;

std::string stripBase64Padding(std::string encoded) {
  while (!encoded.empty() && encoded.back() == '=') {
    encoded.pop_back();
  }
  return encoded;
}

std::string restoreBase64Padding(std::string encoded) {
  while (encoded.size() % 4 != 0) {
    encoded.push_back('=');
  }
  return encoded;
}

} // namespace

std::string base64Encode(const unsigned char* data, std::size_t size) {
  BIO* base64 = BIO_new(BIO_f_base64());
  BIO* memory = BIO_new(BIO_s_mem());
  if (base64 == nullptr || memory == nullptr) {
    BIO_free_all(base64);
    BIO_free_all(memory);
    throw std::runtime_error{"Failed to allocate OpenSSL BIO"};
  }

  BIO_set_flags(base64, BIO_FLAGS_BASE64_NO_NL);
  BioPtr chain{BIO_push(base64, memory)};

  if (BIO_write(chain.get(), data, static_cast<int>(size)) <= 0 || BIO_flush(chain.get()) != 1) {
    throw std::runtime_error{"Failed to encode base64"};
  }

  BUF_MEM* buffer = nullptr;
  BIO_get_mem_ptr(chain.get(), &buffer);
  return std::string{buffer->data, buffer->length};
}

std::string base64Decode(std::string_view encoded) {
  std::string input{encoded};
  BIO* base64 = BIO_new(BIO_f_base64());
  BIO* memory = BIO_new_mem_buf(input.data(), static_cast<int>(input.size()));
  if (base64 == nullptr || memory == nullptr) {
    BIO_free_all(base64);
    BIO_free_all(memory);
    throw std::runtime_error{"Failed to allocate OpenSSL BIO"};
  }

  BIO_set_flags(base64, BIO_FLAGS_BASE64_NO_NL);
  BioPtr chain{BIO_push(base64, memory)};

  std::vector<char> decoded(input.size());
  const int decoded_size =
      BIO_read(chain.get(), decoded.data(), static_cast<int>(decoded.size()));
  if (decoded_size < 0) {
    throw std::runtime_error{"Failed to decode base64"};
  }

  return std::string{decoded.data(), static_cast<std::size_t>(decoded_size)};
}

std::string base64UrlEncode(const unsigned char* data, std::size_t size) {
  std::string encoded = stripBase64Padding(base64Encode(data, size));
  for (char& symbol : encoded) {
    if (symbol == '+') {
      symbol = '-';
    } else if (symbol == '/') {
      symbol = '_';
    }
  }
  return encoded;
}

std::string base64UrlEncode(std::string_view data) {
  return base64UrlEncode(reinterpret_cast<const unsigned char*>(data.data()), data.size());
}

} // namespace fasc::server::security
