#pragma once

#include <controllers/app/FarmAssociation.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/FarmAssociation.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using FarmAssociationRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmAssociationRowsView, fasc::server::views::ErrorView>;
using FarmAssociationRowViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmAssociationRowView, fasc::server::views::ErrorView>;
using FarmAssociationMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmAssociationMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы farm_association.
class FarmAssociationHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit FarmAssociationHttpController(fasc::server::controllers::app::FarmAssociationController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  FarmAssociationRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  FarmAssociationRowViewResult load(const fasc::server::controllers::dto::FarmAssociationKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmAssociationMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmAssociationMutationViewResult update(const fasc::server::controllers::dto::FarmAssociationKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  FarmAssociationMutationViewResult erase(const fasc::server::controllers::dto::FarmAssociationKeyDto& key) const;

private:
  fasc::server::controllers::app::FarmAssociationController& controller_;
};

} // namespace fasc::server::controllers::http
