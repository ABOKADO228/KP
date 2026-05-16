import {
  Building2,
  FileText,
  Leaf,
  Package,
  ShoppingCart,
  Tractor,
  Truck,
  Users,
  type LucideIcon,
} from "lucide-react";

import type { ModuleIcon } from "../domain/roles";

export const moduleIcons: Record<ModuleIcon, LucideIcon> = {
  association: Building2,
  contract: FileText,
  farm: Tractor,
  field: Leaf,
  member: Users,
  order: ShoppingCart,
  product: Package,
  supplier: Truck,
};
