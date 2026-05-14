#pragma once

#include <controllers/dto/FarmPlotProductionProduct.hpp>
#include <views/FarmPlotProductionProduct.hpp>

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

/// Сериализует view строки таблицы farm_plot_production_product.
inline void to_json(nlohmann::json& json, const FarmPlotProductionProductRowView& view) {
  json = nlohmann::json{{"resource", "farm_plot_production_product"}, {"data", view.data}};
}

/// Сериализует view списка таблицы farm_plot_production_product.
inline void to_json(nlohmann::json& json, const FarmPlotProductionProductRowsView& view) {
  json = nlohmann::json{{"resource", "farm_plot_production_product"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы farm_plot_production_product.
inline void to_json(nlohmann::json& json, const FarmPlotProductionProductMutationView& view) {
  json = nlohmann::json{{"resource", "farm_plot_production_product"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
