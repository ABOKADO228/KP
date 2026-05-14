#pragma once

#include <controllers/app/SupplierProductPrice.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/SupplierProductPrice.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using SupplierProductPriceRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::SupplierProductPriceRowsView, fasc::server::views::ErrorView>;
using SupplierProductPriceRowViewResult = fasc::extended::fpp::Result<fasc::server::views::SupplierProductPriceRowView, fasc::server::views::ErrorView>;
using SupplierProductPriceMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::SupplierProductPriceMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы supplier_product_price.
class SupplierProductPriceHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit SupplierProductPriceHttpController(fasc::server::controllers::app::SupplierProductPriceController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  SupplierProductPriceRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  SupplierProductPriceRowViewResult load(const fasc::server::controllers::dto::SupplierProductPriceKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  SupplierProductPriceMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  SupplierProductPriceMutationViewResult update(const fasc::server::controllers::dto::SupplierProductPriceKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  SupplierProductPriceMutationViewResult erase(const fasc::server::controllers::dto::SupplierProductPriceKeyDto& key) const;

private:
  fasc::server::controllers::app::SupplierProductPriceController& controller_;
};

} // namespace fasc::server::controllers::http
