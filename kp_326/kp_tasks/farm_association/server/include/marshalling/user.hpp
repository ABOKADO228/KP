#pragma once

#include <controllers/dto/User.hpp>
#include <views/User.hpp>

#include <nlohmann/json.hpp>
namespace fasc::server::marshalling {
/// Преобразует JSON-запрос в команду создания пользователя.
///@param json    JSON-объект с входными данными.
///@param command команда, в которую будут записаны распарсенные данные.
///@throws @c nlohmann::json::exception, если поле @c name отсутствует или имеет неверный тип.
inline void from_json(const nlohmann::json& json, CreateUserCommand& command) {
  command.name = json.at("name").get<std::string>();
  command.password = json.at("password").get<std::string>();
}

inline void from_json(const nlohmann::json& json, RegisterUserCommand& command) {
  command.name = json.at("name").get<std::string>();
  command.password = json.at("password").get<std::string>();
}

inline void from_json(const nlohmann::json& json, LoginUserCommand& command) {
  command.name = json.at("name").get<std::string>();
  command.password = json.at("password").get<std::string>();
}

/// Преобразует публичный view пользователя в JSON.
///@note Используется для ответа наружу, а не для persistence-сериализации.
///@param json JSON-объект, в который будет записано представление.
///@param view публичное представление пользователя.
inline void to_json(nlohmann::json& json, const UserView& view) {
  json = nlohmann::json{
      {"id", view.id},
      {"name", view.name},
  };
}
} // namespace fasc::server::marshalling
