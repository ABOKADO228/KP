#pragma once

#include <controllers/app/IdentityDocumentType.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/IdentityDocumentType.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using IdentityDocumentTypeRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::IdentityDocumentTypeRowsView, fasc::server::views::ErrorView>;
using IdentityDocumentTypeRowViewResult = fasc::extended::fpp::Result<fasc::server::views::IdentityDocumentTypeRowView, fasc::server::views::ErrorView>;
using IdentityDocumentTypeMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::IdentityDocumentTypeMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы identity_document_type.
class IdentityDocumentTypeHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit IdentityDocumentTypeHttpController(fasc::server::controllers::app::IdentityDocumentTypeController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  IdentityDocumentTypeRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  IdentityDocumentTypeRowViewResult load(const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  IdentityDocumentTypeMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  IdentityDocumentTypeMutationViewResult update(const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  IdentityDocumentTypeMutationViewResult erase(const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key) const;

private:
  fasc::server::controllers::app::IdentityDocumentTypeController& controller_;
};

} // namespace fasc::server::controllers::http
