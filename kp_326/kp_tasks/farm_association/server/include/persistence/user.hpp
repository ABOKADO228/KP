#pragma once

#include <odb/core.hxx>

#include <string>
#include <utility>

///Persistence-модель пользователя.
///@note Используется ODB и должна изменяться с учетом внешних @c db pragmas под классом.
class User {
public:
  ///Создает пустой объект для ODB materialization.
  ///@note Конструктор нужен ODB для загрузки объекта из базы данных.
  User() = default;

  ///Создает нового пользователя с именем.
  ///@param name имя пользователя.
  explicit User(std::string name) : name_(std::move(name)) {}

  ///Возвращает идентификатор пользователя.
  ///@returns database id пользователя.
  unsigned long id() const {
    return id_;
  }

  ///Возвращает имя пользователя.
  ///@returns имя пользователя.
  const std::string& name() const {
    return name_;
  }

private:
  friend class odb::access;

  unsigned long id_{};

  std::string name_;
};

#pragma db object(User)
#pragma db member(User::id_) id auto
