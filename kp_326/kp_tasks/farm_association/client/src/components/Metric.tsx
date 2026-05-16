import type { LucideIcon } from "lucide-react";

interface MetricProps {
  icon: LucideIcon;
  label: string;
  value: number;
  tone: "amber" | "blue" | "green";
}

export function Metric({ icon: Icon, label, value, tone }: MetricProps) {
  return (
    <article className={`metric ${tone}`}>
      <Icon aria-hidden="true" size={20} />
      <div>
        <span>{label}</span>
        <strong>{value}</strong>
      </div>
    </article>
  );
}
