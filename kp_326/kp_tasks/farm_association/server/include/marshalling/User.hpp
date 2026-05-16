#pragma once

#include <controllers/dto/User.hpp>
#include <views/Auth.hpp>
#include <views/Error.hpp>
#include <views/User.hpp>

#include <nlohmann/json.hpp>

#include <string>

namespace fasc::server::controllers::dto {

/// Converts a JSON request to a create-user command.
inline void from_json(const nlohmann::json& json, CreateUserCommand& command) {
  command.login = json.at("login").get<std::string>();
  command.password = json.at("password").get<std::string>();
  command.role = json.value("role", std::string{kDefaultUserRole});
}

/// Converts a JSON request to an update-role command.
inline void from_json(const nlohmann::json& json, UpdateUserRoleCommand& command) {
  command.role = json.at("role").get<std::string>();
}

/// Converts a JSON request to a self-registration command.
inline void from_json(const nlohmann::json& json, RegisterUserCommand& command) {
  command.login = json.at("login").get<std::string>();
  command.password = json.at("password").get<std::string>();
}

/// Converts a JSON request to a login command.
inline void from_json(const nlohmann::json& json, LoginUserCommand& command) {
  command.login = json.at("login").get<std::string>();
  command.password = json.at("password").get<std::string>();
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Converts a public error view to JSON.
inline void to_json(nlohmann::json& json, const ErrorView& view) {
  json = nlohmann::json{{"error", view.message}};
}

/// Converts a public user view to JSON.
inline void to_json(nlohmann::json& json, const UserView& view) {
  json = nlohmann::json{
      {"login", view.login},
      {"role", view.role},
  };
}

/// Converts a public user-list view to JSON.
inline void to_json(nlohmann::json& json, const UserListView& view) {
  json = nlohmann::json{{"users", view.users}};
}

/// Converts a public auth view to JSON.
inline void to_json(nlohmann::json& json, const AuthView& view) {
  json = nlohmann::json{
      {"token", view.token},
      {"token_type", view.token_type},
      {"user", view.user},
  };
}

} // namespace fasc::server::views
