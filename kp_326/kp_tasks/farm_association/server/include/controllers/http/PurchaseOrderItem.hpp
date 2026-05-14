#pragma once

#include <controllers/app/PurchaseOrderItem.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/PurchaseOrderItem.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using PurchaseOrderItemRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::PurchaseOrderItemRowsView, fasc::server::views::ErrorView>;
using PurchaseOrderItemRowViewResult = fasc::extended::fpp::Result<fasc::server::views::PurchaseOrderItemRowView, fasc::server::views::ErrorView>;
using PurchaseOrderItemMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::PurchaseOrderItemMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы purchase_order_item.
class PurchaseOrderItemHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit PurchaseOrderItemHttpController(fasc::server::controllers::app::PurchaseOrderItemController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  PurchaseOrderItemRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  PurchaseOrderItemRowViewResult load(const fasc::server::controllers::dto::PurchaseOrderItemKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  PurchaseOrderItemMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  PurchaseOrderItemMutationViewResult update(const fasc::server::controllers::dto::PurchaseOrderItemKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  PurchaseOrderItemMutationViewResult erase(const fasc::server::controllers::dto::PurchaseOrderItemKeyDto& key) const;

private:
  fasc::server::controllers::app::PurchaseOrderItemController& controller_;
};

} // namespace fasc::server::controllers::http
