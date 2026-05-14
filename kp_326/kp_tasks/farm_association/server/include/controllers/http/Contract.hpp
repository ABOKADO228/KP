#pragma once

#include <controllers/app/Contract.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/Contract.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using ContractRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::ContractRowsView, fasc::server::views::ErrorView>;
using ContractRowViewResult = fasc::extended::fpp::Result<fasc::server::views::ContractRowView, fasc::server::views::ErrorView>;
using ContractMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::ContractMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы contract.
class ContractHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit ContractHttpController(fasc::server::controllers::app::ContractController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  ContractRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  ContractRowViewResult load(const fasc::server::controllers::dto::ContractKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  ContractMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  ContractMutationViewResult update(const fasc::server::controllers::dto::ContractKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  ContractMutationViewResult erase(const fasc::server::controllers::dto::ContractKeyDto& key) const;

private:
  fasc::server::controllers::app::ContractController& controller_;
};

} // namespace fasc::server::controllers::http
