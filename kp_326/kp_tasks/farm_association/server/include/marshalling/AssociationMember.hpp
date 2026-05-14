#pragma once

#include <marshalling/Nullable.hpp>

#include <domain/Types.hpp>

#include <controllers/dto/AssociationMember.hpp>
#include <views/AssociationMember.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/AssociationMember.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы association_member.
inline void to_json(nlohmann::json& json, const AssociationMemberEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["association_id"] = value.associationId;
  json["person_id"] = value.personId;
  json["membership_number"] = value.membershipNumber.null() ? nlohmann::json{nullptr} : nlohmann::json{value.membershipNumber.get()};
  json["joined_date"] = value.joinedDate;
  json["status"] = value.status.null() ? nlohmann::json{nullptr} : nlohmann::json{value.status.get()};
  json["notes"] = value.notes.null() ? nlohmann::json{nullptr} : nlohmann::json{value.notes.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы association_member.
inline void from_json(const nlohmann::json& json, AssociationMemberCreateDto& value) {
  if (json.contains("association_id") && !json.at("association_id").is_null()) {
    value.associationId = json.at("association_id").get<std::uint64_t>();
  }
  if (json.contains("person_id") && !json.at("person_id").is_null()) {
    value.personId = json.at("person_id").get<std::uint64_t>();
  }
  if (json.contains("membership_number") && !json.at("membership_number").is_null()) {
    value.membershipNumber = json.at("membership_number").get<std::string>();
  }
  if (json.contains("joined_date") && !json.at("joined_date").is_null()) {
    value.joinedDate = json.at("joined_date").get<fasc::server::domain::Date>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<fasc::server::domain::AssociationMemberStatus>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

/// Читает DTO обновления таблицы association_member.
inline void from_json(const nlohmann::json& json, AssociationMemberUpdateDto& value) {
  if (json.contains("association_id") && !json.at("association_id").is_null()) {
    value.associationId = json.at("association_id").get<std::uint64_t>();
  }
  if (json.contains("person_id") && !json.at("person_id").is_null()) {
    value.personId = json.at("person_id").get<std::uint64_t>();
  }
  if (json.contains("membership_number") && !json.at("membership_number").is_null()) {
    value.membershipNumber = json.at("membership_number").get<std::string>();
  }
  if (json.contains("joined_date") && !json.at("joined_date").is_null()) {
    value.joinedDate = json.at("joined_date").get<fasc::server::domain::Date>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<fasc::server::domain::AssociationMemberStatus>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

namespace detail {

inline nlohmann::json AssociationMemberRowPayload(const AssociationMemberRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  json["association_id"] = view.associationId;
  json["person_id"] = view.personId;
  if (view.membershipNumber) {
    json["membership_number"] = *view.membershipNumber;
  } else {
    json["membership_number"] = nullptr;
  }
  json["joined_date"] = view.joinedDate;
  if (view.status) {
    json["status"] = *view.status;
  } else {
    json["status"] = nullptr;
  }
  if (view.notes) {
    json["notes"] = *view.notes;
  } else {
    json["notes"] = nullptr;
  }
  return json;
}

} // namespace detail

inline AssociationMemberRowView toView(const fasc::server::persistence::AssociationMemberEntity& entity) {
  return AssociationMemberRowView{
      entity.id,
      entity.associationId,
      entity.personId,
      detail::toOptional(entity.membershipNumber),
      entity.joinedDate,
      detail::toOptional(entity.status),
      detail::toOptional(entity.notes)
  };
}

inline AssociationMemberRowsView toView(const std::vector<fasc::server::persistence::AssociationMemberEntity>& rows) {
  AssociationMemberRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const AssociationMemberRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "association_member";
  json["data"] = detail::AssociationMemberRowPayload(view);
}

inline void to_json(nlohmann::json& json, const AssociationMemberRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "association_member";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::AssociationMemberRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const AssociationMemberMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "association_member";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
