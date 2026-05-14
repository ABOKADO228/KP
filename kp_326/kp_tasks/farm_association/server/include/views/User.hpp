#pragma once

#include <string>

namespace fasc::server::views {

///Публичное представление пользователя для внешнего API.
///@note View не содержит HTTP-статус, служебные поля запроса или чувствительные persistence-данные.
struct UserView {
  ///Идентификатор пользователя.
  unsigned long id{};

  ///Публичное имя пользователя.
  std::string name;
};

} // namespace fasc::server::views
