# Windows Release

Эта папка содержит готовую Windows-сборку системы фермерской ассоциации:

```text
backend/      C++ Release server, DLL и SQL bootstrap files
frontend/     production React build
postgresql/   локальный PostgreSQL runtime/tools
tools/        маленький Node.js static server для frontend
logs/         логи и pid-файлы после запуска
```

## Быстрый Запуск

Открой PowerShell в этой папке и выполни:

```powershell
Set-ExecutionPolicy -Scope Process Bypass
.\start-all.ps1 -OpenBrowser
```

После запуска:

```text
Frontend: http://127.0.0.1:4173
Backend:  http://127.0.0.1:8080
Admin:    admin / admin12345
```

Остановка:

```powershell
.\stop-all.ps1
```

## Что Делает `start-all.ps1`

1. Создает локальную базу PostgreSQL в `postgres-data`, если ее еще нет.
2. Запускает PostgreSQL на `127.0.0.1:15432`.
3. Запускает `backend/farm_association_server.exe` на `127.0.0.1:8080`.
4. Передает серверу локальные SQL-файлы из `backend/sql`.
5. Запускает frontend static server на `127.0.0.1:4173`.

Frontend собран с `VITE_API_BASE_URL=http://127.0.0.1:8080`, поэтому browser напрямую обращается к backend. Vite dev server для этой сборки не нужен.

## Настройки Портов

Порты можно переопределить:

```powershell
.\start-all.ps1 -DbPort 15432 -BackendPort 8080 -FrontendPort 4173
```

Если порт занят или зарезервирован Windows, выбери другой. Проверить занятые и зарезервированные порты можно так:

```powershell
netstat -ano | findstr :8080
netsh interface ipv4 show excludedportrange protocol=tcp
```

## Логи

Логи пишутся в `logs`:

```text
logs/postgres.log
logs/backend.out.log
logs/backend.err.log
logs/frontend.out.log
logs/frontend.err.log
```

Если frontend открылся, но login/register не работают, проверь:

```powershell
Invoke-RestMethod http://127.0.0.1:8080/health
Get-Content .\logs\backend.err.log -Tail 80
Get-Content .\logs\postgres.log -Tail 80
```

## Требования

Нужен Windows x64 и Node.js в `PATH` для запуска frontend static server.

Если `farm_association_server.exe` жалуется на `MSVCP140.dll` или `VCRUNTIME140.dll`, установи Microsoft Visual C++ Redistributable 2015-2022 x64.

## Содержимое Сборки

Backend собран как `Release` через CMake + Ninja + clang++ с checked-in ODB generated files. Frontend собран командой `npm run build`.
