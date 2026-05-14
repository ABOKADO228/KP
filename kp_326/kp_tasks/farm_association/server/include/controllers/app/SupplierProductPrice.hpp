#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/SupplierProductPrice.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using SupplierProductPriceRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::SupplierProductPriceRowsDto, FarmEntityError>;
using SupplierProductPriceRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::SupplierProductPriceRowDto, FarmEntityError>;
using SupplierProductPriceMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::SupplierProductPriceMutationDto, FarmEntityError>;

/// Application controller таблицы supplier_product_price.
class SupplierProductPriceController {
public:
  /// Создает controller таблицы supplier_product_price.
  ///@param db база данных приложения.
  explicit SupplierProductPriceController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  SupplierProductPriceRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  SupplierProductPriceRowResult load(const fasc::server::controllers::dto::SupplierProductPriceKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  SupplierProductPriceMutationResult create(const fasc::server::controllers::dto::SupplierProductPriceCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  SupplierProductPriceMutationResult update(const fasc::server::controllers::dto::SupplierProductPriceKeyDto& key,
                             const fasc::server::controllers::dto::SupplierProductPriceUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  SupplierProductPriceMutationResult erase(const fasc::server::controllers::dto::SupplierProductPriceKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
