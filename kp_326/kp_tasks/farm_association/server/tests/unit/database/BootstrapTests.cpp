#include <database/Bootstrap.hpp>

#include <gtest/gtest.h>

namespace {

using fasc::server::database::DatabaseBootstrapMode;
using fasc::server::database::DatabaseBootstrapOptions;
using fasc::server::database::ConnectionSettings;
using fasc::server::database::compiledRuntimeDefaults;
using fasc::server::database::defaultsForBuildConfig;
using fasc::server::database::prepareDatabase;

TEST(DatabaseBootstrapTests, DebugBuildDefaultsUseResettableTestDatabase) {
  const auto defaults = defaultsForBuildConfig("Debug");

  EXPECT_EQ(defaults.databaseName, "fasc_test");
  EXPECT_EQ(defaults.mode, DatabaseBootstrapMode::ResetAndInitialize);
}

TEST(DatabaseBootstrapTests, NonDebugBuildDefaultsUsePersistentProductionDatabase) {
  const auto release = defaultsForBuildConfig("Release");
  const auto relWithDebInfo = defaultsForBuildConfig("RelWithDebInfo");

  EXPECT_EQ(release.databaseName, "fasc_db");
  EXPECT_EQ(release.mode, DatabaseBootstrapMode::EnsureInitialized);
  EXPECT_EQ(relWithDebInfo.databaseName, "fasc_db");
  EXPECT_EQ(relWithDebInfo.mode, DatabaseBootstrapMode::EnsureInitialized);
}

TEST(DatabaseBootstrapTests, CompiledDefaultsMatchThisBuildConfiguration) {
  const auto expected = defaultsForBuildConfig(FARM_SERVER_BUILD_CONFIG);
  const auto actual = compiledRuntimeDefaults();

  EXPECT_EQ(actual.databaseName, expected.databaseName);
  EXPECT_EQ(actual.mode, expected.mode);
}

TEST(DatabaseBootstrapTests, DisabledBootstrapDoesNotConnectToDatabase) {
  DatabaseBootstrapOptions options;
  options.enabled = false;
  options.connection = ConnectionSettings{"bad_user", "bad_password", "bad_db", "bad_host", 1};

  EXPECT_NO_THROW(prepareDatabase(options));
}

} // namespace
