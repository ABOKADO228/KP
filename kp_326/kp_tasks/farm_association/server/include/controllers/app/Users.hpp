#pragma once

#include <controllers/dto/User.hpp>
#include <database/database.hpp>
#include <security/JwtService.hpp>
#include <security/PasswordHasher.hpp>

///Application controller для пользовательских сценариев.
///@note Работает с @c Database и не знает о HTTP/JSON деталях.
class UserController {
public:
  ///Создает controller поверх database layer.
  ///@param db база данных, через которую выполняются persistence-операции.
  UserController(fasc::server::database::Database& db,
                 fasc::server::security::PasswordHasher& password_hasher,
                 fasc::server::security::JwtService& jwt_service);

  ///Создает пользователя.
  ///@param command команда с данными нового пользователя.
  ///@returns DTO созданного пользователя.
  ///@throws Исключения @c Database или ODB, если persistence-операция не выполнена.
  UserDto create_user(CreateUserCommand command);
  AuthResultDto register_user(RegisterUserCommand command);
  AuthResultDto login_user(LoginUserCommand command);

private:
  UserDto create_user_with_password(std::string name, std::string password);

  fasc::server::database::Database& db_;
  fasc::server::security::PasswordHasher& password_hasher_;
  fasc::server::security::JwtService& jwt_service_;
};
