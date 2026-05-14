#pragma once

#include <controllers/dto/Unit.hpp>
#include <views/Unit.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/Unit.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы unit.
inline void to_json(nlohmann::json& json, const UnitEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["code"] = value.code.null() ? nlohmann::json{nullptr} : nlohmann::json{value.code.get()};
  json["name"] = value.name.null() ? nlohmann::json{nullptr} : nlohmann::json{value.name.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы unit.
inline void from_json(const nlohmann::json& json, UnitCreateDto& value) {
  if (json.contains("id") && !json.at("id").is_null()) {
    value.id = json.at("id").get<int>();
  }
  if (json.contains("code") && !json.at("code").is_null()) {
    value.code = json.at("code").get<std::string>();
  }
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
}

/// Читает DTO обновления таблицы unit.
inline void from_json(const nlohmann::json& json, UnitUpdateDto& value) {
  if (json.contains("code") && !json.at("code").is_null()) {
    value.code = json.at("code").get<std::string>();
  }
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

inline nlohmann::json UnitRowPayload(const UnitRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  if (view.code) {
    json["code"] = *view.code;
  } else {
    json["code"] = nullptr;
  }
  if (view.name) {
    json["name"] = *view.name;
  } else {
    json["name"] = nullptr;
  }
  return json;
}

} // namespace detail

inline UnitRowView toView(const fasc::server::persistence::UnitEntity& entity) {
  return UnitRowView{
      entity.id,
      detail::toOptional(entity.code),
      detail::toOptional(entity.name)
  };
}

inline UnitRowsView toView(const std::vector<fasc::server::persistence::UnitEntity>& rows) {
  UnitRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const UnitRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "unit";
  json["data"] = detail::UnitRowPayload(view);
}

inline void to_json(nlohmann::json& json, const UnitRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "unit";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::UnitRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const UnitMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "unit";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
