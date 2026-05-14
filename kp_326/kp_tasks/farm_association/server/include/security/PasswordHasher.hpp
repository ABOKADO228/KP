#pragma once

#include <string>
#include <string_view>

namespace fasc::server::security {

///Сервис хеширования и проверки паролей.
///@note Использует PBKDF2-SHA256 с солью и фиксированным числом итераций.
class PasswordHasher {
public:
  ///Создает хеш пароля.
  ///@param password пароль в открытом виде.
  ///@returns строка в формате @c pbkdf2_sha256$iterations$salt$hash.
  std::string hash(std::string_view password) const;

  ///Проверяет пароль по ранее сохраненному хешу.
  ///@param password     пароль в открытом виде.
  ///@param encoded_hash сохраненный хеш.
  ///@returns @c true, если пароль соответствует хешу.
  bool verify(std::string_view password, std::string_view encoded_hash) const;

  ///Количество PBKDF2-итераций.
  static constexpr int kIterations = 120000;

  ///Размер случайной соли в байтах.
  static constexpr std::size_t kSaltSize = 16;

  ///Размер итогового PBKDF2-хеша в байтах.
  static constexpr std::size_t kHashSize = 32;
};

} // namespace fasc::server::security
