#pragma once

#include <controllers/app/Product.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/Product.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using ProductRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::ProductRowsView, fasc::server::views::ErrorView>;
using ProductRowViewResult = fasc::extended::fpp::Result<fasc::server::views::ProductRowView, fasc::server::views::ErrorView>;
using ProductMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::ProductMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы product.
class ProductHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit ProductHttpController(fasc::server::controllers::app::ProductController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  ProductRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  ProductRowViewResult load(const fasc::server::controllers::dto::ProductKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  ProductMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  ProductMutationViewResult update(const fasc::server::controllers::dto::ProductKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  ProductMutationViewResult erase(const fasc::server::controllers::dto::ProductKeyDto& key) const;

private:
  fasc::server::controllers::app::ProductController& controller_;
};

} // namespace fasc::server::controllers::http
