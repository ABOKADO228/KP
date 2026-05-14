#pragma once

#include <controllers/app/ProductType.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/ProductType.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using ProductTypeRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::ProductTypeRowsView, fasc::server::views::ErrorView>;
using ProductTypeRowViewResult = fasc::extended::fpp::Result<fasc::server::views::ProductTypeRowView, fasc::server::views::ErrorView>;
using ProductTypeMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::ProductTypeMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы product_type.
class ProductTypeHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit ProductTypeHttpController(fasc::server::controllers::app::ProductTypeController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  ProductTypeRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  ProductTypeRowViewResult load(const fasc::server::controllers::dto::ProductTypeKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  ProductTypeMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  ProductTypeMutationViewResult update(const fasc::server::controllers::dto::ProductTypeKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  ProductTypeMutationViewResult erase(const fasc::server::controllers::dto::ProductTypeKeyDto& key) const;

private:
  fasc::server::controllers::app::ProductTypeController& controller_;
};

} // namespace fasc::server::controllers::http
