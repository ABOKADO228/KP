#pragma once

#include <controllers/app/Supplier.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/Supplier.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using SupplierRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::SupplierRowsView, fasc::server::views::ErrorView>;
using SupplierRowViewResult = fasc::extended::fpp::Result<fasc::server::views::SupplierRowView, fasc::server::views::ErrorView>;
using SupplierMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::SupplierMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы supplier.
class SupplierHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit SupplierHttpController(fasc::server::controllers::app::SupplierController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  SupplierRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  SupplierRowViewResult load(const fasc::server::controllers::dto::SupplierKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  SupplierMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  SupplierMutationViewResult update(const fasc::server::controllers::dto::SupplierKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  SupplierMutationViewResult erase(const fasc::server::controllers::dto::SupplierKeyDto& key) const;

private:
  fasc::server::controllers::app::SupplierController& controller_;
};

} // namespace fasc::server::controllers::http
