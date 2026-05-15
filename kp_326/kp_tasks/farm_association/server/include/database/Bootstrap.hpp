#pragma once

#include <database/Database.hpp>

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace fasc::server::database {

enum class DatabaseBootstrapMode {
  EnsureInitialized,
  ResetAndInitialize,
};

enum class DatabaseBootstrapScriptKind {
  DomainSchema,
  UserSchema,
};

struct DatabaseBootstrapScript {
  DatabaseBootstrapScriptKind kind{DatabaseBootstrapScriptKind::DomainSchema};
  std::filesystem::path path;
};

struct DatabaseRuntimeDefaults {
  std::string databaseName;
  DatabaseBootstrapMode mode{DatabaseBootstrapMode::EnsureInitialized};
};

struct DatabaseBootstrapOptions {
  bool enabled{true};
  ConnectionSettings connection;
  std::string maintenanceDatabase{"postgres"};
  DatabaseBootstrapMode mode{DatabaseBootstrapMode::EnsureInitialized};
  std::vector<DatabaseBootstrapScript> initializationScripts;
};

DatabaseRuntimeDefaults defaultsForBuildConfig(std::string_view buildConfig);

DatabaseRuntimeDefaults compiledRuntimeDefaults();

ConnectionSettings connectionSettingsFromEnv(std::string defaultDatabaseName);

DatabaseBootstrapOptions bootstrapOptionsFromEnv();

void prepareDatabase(const DatabaseBootstrapOptions& options);

} // namespace fasc::server::database
