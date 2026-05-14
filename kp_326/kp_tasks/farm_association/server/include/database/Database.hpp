#pragma once

#include <functional>
#include <memory>
#include <odb/database.hxx>
#include <odb/exceptions.hxx>
#include <odb/transaction.hxx>
#include <type_traits>
#include <utility>

namespace fasc::server::database {
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

  ///Возвращает доступ к сырому ODB database object.
  ///@note Использовать только для низкоуровневых сценариев, которые не покрыты методами @c Database.
  ///@returns ссылка на @c odb::database.
  odb::database& raw();

  ///Возвращает read-only доступ к сырому ODB database object.
  ///@note Использовать только для низкоуровневых сценариев, которые не покрыты методами @c Database.
  ///@returns const-ссылка на @c odb::database.
  const odb::database& raw() const;

private:
  std::unique_ptr<odb::database> underlying;
};

} // namespace fasc::server::database
