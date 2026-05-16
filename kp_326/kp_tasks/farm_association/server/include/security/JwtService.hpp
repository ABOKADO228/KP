#pragma once

#include <controllers/dto/User.hpp>

#include <chrono>
#include <optional>
#include <string>
#include <string_view>

namespace fasc::server::security {

///Сервис выпуска JWT для авторизованных пользователей.
///@note Формирует HMAC-SHA256 токен без хранения состояния на сервере.
class JwtService {
public:
  ///Создает сервис выпуска токенов.
  ///@param secret секрет подписи JWT.
  ///@param issuer значение claims @c iss.
  ///@param ttl время жизни токена.
  ///@throws @c std::invalid_argument, если @p secret пустой.
  JwtService(std::string secret,
             std::string issuer = "farm-association-server",
             std::chrono::seconds ttl = std::chrono::hours{24});

  ///Выпускает JWT для пользователя.
  ///@param user пользователь application layer.
  ///@returns строка JWT.
  std::string issue(const fasc::server::controllers::dto::UserDto& user) const;

  std::optional<fasc::server::controllers::dto::UserDto> verify(std::string_view token) const;

private:
  ///Секрет подписи JWT.
  std::string secret_;

  ///Issuer, записываемый в payload токена.
  std::string issuer_;

  ///Время жизни токена.
  std::chrono::seconds ttl_;
};

} // namespace fasc::server::security
