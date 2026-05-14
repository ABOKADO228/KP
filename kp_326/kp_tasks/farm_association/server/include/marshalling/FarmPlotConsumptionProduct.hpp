#pragma once

#include <controllers/dto/FarmPlotConsumptionProduct.hpp>
#include <views/FarmPlotConsumptionProduct.hpp>

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
    value.productId = json.at("product_id").get<int>();
  }
  if (json.contains("farm_plot_id") && !json.at("farm_plot_id").is_null()) {
    value.farmPlotId = json.at("farm_plot_id").get<int>();
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

/// Сериализует view строки таблицы farm_plot_consumption_product.
inline void to_json(nlohmann::json& json, const FarmPlotConsumptionProductRowView& view) {
  json = nlohmann::json{{"resource", "farm_plot_consumption_product"}, {"data", view.data}};
}

/// Сериализует view списка таблицы farm_plot_consumption_product.
inline void to_json(nlohmann::json& json, const FarmPlotConsumptionProductRowsView& view) {
  json = nlohmann::json{{"resource", "farm_plot_consumption_product"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы farm_plot_consumption_product.
inline void to_json(nlohmann::json& json, const FarmPlotConsumptionProductMutationView& view) {
  json = nlohmann::json{{"resource", "farm_plot_consumption_product"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
