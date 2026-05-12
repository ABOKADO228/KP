#pragma once

#include <controllers/dto/User.hpp>
#include <database/database.hpp>

///Application controller для пользовательских сценариев.
///@note Работает с @c Database и не знает о HTTP/JSON деталях.
class UserController {
public:
  ///Создает controller поверх database layer.
  ///@param db база данных, через которую выполняются persistence-операции.
  explicit UserController(fasc::server::database::Database& db);

  ///Создает пользователя.
  ///@param command команда с данными нового пользователя.
  ///@returns DTO созданного пользователя.
  ///@throws Исключения @c Database или ODB, если persistence-операция не выполнена.
  UserDto create_user(CreateUserCommand command);

private:
  fasc::server::database::Database& db_;
};
