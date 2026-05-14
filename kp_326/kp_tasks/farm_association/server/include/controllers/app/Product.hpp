#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/Product.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using ProductRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::ProductRowsDto, FarmEntityError>;
using ProductRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::ProductRowDto, FarmEntityError>;
using ProductMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::ProductMutationDto, FarmEntityError>;

/// Application controller таблицы product.
class ProductController {
public:
  /// Создает controller таблицы product.
  ///@param db база данных приложения.
  explicit ProductController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  ProductRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  ProductRowResult load(const fasc::server::controllers::dto::ProductKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  ProductMutationResult create(const fasc::server::controllers::dto::ProductCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  ProductMutationResult update(const fasc::server::controllers::dto::ProductKeyDto& key,
                             const fasc::server::controllers::dto::ProductUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  ProductMutationResult erase(const fasc::server::controllers::dto::ProductKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
