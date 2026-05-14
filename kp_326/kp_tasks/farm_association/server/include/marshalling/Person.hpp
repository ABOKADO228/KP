#pragma once

#include <controllers/dto/Person.hpp>
#include <views/Person.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы person.
inline void to_json(nlohmann::json& json, const PersonEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["first_name"] = value.firstName.null() ? nlohmann::json{nullptr} : nlohmann::json{value.firstName.get()};
  json["last_name"] = value.lastName.null() ? nlohmann::json{nullptr} : nlohmann::json{value.lastName.get()};
  json["middle_name"] = value.middleName.null() ? nlohmann::json{nullptr} : nlohmann::json{value.middleName.get()};
  json["birth_date"] = value.birthDate.null() ? nlohmann::json{nullptr} : nlohmann::json{value.birthDate.get()};
  json["phone"] = value.phone.null() ? nlohmann::json{nullptr} : nlohmann::json{value.phone.get()};
  json["email"] = value.email.null() ? nlohmann::json{nullptr} : nlohmann::json{value.email.get()};
  json["address"] = value.address.null() ? nlohmann::json{nullptr} : nlohmann::json{value.address.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы person.
inline void from_json(const nlohmann::json& json, PersonCreateDto& value) {
  if (json.contains("first_name") && !json.at("first_name").is_null()) {
    value.firstName = json.at("first_name").get<std::string>();
  }
  if (json.contains("last_name") && !json.at("last_name").is_null()) {
    value.lastName = json.at("last_name").get<std::string>();
  }
  if (json.contains("middle_name") && !json.at("middle_name").is_null()) {
    value.middleName = json.at("middle_name").get<std::string>();
  }
  if (json.contains("birth_date") && !json.at("birth_date").is_null()) {
    value.birthDate = json.at("birth_date").get<std::string>();
  }
  if (json.contains("phone") && !json.at("phone").is_null()) {
    value.phone = json.at("phone").get<std::string>();
  }
  if (json.contains("email") && !json.at("email").is_null()) {
    value.email = json.at("email").get<std::string>();
  }
  if (json.contains("address") && !json.at("address").is_null()) {
    value.address = json.at("address").get<std::string>();
  }
}

/// Читает DTO обновления таблицы person.
inline void from_json(const nlohmann::json& json, PersonUpdateDto& value) {
  if (json.contains("first_name") && !json.at("first_name").is_null()) {
    value.firstName = json.at("first_name").get<std::string>();
  }
  if (json.contains("last_name") && !json.at("last_name").is_null()) {
    value.lastName = json.at("last_name").get<std::string>();
  }
  if (json.contains("middle_name") && !json.at("middle_name").is_null()) {
    value.middleName = json.at("middle_name").get<std::string>();
  }
  if (json.contains("birth_date") && !json.at("birth_date").is_null()) {
    value.birthDate = json.at("birth_date").get<std::string>();
  }
  if (json.contains("phone") && !json.at("phone").is_null()) {
    value.phone = json.at("phone").get<std::string>();
  }
  if (json.contains("email") && !json.at("email").is_null()) {
    value.email = json.at("email").get<std::string>();
  }
  if (json.contains("address") && !json.at("address").is_null()) {
    value.address = json.at("address").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы person.
inline void to_json(nlohmann::json& json, const PersonRowView& view) {
  json = nlohmann::json{{"resource", "person"}, {"data", view.data}};
}

/// Сериализует view списка таблицы person.
inline void to_json(nlohmann::json& json, const PersonRowsView& view) {
  json = nlohmann::json{{"resource", "person"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы person.
inline void to_json(nlohmann::json& json, const PersonMutationView& view) {
  json = nlohmann::json{{"resource", "person"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
