#pragma once

#include <controllers/app/UserError.hpp>
#include <controllers/dto/User.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>
#include <security/JwtService.hpp>
#include <security/PasswordHasher.hpp>

namespace fasc::server::controllers::app {

using fasc::server::controllers::dto::AuthResultDto;
using fasc::server::controllers::dto::CreateUserCommand;
using fasc::server::controllers::dto::LoginUserCommand;
using fasc::server::controllers::dto::RegisterUserCommand;
using fasc::server::controllers::dto::UpdateUserRoleCommand;
using fasc::server::controllers::dto::UserDto;
using fasc::server::controllers::dto::UserListDto;
using fasc::extended::fpp::Result;

///Результат создания пользователя.
using CreateUserResult = Result<UserDto, UserError>;

///Результат получения списка пользователей.
using UserListResult = Result<UserListDto, UserError>;

///Результат изменения роли пользователя.
using UpdateUserRoleResult = Result<UserDto, UserError>;

///Результат регистрации или авторизации пользователя.
using AuthResult = Result<AuthResultDto, UserError>;

/// Application controller для пользовательских сценариев.
///@note Работает с @c Database и не знает о HTTP/JSON деталях.
class UserController {
public:
  /// Создает controller поверх database layer.
  ///@param db база данных, через которую выполняются persistence-операции.
  ///@param passwordHasher сервис хеширования и проверки паролей.
  ///@param jwt_service сервис выпуска JWT.
  UserController(fasc::server::database::Database& db,
                 fasc::server::security::PasswordHasher& passwordHasher,
                 fasc::server::security::JwtService& jwt_service);

  /// Создает пользователя.
  ///@param command команда с данными нового пользователя.
  ///@returns результат с DTO созданного пользователя или предметной ошибкой.
  CreateUserResult createUser(CreateUserCommand command);

  ///Возвращает всех пользователей.
  ///@returns результат со списком пользователей или предметной ошибкой.
  UserListResult listUsers();

  ///Изменяет роль существующего пользователя.
  ///@param command команда с логином пользователя и новой ролью.
  ///@returns результат с обновленным DTO пользователя или предметной ошибкой.
  UpdateUserRoleResult updateUserRole(UpdateUserRoleCommand command);

  ///Регистрирует пользователя и выпускает JWT.
  ///@param command команда регистрации.
  ///@returns результат с DTO пользователя и токеном или предметной ошибкой.
  AuthResult registerUser(RegisterUserCommand command);

  ///Авторизует пользователя и выпускает JWT.
  ///@param command команда авторизации.
  ///@returns результат с DTO пользователя и токеном или предметной ошибкой.
  AuthResult loginUser(LoginUserCommand command);

private:
  ///Создает пользователя после общей проверки логина, пароля и роли.
  ///@param login логин пользователя.
  ///@param password пароль в открытом виде.
  ///@param role роль пользователя.
  ///@returns результат с DTO созданного пользователя или предметной ошибкой.
  CreateUserResult createUserWithPassword(std::string login,
                                          std::string password,
                                          std::string role);

  /// База данных для persistence-операций.
  fasc::server::database::Database& db_;

  /// Сервис хеширования паролей.
  fasc::server::security::PasswordHasher& passwordHasher_;

  /// JWT сервис авторизации.
  fasc::server::security::JwtService& jwt_service_;
};

} // namespace fasc::server::controllers::app
