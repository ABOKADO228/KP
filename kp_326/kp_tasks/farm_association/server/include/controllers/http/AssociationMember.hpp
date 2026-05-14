#pragma once

#include <controllers/app/AssociationMember.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/AssociationMember.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using AssociationMemberRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::AssociationMemberRowsView, fasc::server::views::ErrorView>;
using AssociationMemberRowViewResult = fasc::extended::fpp::Result<fasc::server::views::AssociationMemberRowView, fasc::server::views::ErrorView>;
using AssociationMemberMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::AssociationMemberMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы association_member.
class AssociationMemberHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit AssociationMemberHttpController(fasc::server::controllers::app::AssociationMemberController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  AssociationMemberRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  AssociationMemberRowViewResult load(const fasc::server::controllers::dto::AssociationMemberKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  AssociationMemberMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  AssociationMemberMutationViewResult update(const fasc::server::controllers::dto::AssociationMemberKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  AssociationMemberMutationViewResult erase(const fasc::server::controllers::dto::AssociationMemberKeyDto& key) const;

private:
  fasc::server::controllers::app::AssociationMemberController& controller_;
};

} // namespace fasc::server::controllers::http
