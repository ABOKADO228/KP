#pragma once

#include <controllers/dto/FarmOwner.hpp>
#include <views/FarmOwner.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/FarmOwner.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы farm_owner.
inline void to_json(nlohmann::json& json, const FarmOwnerEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["person_id"] = value.personId;
  json["status"] = value.status.null() ? nlohmann::json{nullptr} : nlohmann::json{value.status.get()};
  json["rating"] = value.rating.null() ? nlohmann::json{nullptr} : nlohmann::json{value.rating.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы farm_owner.
inline void from_json(const nlohmann::json& json, FarmOwnerCreateDto& value) {
  if (json.contains("person_id") && !json.at("person_id").is_null()) {
    value.personId = json.at("person_id").get<int>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<std::string>();
  }
  if (json.contains("rating") && !json.at("rating").is_null()) {
    value.rating = json.at("rating").get<double>();
  }
}

/// Читает DTO обновления таблицы farm_owner.
inline void from_json(const nlohmann::json& json, FarmOwnerUpdateDto& value) {
  if (json.contains("person_id") && !json.at("person_id").is_null()) {
    value.personId = json.at("person_id").get<int>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<std::string>();
  }
  if (json.contains("rating") && !json.at("rating").is_null()) {
    value.rating = json.at("rating").get<double>();
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

inline nlohmann::json FarmOwnerRowPayload(const FarmOwnerRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  json["person_id"] = view.personId;
  if (view.status) {
    json["status"] = *view.status;
  } else {
    json["status"] = nullptr;
  }
  if (view.rating) {
    json["rating"] = *view.rating;
  } else {
    json["rating"] = nullptr;
  }
  return json;
}

} // namespace detail

inline FarmOwnerRowView toView(const fasc::server::persistence::FarmOwnerEntity& entity) {
  return FarmOwnerRowView{
      entity.id,
      entity.personId,
      detail::toOptional(entity.status),
      detail::toOptional(entity.rating)
  };
}

inline FarmOwnerRowsView toView(const std::vector<fasc::server::persistence::FarmOwnerEntity>& rows) {
  FarmOwnerRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const FarmOwnerRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_owner";
  json["data"] = detail::FarmOwnerRowPayload(view);
}

inline void to_json(nlohmann::json& json, const FarmOwnerRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_owner";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::FarmOwnerRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const FarmOwnerMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_owner";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
