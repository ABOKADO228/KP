#pragma once

#include <functional>
#include <memory>
#include <odb/database.hxx>
#include <odb/exceptions.hxx>
#include <odb/result.hxx>
#include <odb/transaction.hxx>
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace fasc::server::database {

struct ConnectionSettings {
  std::string user;
  std::string password;
  std::string databaseName;
  std::string host;
  unsigned int port{};
};

///SQL-параметр для низкоуровневой операции database layer.
struct SqlParameter {
  ///Текстовое значение параметра.
  std::string text;

  ///Признак SQL NULL.
  bool isNull{};
};

///Строка результата SQL-запроса.
using SqlRow = std::unordered_map<std::string, std::optional<std::string>>;

///RAII-обертка над @c odb::transaction.
///@note Если @c commit() не вызван, ODB откатит активную транзакцию при уничтожении объекта.
class Transaction {
public:
  ///Открывает новую ODB-транзакцию.
  ///@param database база данных, в которой нужно открыть транзакцию.
  ///@throws Исключения ODB, если транзакцию нельзя открыть.
  explicit Transaction(odb::database& database);

  Transaction(const Transaction&) = delete;
  Transaction& operator=(const Transaction&) = delete;

  Transaction(Transaction&&) noexcept = default;
  Transaction& operator=(Transaction&&) noexcept = default;

  ///Уничтожает транзакцию.
  ///@note Для незакоммиченной владеющей транзакции сработает rollback через деструктор @c odb::transaction.
  ~Transaction() noexcept;

  ///Фиксирует владеющую транзакцию.
  ///@note Для вложенного no-op объекта, созданного через @c Database::makeTransaction(), ничего не делает.
  ///@throws Исключения ODB, если commit не выполнен.
  void commit();

  ///Откатывает владеющую транзакцию.
  ///@note Для вложенного no-op объекта, созданного через @c Database::makeTransaction(), ничего не делает.
  ///@throws Исключения ODB, если rollback не выполнен.
  void rollback();

private:
  Transaction() = default;

  friend class Database;

  std::unique_ptr<odb::transaction> transaction_;
};

///Проектная обертка над @c odb::database.
///@note Application layer должен использовать @c Database вместо прямой работы с @c odb::database.
class Database {
public:
  ///Создает wrapper над уже созданным ODB database object.
  ///@param database владеющий указатель на ODB database object.
  explicit Database(std::unique_ptr<odb::database> database);

  ///Создает подключение к PostgreSQL из переменных окружения.
  ///@returns database wrapper с настроенным @c odb::pgsql::database.
  ///@throws Исключения ODB/PostgreSQL, если подключение не может быть создано.
  static Database createFromEnv();

  static Database create(ConnectionSettings settings);

  ///Создает транзакцию или присоединяется к активной транзакции этой же базы.
  ///@returns объект @c Transaction, владеющий новой транзакцией или no-op объект для вложенного вызова.
  ///@throws @c odb::already_in_transaction, если в текущем потоке уже активна транзакция другой базы.
  Transaction makeTransaction();

  ///Проверяет наличие активной ODB-транзакции в текущем потоке.
  ///@returns @c true, если ODB уже имеет активную транзакцию в текущем потоке.
  bool hasActiveTransaction() const;

  ///Выполняет callback внутри транзакции.
  ///@note Если транзакция уже открыта на этой же @c Database, новая транзакция не создается.
  ///@param work callable-объект с работой, которую нужно выполнить транзакционно.
  ///@returns результат @c work, если callback возвращает не @c void.
  ///@throws @c odb::already_in_transaction, если активная транзакция принадлежит другой базе.
  ///@throws Любое исключение, выброшенное @c work или ODB; при исключении commit не выполняется.
  template <typename Work>
  decltype(auto) invokeTransactionally(Work&& work) {
    if (hasActiveTransaction()) {
      if (&odb::transaction::current().database() != underlying.get()) {
        throw odb::already_in_transaction();
      }

      return std::invoke(std::forward<Work>(work));
    }

    auto transaction = makeTransaction();

    if constexpr (std::is_void_v<std::invoke_result_t<Work>>) {
      std::invoke(std::forward<Work>(work));
      transaction.commit();
    } else {
      decltype(auto) result = std::invoke(std::forward<Work>(work));
      transaction.commit();
      return result;
    }
  }

  ///Сохраняет новую persistence-сущность.
  ///@note Использовать внутри активной транзакции или через @c invokeTransactionally().
  ///@param entity сущность, которую нужно сохранить.
  ///@returns идентификатор, возвращенный ODB для сохраненной сущности.
  ///@throws Исключения ODB, если сохранение не выполнено.
  template <typename Entity>
  auto persist(Entity& entity) {
    return underlying->persist(entity);
  }

  ///Обновляет существующую persistence-сущность.
  ///@note Использовать внутри активной транзакции или через @c invokeTransactionally().
  ///@param entity сущность, которую нужно обновить.
  ///@throws Исключения ODB, если обновление не выполнено.
  template <typename Entity>
  void update(Entity& entity) {
    underlying->update(entity);
  }

  ///Удаляет существующую persistence-сущность.
  ///@note Использовать внутри активной транзакции или через @c invokeTransactionally().
  ///@param entity сущность, которую нужно удалить.
  ///@throws Исключения ODB, если удаление не выполнено.
  template <typename Entity>
  void erase(Entity& entity) {
    underlying->erase(entity);
  }

  ///Удаляет persistence-сущность по идентификатору.
  ///@note Использовать внутри активной транзакции или через @c invokeTransactionally().
  ///@param id идентификатор удаляемой сущности.
  ///@returns @c true, если сущность была удалена.
  ///@throws Исключения ODB, если удаление не выполнено.
  template <typename Entity, typename Id>
  bool eraseById(const Id& id) {
    return underlying->erase<Entity>(id);
  }

  ///Runs an ODB query through the wrapper and materializes the result.
  ///@note Use inside an active transaction or via @c invokeTransactionally().
  template <typename Entity, typename Query>
  std::vector<Entity> query(Query&& query) {
    odb::result<Entity> result = underlying->query<Entity>(std::forward<Query>(query));
    std::vector<Entity> entities;
    for (auto iterator = result.begin(); iterator != result.end(); ++iterator) {
      entities.push_back(*iterator);
    }
    return entities;
  }

  ///Возвращает доступ к сырому ODB database object.
  ///@note Использовать только для низкоуровневых сценариев, которые не покрыты методами @c Database.
  ///@returns ссылка на @c odb::database.
  odb::database& raw();

  ///Возвращает read-only доступ к сырому ODB database object.
  ///@note Использовать только для низкоуровневых сценариев, которые не покрыты методами @c Database.
  ///@returns const-ссылка на @c odb::database.
  const odb::database& raw() const;

  // Table-level CRUD helpers for application controllers. They keep SQL assembly
  // in the database wrapper while still using positional PostgreSQL parameters.
  std::vector<SqlRow> selectRows(const std::string& table,
                                 const std::vector<std::string>& columns);

  std::optional<SqlRow> selectOneRow(const std::string& table,
                                     const std::vector<std::string>& columns,
                                     const std::vector<std::string>& keyColumns,
                                     const std::vector<SqlParameter>& keyValues);

  unsigned long long insertRow(const std::string& table,
                               const std::vector<std::string>& columns,
                               const std::vector<SqlParameter>& values);

  unsigned long long updateRows(const std::string& table,
                                const std::vector<std::string>& columns,
                                const std::vector<SqlParameter>& values,
                                const std::vector<std::string>& keyColumns,
                                const std::vector<SqlParameter>& keyValues);

  unsigned long long deleteRows(const std::string& table,
                                const std::vector<std::string>& keyColumns,
                                const std::vector<SqlParameter>& keyValues);

  ///Выполняет SELECT через ODB PostgreSQL connection.
  ///@param sql    SQL-запрос с позиционными параметрами PostgreSQL.
  ///@param values значения параметров.
  ///@returns строки результата.
  std::vector<SqlRow> querySql(const std::string& sql,
                               const std::vector<SqlParameter>& values);

  ///Выполняет команду изменения данных через ODB PostgreSQL connection.
  ///@param sql    SQL-команда с позиционными параметрами PostgreSQL.
  ///@param values значения параметров.
  ///@returns количество затронутых строк.
  unsigned long long executeSql(const std::string& sql,
                                const std::vector<SqlParameter>& values);

private:
  std::unique_ptr<odb::database> underlying;
};

} // namespace fasc::server::database
