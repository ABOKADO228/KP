#pragma once

#include <controllers/app/Person.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/Person.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using PersonRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::PersonRowsView, fasc::server::views::ErrorView>;
using PersonRowViewResult = fasc::extended::fpp::Result<fasc::server::views::PersonRowView, fasc::server::views::ErrorView>;
using PersonMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::PersonMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы person.
class PersonHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit PersonHttpController(fasc::server::controllers::app::PersonController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  PersonRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  PersonRowViewResult load(const fasc::server::controllers::dto::PersonKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  PersonMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  PersonMutationViewResult update(const fasc::server::controllers::dto::PersonKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  PersonMutationViewResult erase(const fasc::server::controllers::dto::PersonKeyDto& key) const;

private:
  fasc::server::controllers::app::PersonController& controller_;
};

} // namespace fasc::server::controllers::http
