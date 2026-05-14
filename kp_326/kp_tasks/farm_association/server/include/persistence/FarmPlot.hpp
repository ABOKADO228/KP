#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы farm_plot.
struct FarmPlotEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки farm_plot_type_id.
  odb::nullable<int> farmPlotTypeId;

  /// Значение колонки name.
  odb::nullable<std::string> name;

  /// Значение колонки area.
  odb::nullable<double> area;

  /// Значение колонки location.
  odb::nullable<std::string> location;

  /// Значение колонки cadastral_number.
  odb::nullable<std::string> cadastralNumber;

  /// Значение колонки status.
  odb::nullable<std::string> status;

};

#pragma db object(FarmPlotEntity) table("farm_plot")
#pragma db member(FarmPlotEntity::id) id auto column("id")
#pragma db member(FarmPlotEntity::farmPlotTypeId) column("farm_plot_type_id")
#pragma db member(FarmPlotEntity::name) column("name")
#pragma db member(FarmPlotEntity::area) column("area")
#pragma db member(FarmPlotEntity::location) column("location")
#pragma db member(FarmPlotEntity::cadastralNumber) column("cadastral_number")
#pragma db member(FarmPlotEntity::status) column("status")

} // namespace fasc::server::persistence
