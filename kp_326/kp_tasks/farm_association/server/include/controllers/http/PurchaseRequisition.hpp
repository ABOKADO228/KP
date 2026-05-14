#pragma once

#include <controllers/app/PurchaseRequisition.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/PurchaseRequisition.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using PurchaseRequisitionRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::PurchaseRequisitionRowsView, fasc::server::views::ErrorView>;
using PurchaseRequisitionRowViewResult = fasc::extended::fpp::Result<fasc::server::views::PurchaseRequisitionRowView, fasc::server::views::ErrorView>;
using PurchaseRequisitionMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::PurchaseRequisitionMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы purchase_requisition.
class PurchaseRequisitionHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit PurchaseRequisitionHttpController(fasc::server::controllers::app::PurchaseRequisitionController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  PurchaseRequisitionRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  PurchaseRequisitionRowViewResult load(const fasc::server::controllers::dto::PurchaseRequisitionKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  PurchaseRequisitionMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  PurchaseRequisitionMutationViewResult update(const fasc::server::controllers::dto::PurchaseRequisitionKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  PurchaseRequisitionMutationViewResult erase(const fasc::server::controllers::dto::PurchaseRequisitionKeyDto& key) const;

private:
  fasc::server::controllers::app::PurchaseRequisitionController& controller_;
};

} // namespace fasc::server::controllers::http
