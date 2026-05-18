#pragma once

#include <database/Database.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::database {

std::vector<SqlRow> selectOdbRows(Database& db,
                                  const std::string& table,
                                  const std::vector<std::string>& columns);

std::optional<SqlRow> selectOneOdbRow(Database& db,
                                      const std::string& table,
                                      const std::vector<std::string>& columns,
                                      const std::vector<std::string>& keyColumns,
                                      const std::vector<SqlParameter>& keyValues);

unsigned long long insertOdbRow(Database& db,
                                const std::string& table,
                                const std::vector<std::string>& columns,
                                const std::vector<SqlParameter>& values);

unsigned long long updateOdbRows(Database& db,
                                 const std::string& table,
                                 const std::vector<std::string>& columns,
                                 const std::vector<SqlParameter>& values,
                                 const std::vector<std::string>& keyColumns,
                                 const std::vector<SqlParameter>& keyValues);

unsigned long long deleteOdbRows(Database& db,
                                 const std::string& table,
                                 const std::vector<std::string>& keyColumns,
                                 const std::vector<SqlParameter>& keyValues);

} // namespace fasc::server::database
