#pragma once

#include <controllers/app/FarmOwner.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/FarmOwner.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using FarmOwnerRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmOwnerRowsView, fasc::server::views::ErrorView>;
using FarmOwnerRowViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmOwnerRowView, fasc::server::views::ErrorView>;
using FarmOwnerMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmOwnerMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы farm_owner.
class FarmOwnerHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit FarmOwnerHttpController(fasc::server::controllers::app::FarmOwnerController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  FarmOwnerRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  FarmOwnerRowViewResult load(const fasc::server::controllers::dto::FarmOwnerKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmOwnerMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmOwnerMutationViewResult update(const fasc::server::controllers::dto::FarmOwnerKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  FarmOwnerMutationViewResult erase(const fasc::server::controllers::dto::FarmOwnerKeyDto& key) const;

private:
  fasc::server::controllers::app::FarmOwnerController& controller_;
};

} // namespace fasc::server::controllers::http
