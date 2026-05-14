#pragma once

#include <controllers/dto/Person.hpp>
#include <views/Person.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/Person.hpp>

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

namespace detail {

template <typename T>
inline std::optional<T> toOptional(const odb::nullable<T>& value) {
  if (value.null()) {
    return std::nullopt;
  }
  return value.get();
}

inline nlohmann::json PersonRowPayload(const PersonRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  if (view.firstName) {
    json["first_name"] = *view.firstName;
  } else {
    json["first_name"] = nullptr;
  }
  if (view.lastName) {
    json["last_name"] = *view.lastName;
  } else {
    json["last_name"] = nullptr;
  }
  if (view.middleName) {
    json["middle_name"] = *view.middleName;
  } else {
    json["middle_name"] = nullptr;
  }
  if (view.birthDate) {
    json["birth_date"] = *view.birthDate;
  } else {
    json["birth_date"] = nullptr;
  }
  if (view.phone) {
    json["phone"] = *view.phone;
  } else {
    json["phone"] = nullptr;
  }
  if (view.email) {
    json["email"] = *view.email;
  } else {
    json["email"] = nullptr;
  }
  if (view.address) {
    json["address"] = *view.address;
  } else {
    json["address"] = nullptr;
  }
  return json;
}

} // namespace detail

inline PersonRowView toView(const fasc::server::persistence::PersonEntity& entity) {
  return PersonRowView{
      entity.id,
      detail::toOptional(entity.firstName),
      detail::toOptional(entity.lastName),
      detail::toOptional(entity.middleName),
      detail::toOptional(entity.birthDate),
      detail::toOptional(entity.phone),
      detail::toOptional(entity.email),
      detail::toOptional(entity.address)
  };
}

inline PersonRowsView toView(const std::vector<fasc::server::persistence::PersonEntity>& rows) {
  PersonRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const PersonRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "person";
  json["data"] = detail::PersonRowPayload(view);
}

inline void to_json(nlohmann::json& json, const PersonRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "person";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::PersonRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const PersonMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "person";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
