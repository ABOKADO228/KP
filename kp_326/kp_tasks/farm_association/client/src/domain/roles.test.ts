import { describe, expect, it } from "vitest";

import {
  businessModules,
  canAccessModule,
  canMutateModule,
  getAccessibleModules,
} from "./roles";

describe("role access map", () => {
  it("gives agriculture administrator full module access", () => {
    expect(getAccessibleModules("agriculture_admin")).toHaveLength(
      businessModules.length,
    );
    expect(
      businessModules.every((module) =>
        canMutateModule("agriculture_admin", module),
      ),
    ).toBe(true);
  });

  it("keeps farm owner focused on farm operations", () => {
    const modules = getAccessibleModules("farm_owner").map((module) => module.id);

    expect(modules).toContain("farm");
    expect(modules).toContain("purchase_requisition");
    expect(modules).toContain("sales_requisition");
    expect(modules).not.toContain("supplier");
    expect(modules).not.toContain("purchase_order");
  });

  it("does not duplicate business module ids", () => {
    const ids = businessModules.map((module) => module.id);
    expect(new Set(ids).size).toBe(ids.length);
  });

  it("keeps farm worker out of association administration", () => {
    const associationModule = businessModules.find(
      (module) => module.id === "farm_association",
    );

    expect(associationModule).toBeDefined();
    expect(canAccessModule("farm_worker", associationModule!)).toBe(false);
  });
});
