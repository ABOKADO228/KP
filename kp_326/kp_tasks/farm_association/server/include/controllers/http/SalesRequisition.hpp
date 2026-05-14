#pragma once

#include <controllers/app/SalesRequisition.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/SalesRequisition.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using SalesRequisitionRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::SalesRequisitionRowsView, fasc::server::views::ErrorView>;
using SalesRequisitionRowViewResult = fasc::extended::fpp::Result<fasc::server::views::SalesRequisitionRowView, fasc::server::views::ErrorView>;
using SalesRequisitionMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::SalesRequisitionMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы sales_requisition.
class SalesRequisitionHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit SalesRequisitionHttpController(fasc::server::controllers::app::SalesRequisitionController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  SalesRequisitionRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  SalesRequisitionRowViewResult load(const fasc::server::controllers::dto::SalesRequisitionKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  SalesRequisitionMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  SalesRequisitionMutationViewResult update(const fasc::server::controllers::dto::SalesRequisitionKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  SalesRequisitionMutationViewResult erase(const fasc::server::controllers::dto::SalesRequisitionKeyDto& key) const;

private:
  fasc::server::controllers::app::SalesRequisitionController& controller_;
};

} // namespace fasc::server::controllers::http
