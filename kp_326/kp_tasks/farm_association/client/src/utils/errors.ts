import { ApiRequestError } from "../api/farmApi";

export function errorMessage(caught: unknown): string {
  if (caught instanceof ApiRequestError) {
    return `${caught.status}: ${caught.message}`;
  }
  if (caught instanceof Error) {
    return caught.message;
  }
  return "Неизвестная ошибка";
}
