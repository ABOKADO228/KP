#pragma once

#include <marshalling/Nullable.hpp>

#include <domain/Types.hpp>

#include <controllers/dto/FarmPlotConsumptionProduct.hpp>
#include <views/FarmPlotConsumptionProduct.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/FarmPlotConsumptionProduct.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы farm_plot_consumption_product.
inline void to_json(nlohmann::json& json, const FarmPlotConsumptionProductEntity& value) {
  json = nlohmann::json::object();
  json["product_id"] = value.productId;
  json["farm_plot_id"] = value.farmPlotId;
  json["quantity"] = value.quantity;
  json["consumption_now"] = value.consumptionNow;
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы farm_plot_consumption_product.
inline void from_json(const nlohmann::json& json, FarmPlotConsumptionProductCreateDto& value) {
  if (json.contains("product_id") && !json.at("product_id").is_null()) {
    value.productId = json.at("product_id").get<std::uint64_t>();
  }
  if (json.contains("farm_plot_id") && !json.at("farm_plot_id").is_null()) {
    value.farmPlotId = json.at("farm_plot_id").get<std::uint64_t>();
  }
  if (json.contains("quantity") && !json.at("quantity").is_null()) {
    value.quantity = json.at("quantity").get<int>();
  }
  if (json.contains("consumption_now") && !json.at("consumption_now").is_null()) {
    value.consumptionNow = json.at("consumption_now").get<int>();
  }
}

/// Читает DTO обновления таблицы farm_plot_consumption_product.
inline void from_json(const nlohmann::json& json, FarmPlotConsumptionProductUpdateDto& value) {
  if (json.contains("quantity") && !json.at("quantity").is_null()) {
    value.quantity = json.at("quantity").get<int>();
  }
  if (json.contains("consumption_now") && !json.at("consumption_now").is_null()) {
    value.consumptionNow = json.at("consumption_now").get<int>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

namespace detail {

inline nlohmann::json FarmPlotConsumptionProductRowPayload(const FarmPlotConsumptionProductRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["product_id"] = view.productId;
  json["farm_plot_id"] = view.farmPlotId;
  json["quantity"] = view.quantity;
  json["consumption_now"] = view.consumptionNow;
  return json;
}

} // namespace detail

inline FarmPlotConsumptionProductRowView toView(const fasc::server::persistence::FarmPlotConsumptionProductEntity& entity) {
  return FarmPlotConsumptionProductRowView{
      entity.productId,
      entity.farmPlotId,
      entity.quantity,
      entity.consumptionNow
  };
}

inline FarmPlotConsumptionProductRowsView toView(const std::vector<fasc::server::persistence::FarmPlotConsumptionProductEntity>& rows) {
  FarmPlotConsumptionProductRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const FarmPlotConsumptionProductRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_plot_consumption_product";
  json["data"] = detail::FarmPlotConsumptionProductRowPayload(view);
}

inline void to_json(nlohmann::json& json, const FarmPlotConsumptionProductRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_plot_consumption_product";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::FarmPlotConsumptionProductRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const FarmPlotConsumptionProductMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_plot_consumption_product";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
