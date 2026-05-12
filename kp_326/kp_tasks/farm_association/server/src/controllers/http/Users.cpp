#include <controllers/http/Users.hpp>

#include <marshalling/User.hpp>

#include <nlohmann/json.hpp>

UserHttpController::UserHttpController(UserController& users) : users_(users) {}

HttpResponse UserHttpController::create_user(const HttpRequest& request) {
  auto command = nlohmann::json::parse(request.body).get<CreateUserCommand>();

  const UserDto user = users_.create_user(std::move(command));
  const UserView view{user.id, user.name};
  const nlohmann::json body = view;

  return HttpResponse{201, "application/json", body.dump()};
}
