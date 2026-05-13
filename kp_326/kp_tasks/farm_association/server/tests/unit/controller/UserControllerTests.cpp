#include <controllers/app/Users.hpp>
#include <database/database.hpp>
#include <security/JwtService.hpp>
#include <security/PasswordHasher.hpp>

#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>

namespace {

TEST(UserControllerTests, RejectsEmptyUserNameBeforePersistence) {
  fasc::server::database::Database database{nullptr};
  fasc::server::security::PasswordHasher password_hasher;
  fasc::server::security::JwtService jwt_service{"test-secret"};
  UserController controller{database, password_hasher, jwt_service};

  EXPECT_THROW(controller.create_user(CreateUserCommand{"", "password123"}), std::invalid_argument);
}

TEST(UserControllerTests, RejectsShortPasswordBeforePersistence) {
  fasc::server::database::Database database{nullptr};
  fasc::server::security::PasswordHasher password_hasher;
  fasc::server::security::JwtService jwt_service{"test-secret"};
  UserController controller{database, password_hasher, jwt_service};

  EXPECT_THROW(controller.create_user(CreateUserCommand{"Alex", "short"}), std::invalid_argument);
}

} // namespace
