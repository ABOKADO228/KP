#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы farm_plot_type.
struct FarmPlotTypeEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки name.
  odb::nullable<std::string> name;

  /// Значение колонки description.
  odb::nullable<std::string> description;

  /// Значение колонки farm_plot_level.
  int farmPlotLevel{};

  /// Значение колонки parent_id.
  odb::nullable<std::uint64_t> parentId;

};

#pragma db object(FarmPlotTypeEntity) table("farm_plot_type")
#pragma db member(FarmPlotTypeEntity::id) id column("id")
#pragma db member(FarmPlotTypeEntity::name) column("name")
#pragma db member(FarmPlotTypeEntity::description) column("description")
#pragma db member(FarmPlotTypeEntity::farmPlotLevel) column("farm_plot_level")
#pragma db member(FarmPlotTypeEntity::parentId) column("parent_id")

} // namespace fasc::server::persistence
