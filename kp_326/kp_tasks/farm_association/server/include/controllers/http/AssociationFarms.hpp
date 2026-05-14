#pragma once

#include <controllers/app/AssociationFarms.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/AssociationFarms.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using AssociationFarmsRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::AssociationFarmsRowsView, fasc::server::views::ErrorView>;
using AssociationFarmsRowViewResult = fasc::extended::fpp::Result<fasc::server::views::AssociationFarmsRowView, fasc::server::views::ErrorView>;
using AssociationFarmsMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::AssociationFarmsMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы association_farms.
class AssociationFarmsHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit AssociationFarmsHttpController(fasc::server::controllers::app::AssociationFarmsController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  AssociationFarmsRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  AssociationFarmsRowViewResult load(const fasc::server::controllers::dto::AssociationFarmsKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  AssociationFarmsMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  AssociationFarmsMutationViewResult update(const fasc::server::controllers::dto::AssociationFarmsKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  AssociationFarmsMutationViewResult erase(const fasc::server::controllers::dto::AssociationFarmsKeyDto& key) const;

private:
  fasc::server::controllers::app::AssociationFarmsController& controller_;
};

} // namespace fasc::server::controllers::http
