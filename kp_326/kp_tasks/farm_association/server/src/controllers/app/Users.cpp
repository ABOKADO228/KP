#include <controllers/app/Users.hpp>

#include <persistence/user-odb.hxx>
#include <persistence/User.hpp>

#include <stdexcept>

UserController::UserController(fasc::server::database::Database& db) : db_(db) {}

UserDto UserController::create_user(CreateUserCommand command) {
  if (command.name.empty()) {
    throw std::invalid_argument("User name is required");
  }

  return db_.invokeTransactionally([&] {
    User user{std::move(command.name)};
    db_.persist(user);

    return UserDto{user.id(), user.name()};
  });
}
