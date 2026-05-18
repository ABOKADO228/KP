#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#ifndef ODB_COMPILER
#include <persistence/DomainValueTraits.hpp>
#endif

#include <string>

namespace fasc {
namespace server {
namespace persistence {

/// Сущность таблицы farm_plot.
struct FarmPlotEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки farm_plot_type_id.
  odb::nullable<unsigned long long> farmPlotTypeId;

  /// Значение колонки name.
  odb::nullable<std::string> name;

  /// Значение колонки area.
  odb::nullable<double> area;

  /// Значение колонки location.
  odb::nullable<std::string> location;

  /// Значение колонки cadastral_number.
  odb::nullable<std::string> cadastralNumber;

  /// Значение колонки status.
  odb::nullable<fasc::server::domain::FarmPlotStatus> status;

};

#pragma db object(FarmPlotEntity) table("farm_plot")
#pragma db member(FarmPlotEntity::id) id auto column("id")
#pragma db member(FarmPlotEntity::farmPlotTypeId) column("farm_plot_type_id")
#pragma db member(FarmPlotEntity::name) column("name")
#pragma db member(FarmPlotEntity::area) column("area")
#pragma db member(FarmPlotEntity::location) column("location")
#pragma db member(FarmPlotEntity::cadastralNumber) column("cadastral_number")
#pragma db member(FarmPlotEntity::status) column("status")

} // namespace persistence
} // namespace server
} // namespace fasc
