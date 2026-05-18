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

/// Составной ключ таблицы farm_plot_assignment.
struct FarmPlotAssignmentKey {
  FarmPlotAssignmentKey() = default;
  FarmPlotAssignmentKey(std::uint64_t farmIdValue, std::uint64_t farmPlotIdValue)
      : farmId(farmIdValue), farmPlotId(farmPlotIdValue) {}

  std::uint64_t farmId{};
  std::uint64_t farmPlotId{};
};

/// Сущность таблицы farm_plot_assignment.
struct FarmPlotAssignmentEntity {
  /// Значение колонки farm_id.
  std::uint64_t farmId{};

  /// Значение колонки farm_plot_id.
  std::uint64_t farmPlotId{};

  /// Значение колонки status.
  odb::nullable<fasc::server::domain::FarmPlotAssignmentStatus> status;

  /// Значение колонки notes.
  odb::nullable<std::string> notes;

  FarmPlotAssignmentKey id() const {
    return FarmPlotAssignmentKey{farmId, farmPlotId};
  }

  void id(const FarmPlotAssignmentKey& key) {
    farmId = key.farmId;
    farmPlotId = key.farmPlotId;
  }

};

#pragma db value(FarmPlotAssignmentKey)
#pragma db member(FarmPlotAssignmentKey::farmId) column("farm_id")
#pragma db member(FarmPlotAssignmentKey::farmPlotId) column("farm_plot_id")

#pragma db object(FarmPlotAssignmentEntity) table("farm_plot_assignment")
#pragma db member(FarmPlotAssignmentEntity::id) virtual(FarmPlotAssignmentKey) access(id) id column("")
#pragma db member(FarmPlotAssignmentEntity::farmId) transient
#pragma db member(FarmPlotAssignmentEntity::farmPlotId) transient
#pragma db member(FarmPlotAssignmentEntity::status) column("status")
#pragma db member(FarmPlotAssignmentEntity::notes) column("notes")

} // namespace persistence
} // namespace server
} // namespace fasc
