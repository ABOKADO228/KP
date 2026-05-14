#pragma once

#include <database/Database.hpp>
#include <server/core/Server.hpp>

namespace fasc::server::handling {

/// Регистрирует маршруты таблиц фермерской ассоциации.
///@param server HTTP-сервер.
///@param database база данных приложения.
void registerFarmEntityRoutes(fasc::server::core::Server& server,
                              fasc::server::database::Database& database);

} // namespace fasc::server::handling
