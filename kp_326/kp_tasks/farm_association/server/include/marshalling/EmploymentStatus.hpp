#pragma once

#include <controllers/dto/EmploymentStatus.hpp>
#include <views/EmploymentStatus.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/EmploymentStatus.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы employment_status.
inline void to_json(nlohmann::json& json, const EmploymentStatusEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["name"] = value.name.null() ? nlohmann::json{nullptr} : nlohmann::json{value.name.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы employment_status.
inline void from_json(const nlohmann::json& json, EmploymentStatusCreateDto& value) {
  if (json.contains("id") && !json.at("id").is_null()) {
    value.id = json.at("id").get<int>();
  }
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
}

/// Читает DTO обновления таблицы employment_status.
inline void from_json(const nlohmann::json& json, EmploymentStatusUpdateDto& value) {
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
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

inline nlohmann::json EmploymentStatusRowPayload(const EmploymentStatusRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  if (view.name) {
    json["name"] = *view.name;
  } else {
    json["name"] = nullptr;
  }
  return json;
}

} // namespace detail

inline EmploymentStatusRowView toView(const fasc::server::persistence::EmploymentStatusEntity& entity) {
  return EmploymentStatusRowView{
      entity.id,
      detail::toOptional(entity.name)
  };
}

inline EmploymentStatusRowsView toView(const std::vector<fasc::server::persistence::EmploymentStatusEntity>& rows) {
  EmploymentStatusRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const EmploymentStatusRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "employment_status";
  json["data"] = detail::EmploymentStatusRowPayload(view);
}

inline void to_json(nlohmann::json& json, const EmploymentStatusRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "employment_status";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::EmploymentStatusRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const EmploymentStatusMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "employment_status";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
