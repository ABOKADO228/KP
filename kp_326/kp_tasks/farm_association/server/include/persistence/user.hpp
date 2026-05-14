#pragma once

#include <odb/core.hxx>

#include <string>
#include <utility>

namespace fasc::server::persistence {
class User {
public:
  User() = default;

  User(std::string name, std::string password_hash)
      : name_(std::move(name)), password_hash_(std::move(password_hash)) {}

  unsigned long id() const {
    return id_;
  }

  const std::string& name() const {
    return name_;
  }

  const std::string& password_hash() const {
    return password_hash_;
  }

private:
  friend class odb::access;

  unsigned long id_{};
  std::string name_;
  std::string password_hash_;
};

#pragma db object(User)
#pragma db member(User::id_) id auto
#pragma db member(User::name_) unique
} // namespace fasc::server::persistence
