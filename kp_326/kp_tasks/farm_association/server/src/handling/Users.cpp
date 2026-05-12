#include <handling/Users.hpp>

UserHandler::UserHandler(UserHttpController& users) : users_(users) {}

HttpResponse UserHandler::create_user(const HttpRequest& request) {
  return users_.create_user(request);
}
