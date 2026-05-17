import { describe, expect, it } from "vitest";

import type { BusinessModule } from "./roles";
import { formColumnsFor, keyFromRow } from "./resourceFields";

const baseModule = {
  area: "farms",
  endpoint: "/api/farm",
  icon: "farm",
  process: "test",
  title: "Test",
  visibleBy: ["agriculture_admin"],
  writableBy: ["agriculture_admin"],
} satisfies Omit<BusinessModule, "defaultColumns" | "id">;

describe("resource key fields", () => {
  it("extracts the configured composite key from a row", () => {
    const module: BusinessModule = {
      ...baseModule,
      defaultColumns: ["farm_id", "farm_plot_id", "status"],
      endpoint: "/api/farm_plot_assignment",
      id: "farm_plot_assignment",
      keyColumns: ["farm_id", "farm_plot_id"],
    };

    expect(
      keyFromRow(module, {
        farm_id: 1,
        farm_plot_id: 61,
        status: "active",
      }),
    ).toEqual({
      farm_id: 1,
      farm_plot_id: 61,
    });
  });

  it("does not expose key columns in edit forms", () => {
    const module: BusinessModule = {
      ...baseModule,
      defaultColumns: ["farm_id", "association_id", "join_date", "status"],
      endpoint: "/api/association_farms",
      id: "association_farms",
      keyColumns: ["farm_id", "association_id"],
    };

    expect(formColumnsFor(module, [], "create")).toEqual([
      "farm_id",
      "association_id",
      "join_date",
      "status",
    ]);
    expect(formColumnsFor(module, [], "edit")).toEqual(["join_date", "status"]);
  });

  it("uses id as the default simple key and hides generated ids on create", () => {
    const module: BusinessModule = {
      ...baseModule,
      defaultColumns: ["id", "name", "status"],
      id: "farm",
    };

    expect(keyFromRow(module, { id: 2, name: "farm-key-check" })).toEqual({ id: 2 });
    expect(formColumnsFor(module, [], "create")).toEqual(["name", "status"]);
    expect(formColumnsFor(module, [], "edit")).toEqual(["name", "status"]);
  });
});
