#pragma once

#include <controllers/app/Unit.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/Unit.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using UnitRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::UnitRowsView, fasc::server::views::ErrorView>;
using UnitRowViewResult = fasc::extended::fpp::Result<fasc::server::views::UnitRowView, fasc::server::views::ErrorView>;
using UnitMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::UnitMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы unit.
class UnitHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit UnitHttpController(fasc::server::controllers::app::UnitController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  UnitRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  UnitRowViewResult load(const fasc::server::controllers::dto::UnitKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  UnitMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  UnitMutationViewResult update(const fasc::server::controllers::dto::UnitKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  UnitMutationViewResult erase(const fasc::server::controllers::dto::UnitKeyDto& key) const;

private:
  fasc::server::controllers::app::UnitController& controller_;
};

} // namespace fasc::server::controllers::http
