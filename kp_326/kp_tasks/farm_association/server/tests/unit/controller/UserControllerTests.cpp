#include <controllers/app/Users.hpp>
#include <database/database.hpp>

#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>

namespace {

TEST(UserControllerTests, RejectsEmptyUserNameBeforePersistence) {
  fasc::server::database::Database database{nullptr};
  UserController controller{database};

  EXPECT_THROW(controller.create_user(CreateUserCommand{""}), std::invalid_argument);
}

} // namespace
