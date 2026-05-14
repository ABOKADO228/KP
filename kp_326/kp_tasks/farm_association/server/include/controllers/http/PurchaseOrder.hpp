#pragma once

#include <controllers/app/PurchaseOrder.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/PurchaseOrder.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using PurchaseOrderRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::PurchaseOrderRowsView, fasc::server::views::ErrorView>;
using PurchaseOrderRowViewResult = fasc::extended::fpp::Result<fasc::server::views::PurchaseOrderRowView, fasc::server::views::ErrorView>;
using PurchaseOrderMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::PurchaseOrderMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы purchase_order.
class PurchaseOrderHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit PurchaseOrderHttpController(fasc::server::controllers::app::PurchaseOrderController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  PurchaseOrderRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  PurchaseOrderRowViewResult load(const fasc::server::controllers::dto::PurchaseOrderKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  PurchaseOrderMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  PurchaseOrderMutationViewResult update(const fasc::server::controllers::dto::PurchaseOrderKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  PurchaseOrderMutationViewResult erase(const fasc::server::controllers::dto::PurchaseOrderKeyDto& key) const;

private:
  fasc::server::controllers::app::PurchaseOrderController& controller_;
};

} // namespace fasc::server::controllers::http
