#pragma once

#include <odb/core.hxx>

#include <string>
#include <utility>

namespace fasc {
namespace server {
namespace persistence {

/// Persistence entity for an application user.
class User {
public:
  /// Creates an empty entity for ODB materialization.
  User() = default;

  /// Creates a new user with a prepared password hash and an access role.
  User(std::string login, std::string passwordHash, std::string role)
      : login_(std::move(login)),
        passwordHash_(std::move(passwordHash)),
        role_(std::move(role)) {}

  /// Returns the user login. The login is the primary key.
  const std::string& login() const {
    return login_;
  }

  /// Returns the password hash.
  const std::string& passwordHash() const {
    return passwordHash_;
  }

  /// Returns the server-owned access role.
  const std::string& role() const {
    return role_;
  }

  /// Replaces the server-owned access role.
  void role(std::string role) {
    role_ = std::move(role);
  }

private:
  friend class odb::access;

  /// Primary key and public account login.
  std::string login_;

  /// User password hash.
  std::string passwordHash_;

  /// Role code used by the backend and returned to clients.
  std::string role_;
};

#pragma db object(User)
#pragma db member(User::login_) id column("login")
#pragma db member(User::passwordHash_) column("password_hash")
#pragma db member(User::role_) column("role")

} // namespace persistence
} // namespace server
} // namespace fasc
