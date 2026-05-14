#pragma once

#include <controllers/app/Farm.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/Farm.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using FarmRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmRowsView, fasc::server::views::ErrorView>;
using FarmRowViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmRowView, fasc::server::views::ErrorView>;
using FarmMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы farm.
class FarmHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit FarmHttpController(fasc::server::controllers::app::FarmController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  FarmRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  FarmRowViewResult load(const fasc::server::controllers::dto::FarmKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmMutationViewResult update(const fasc::server::controllers::dto::FarmKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  FarmMutationViewResult erase(const fasc::server::controllers::dto::FarmKeyDto& key) const;

private:
  fasc::server::controllers::app::FarmController& controller_;
};

} // namespace fasc::server::controllers::http
