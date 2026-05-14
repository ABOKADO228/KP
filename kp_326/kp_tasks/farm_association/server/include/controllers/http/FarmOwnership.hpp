#pragma once

#include <controllers/app/FarmOwnership.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/FarmOwnership.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using FarmOwnershipRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmOwnershipRowsView, fasc::server::views::ErrorView>;
using FarmOwnershipRowViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmOwnershipRowView, fasc::server::views::ErrorView>;
using FarmOwnershipMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmOwnershipMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы farm_ownership.
class FarmOwnershipHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit FarmOwnershipHttpController(fasc::server::controllers::app::FarmOwnershipController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  FarmOwnershipRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  FarmOwnershipRowViewResult load(const fasc::server::controllers::dto::FarmOwnershipKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmOwnershipMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmOwnershipMutationViewResult update(const fasc::server::controllers::dto::FarmOwnershipKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  FarmOwnershipMutationViewResult erase(const fasc::server::controllers::dto::FarmOwnershipKeyDto& key) const;

private:
  fasc::server::controllers::app::FarmOwnershipController& controller_;
};

} // namespace fasc::server::controllers::http
