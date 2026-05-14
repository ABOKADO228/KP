#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/ProductType.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using ProductTypeRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::ProductTypeRowsDto, FarmEntityError>;
using ProductTypeRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::ProductTypeRowDto, FarmEntityError>;
using ProductTypeMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::ProductTypeMutationDto, FarmEntityError>;

/// Application controller таблицы product_type.
class ProductTypeController {
public:
  /// Создает controller таблицы product_type.
  ///@param db база данных приложения.
  explicit ProductTypeController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  ProductTypeRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  ProductTypeRowResult load(const fasc::server::controllers::dto::ProductTypeKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  ProductTypeMutationResult create(const fasc::server::controllers::dto::ProductTypeCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  ProductTypeMutationResult update(const fasc::server::controllers::dto::ProductTypeKeyDto& key,
                             const fasc::server::controllers::dto::ProductTypeUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  ProductTypeMutationResult erase(const fasc::server::controllers::dto::ProductTypeKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
