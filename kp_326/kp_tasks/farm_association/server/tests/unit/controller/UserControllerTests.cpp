#include <controllers/app/Users.hpp>
#include <database/Database.hpp>
#include <security/JwtService.hpp>
#include <security/PasswordHasher.hpp>

#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>

namespace {

using fasc::server::controllers::app::UserController;
using fasc::server::controllers::dto::CreateUserCommand;

TEST(UserControllerTests, RejectsEmptyLoginBeforePersistence) {
  fasc::server::database::Database database{nullptr};
  fasc::server::security::PasswordHasher passwordHasher;
  fasc::server::security::JwtService jwt_service{"test-secret"};
  UserController controller{database, passwordHasher, jwt_service};

  const auto result = controller.createUser(CreateUserCommand{"", "password123"});

  ASSERT_TRUE(result.hasError());
  EXPECT_EQ(result.error().code, fasc::server::controllers::app::UserErrorCode::InvalidInput);
  EXPECT_EQ(result.error().message, "User login is required");
}

TEST(UserControllerTests, RejectsShortPasswordBeforePersistence) {
  fasc::server::database::Database database{nullptr};
  fasc::server::security::PasswordHasher passwordHasher;
  fasc::server::security::JwtService jwt_service{"test-secret"};
  UserController controller{database, passwordHasher, jwt_service};

  const auto result = controller.createUser(CreateUserCommand{"alex", "short"});

  ASSERT_TRUE(result.hasError());
  EXPECT_EQ(result.error().code, fasc::server::controllers::app::UserErrorCode::InvalidInput);
  EXPECT_EQ(result.error().message, "Password must contain at least 8 characters");
}

} // namespace
