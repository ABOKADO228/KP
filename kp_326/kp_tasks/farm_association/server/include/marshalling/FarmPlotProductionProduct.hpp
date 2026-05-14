#pragma once

#include <controllers/dto/FarmPlotProductionProduct.hpp>
#include <views/FarmPlotProductionProduct.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/FarmPlotProductionProduct.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы farm_plot_production_product.
inline void to_json(nlohmann::json& json, const FarmPlotProductionProductEntity& value) {
  json = nlohmann::json::object();
  json["product_id"] = value.productId;
  json["farm_plot_id"] = value.farmPlotId;
  json["quantity"] = value.quantity;
  json["production_now"] = value.productionNow;
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы farm_plot_production_product.
inline void from_json(const nlohmann::json& json, FarmPlotProductionProductCreateDto& value) {
  if (json.contains("product_id") && !json.at("product_id").is_null()) {
    value.productId = json.at("product_id").get<int>();
  }
  if (json.contains("farm_plot_id") && !json.at("farm_plot_id").is_null()) {
    value.farmPlotId = json.at("farm_plot_id").get<int>();
  }
  if (json.contains("quantity") && !json.at("quantity").is_null()) {
    value.quantity = json.at("quantity").get<int>();
  }
  if (json.contains("production_now") && !json.at("production_now").is_null()) {
    value.productionNow = json.at("production_now").get<int>();
  }
}

/// Читает DTO обновления таблицы farm_plot_production_product.
inline void from_json(const nlohmann::json& json, FarmPlotProductionProductUpdateDto& value) {
  if (json.contains("quantity") && !json.at("quantity").is_null()) {
    value.quantity = json.at("quantity").get<int>();
  }
  if (json.contains("production_now") && !json.at("production_now").is_null()) {
    value.productionNow = json.at("production_now").get<int>();
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

inline nlohmann::json FarmPlotProductionProductRowPayload(const FarmPlotProductionProductRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["product_id"] = view.productId;
  json["farm_plot_id"] = view.farmPlotId;
  json["quantity"] = view.quantity;
  json["production_now"] = view.productionNow;
  return json;
}

} // namespace detail

inline FarmPlotProductionProductRowView toView(const fasc::server::persistence::FarmPlotProductionProductEntity& entity) {
  return FarmPlotProductionProductRowView{
      entity.productId,
      entity.farmPlotId,
      entity.quantity,
      entity.productionNow
  };
}

inline FarmPlotProductionProductRowsView toView(const std::vector<fasc::server::persistence::FarmPlotProductionProductEntity>& rows) {
  FarmPlotProductionProductRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const FarmPlotProductionProductRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_plot_production_product";
  json["data"] = detail::FarmPlotProductionProductRowPayload(view);
}

inline void to_json(nlohmann::json& json, const FarmPlotProductionProductRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_plot_production_product";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::FarmPlotProductionProductRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const FarmPlotProductionProductMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_plot_production_product";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
