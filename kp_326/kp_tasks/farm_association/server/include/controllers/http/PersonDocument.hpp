#pragma once

#include <controllers/app/PersonDocument.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/PersonDocument.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using PersonDocumentRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::PersonDocumentRowsView, fasc::server::views::ErrorView>;
using PersonDocumentRowViewResult = fasc::extended::fpp::Result<fasc::server::views::PersonDocumentRowView, fasc::server::views::ErrorView>;
using PersonDocumentMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::PersonDocumentMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы person_document.
class PersonDocumentHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit PersonDocumentHttpController(fasc::server::controllers::app::PersonDocumentController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  PersonDocumentRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  PersonDocumentRowViewResult load(const fasc::server::controllers::dto::PersonDocumentKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  PersonDocumentMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  PersonDocumentMutationViewResult update(const fasc::server::controllers::dto::PersonDocumentKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  PersonDocumentMutationViewResult erase(const fasc::server::controllers::dto::PersonDocumentKeyDto& key) const;

private:
  fasc::server::controllers::app::PersonDocumentController& controller_;
};

} // namespace fasc::server::controllers::http
