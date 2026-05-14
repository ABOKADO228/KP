#pragma once

#include <odb/core.hxx>

#include <string>
#include <utility>

namespace fasc {
namespace server {
namespace persistence {

///Persistence-сущность пользователя сервера.
///@note Используется ODB для хранения учетных данных приложения.
class User {
public:
  ///Создает пустую сущность для ODB materialization.
  User() = default;

  ///Создает нового пользователя с уже подготовленным хешем пароля.
  ///@param name          имя пользователя.
  ///@param passwordHash хеш пароля в формате @c PasswordHasher.
  User(std::string name, std::string passwordHash)
      : name_(std::move(name)), passwordHash_(std::move(passwordHash)) {}

  ///Возвращает идентификатор пользователя.
  ///@returns primary key пользователя.
  unsigned long id() const {
    return id_;
  }

  ///Возвращает имя пользователя.
  ///@returns строка имени пользователя.
  const std::string& name() const {
    return name_;
  }

  ///Возвращает хеш пароля.
  ///@returns строка хеша пароля.
  const std::string& passwordHash() const {
    return passwordHash_;
  }

private:
  friend class odb::access;

  ///Primary key пользователя.
  unsigned long id_{};

  ///Уникальное имя пользователя.
  std::string name_;

  ///Хеш пароля пользователя.
  std::string passwordHash_;
};

#pragma db object(User)
#pragma db member(User::id_) id auto
#pragma db member(User::name_) unique

} // namespace persistence
} // namespace server
} // namespace fasc
