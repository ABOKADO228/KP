#pragma once

#include <controllers/dto/AssociationMember.hpp>
#include <views/AssociationMember.hpp>

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
    value.associationId = json.at("association_id").get<int>();
  }
  if (json.contains("person_id") && !json.at("person_id").is_null()) {
    value.personId = json.at("person_id").get<int>();
  }
  if (json.contains("membership_number") && !json.at("membership_number").is_null()) {
    value.membershipNumber = json.at("membership_number").get<std::string>();
  }
  if (json.contains("joined_date") && !json.at("joined_date").is_null()) {
    value.joinedDate = json.at("joined_date").get<std::string>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<std::string>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

/// Читает DTO обновления таблицы association_member.
inline void from_json(const nlohmann::json& json, AssociationMemberUpdateDto& value) {
  if (json.contains("association_id") && !json.at("association_id").is_null()) {
    value.associationId = json.at("association_id").get<int>();
  }
  if (json.contains("person_id") && !json.at("person_id").is_null()) {
    value.personId = json.at("person_id").get<int>();
  }
  if (json.contains("membership_number") && !json.at("membership_number").is_null()) {
    value.membershipNumber = json.at("membership_number").get<std::string>();
  }
  if (json.contains("joined_date") && !json.at("joined_date").is_null()) {
    value.joinedDate = json.at("joined_date").get<std::string>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<std::string>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы association_member.
inline void to_json(nlohmann::json& json, const AssociationMemberRowView& view) {
  json = nlohmann::json{{"resource", "association_member"}, {"data", view.data}};
}

/// Сериализует view списка таблицы association_member.
inline void to_json(nlohmann::json& json, const AssociationMemberRowsView& view) {
  json = nlohmann::json{{"resource", "association_member"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы association_member.
inline void to_json(nlohmann::json& json, const AssociationMemberMutationView& view) {
  json = nlohmann::json{{"resource", "association_member"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
