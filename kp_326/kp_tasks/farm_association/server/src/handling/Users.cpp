#include <handling/Users.hpp>

UserHandler::UserHandler(UserHttpController& users) : users_(users) {}

HttpResponse UserHandler::create_user(const HttpRequest& request) {
  return users_.create_user(request);
}

HttpResponse UserHandler::register_user(const HttpRequest& request) {
  return users_.register_user(request);
}

HttpResponse UserHandler::login_user(const HttpRequest& request) {
  return users_.login_user(request);
}
