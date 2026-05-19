# Готовая Windows-Сборка

Готовый пакет лежит в корне проекта:

```text
windows-release/
```

Запуск:

```powershell
cd windows-release
Set-ExecutionPolicy -Scope Process Bypass
.\start-all.ps1 -OpenBrowser
```

Адреса после запуска:

```text
Frontend: http://127.0.0.1:4173
Backend:  http://127.0.0.1:8080
Admin:    admin / admin12345
```

Остановка:

```powershell
.\stop-all.ps1
```

Подробности, логи и настройка портов описаны в [windows-release/README.md](windows-release/README.md).
