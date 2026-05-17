import { describe, expect, it } from "vitest";

import { filterResourceRows, type ResourceFilter } from "./resourceFilters";

const rows = [
  { id: 1, name: "Ферма Север", status: "active", area: 120 },
  { id: 2, name: "Южный сад", status: "inactive", area: 75 },
  { id: 3, name: "Поле Восток", status: "", area: 210 },
];

describe("resource table filters", () => {
  it("searches across configured columns", () => {
    expect(filterResourceRows(rows, ["name", "status"], "сад", [])).toEqual([
      rows[1],
    ]);
  });

  it("combines search and exact filters", () => {
    const filters: ResourceFilter[] = [
      { column: "status", id: "status", operator: "equals", value: "active" },
    ];

    expect(filterResourceRows(rows, ["name", "status"], "ферма", filters)).toEqual([
      rows[0],
    ]);
  });

  it("supports empty and not-empty filters", () => {
    expect(
      filterResourceRows(rows, ["name", "status"], "", [
        { column: "status", id: "empty", operator: "empty", value: "" },
      ]),
    ).toEqual([rows[2]]);

    expect(
      filterResourceRows(rows, ["name", "status"], "", [
        { column: "status", id: "filled", operator: "not_empty", value: "" },
      ]),
    ).toEqual([rows[0], rows[1]]);
  });
});
