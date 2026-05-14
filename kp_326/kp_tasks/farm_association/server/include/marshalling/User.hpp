#pragma once

#include <controllers/dto/User.hpp>
#include <views/Auth.hpp>
#include <views/Error.hpp>
#include <views/User.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::controllers::dto {

/// Преобразует JSON-запрос в команду создания пользователя.
///@param json    JSON-объект с входными данными.
///@param command команда, в которую будут записаны распарсенные данные.
///@throws @c nlohmann::json::exception, если обязательное поле отсутствует или имеет неверный тип.
inline void from_json(const nlohmann::json& json, CreateUserCommand& command) {
  command.name = json.at("name").get<std::string>();
  command.password = json.at("password").get<std::string>();
}

/// Преобразует JSON-запрос в команду регистрации пользователя.
///@param json    JSON-объект с входными данными.
///@param command команда, в которую будут записаны распарсенные данные.
///@throws @c nlohmann::json::exception, если обязательное поле отсутствует или имеет неверный тип.
inline void from_json(const nlohmann::json& json, RegisterUserCommand& command) {
  command.name = json.at("name").get<std::string>();
  command.password = json.at("password").get<std::string>();
}

/// Преобразует JSON-запрос в команду авторизации пользователя.
///@param json    JSON-объект с входными данными.
///@param command команда, в которую будут записаны распарсенные данные.
///@throws @c nlohmann::json::exception, если обязательное поле отсутствует или имеет неверный тип.
inline void from_json(const nlohmann::json& json, LoginUserCommand& command) {
  command.name = json.at("name").get<std::string>();
  command.password = json.at("password").get<std::string>();
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Преобразует публичный error view в JSON.
///@param json JSON-объект, в который будет записано представление.
///@param view публичное представление ошибки.
inline void to_json(nlohmann::json& json, const ErrorView& view) {
  json = nlohmann::json{{"error", view.message}};
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

/// Преобразует публичный view авторизации в JSON.
///@param json JSON-объект, в который будет записано представление.
///@param view публичное представление авторизации.
inline void to_json(nlohmann::json& json, const AuthView& view) {
  json = nlohmann::json{
      {"token", view.token},
      {"token_type", view.token_type},
      {"user", view.user},
  };
}

} // namespace fasc::server::views
