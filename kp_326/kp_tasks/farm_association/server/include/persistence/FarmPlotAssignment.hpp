#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы farm_plot_assignment.
struct FarmPlotAssignmentEntity {
  /// Значение колонки farm_id.
  int farmId{};

  /// Значение колонки farm_plot_id.
  int farmPlotId{};

  /// Значение колонки status.
  odb::nullable<std::string> status;

  /// Значение колонки notes.
  odb::nullable<std::string> notes;

};

#pragma db object(FarmPlotAssignmentEntity) table("farm_plot_assignment")
#pragma db member(FarmPlotAssignmentEntity::farmId) column("farm_id")
#pragma db member(FarmPlotAssignmentEntity::farmPlotId) column("farm_plot_id")
#pragma db member(FarmPlotAssignmentEntity::status) column("status")
#pragma db member(FarmPlotAssignmentEntity::notes) column("notes")

} // namespace fasc::server::persistence
