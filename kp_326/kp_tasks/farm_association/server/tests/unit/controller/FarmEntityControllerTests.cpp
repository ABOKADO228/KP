#include <controllers/app/Farm.hpp>

#include <gtest/gtest.h>

namespace {

using fasc::server::controllers::app::FarmController;
using fasc::server::controllers::app::FarmEntityErrorCode;
using fasc::server::controllers::dto::FarmCreateDto;

TEST(FarmEntityControllerTests, FarmRejectsEmptyCreateBeforeDatabaseAccess) {
  fasc::server::database::Database database{nullptr};
  FarmController controller{database};

  const auto result = controller.create(FarmCreateDto{});

  ASSERT_TRUE(result.hasError());
  EXPECT_EQ(result.error().code, FarmEntityErrorCode::InvalidInput);
  EXPECT_EQ(result.error().message, "No writable columns provided");
}

} // namespace
