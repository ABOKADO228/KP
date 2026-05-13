#pragma once

#include <libpq-fe.h>

#include <memory>
#include <string>

namespace fasc::server::database {

struct PgResultDeleter {
  void operator()(PGresult* result) const;
};

using PgResultPtr = std::unique_ptr<PGresult, PgResultDeleter>;

class PgConnection {
public:
  PgConnection();

  PgConnection(const PgConnection&) = delete;
  PgConnection& operator=(const PgConnection&) = delete;

  PgConnection(PgConnection&&) noexcept = default;
  PgConnection& operator=(PgConnection&&) noexcept = default;

  PGconn* raw();
  PgResultPtr execParams(const std::string& sql, const char* const* values, int value_count);

private:
  struct ConnectionDeleter {
    void operator()(PGconn* connection) const;
  };

  std::unique_ptr<PGconn, ConnectionDeleter> connection_;
};

} // namespace fasc::server::database
