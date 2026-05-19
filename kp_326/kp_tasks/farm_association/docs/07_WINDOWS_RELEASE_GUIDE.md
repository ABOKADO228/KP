# 07. Windows Release: Как Это Работает

Документ объясняет готовую Windows-сборку из `windows-release`: почему нельзя надежно запускать production frontend двойным кликом по `index.html`, зачем нужен PostgreSQL рядом с `farm_association_server.exe`, и что делает `start-all.ps1`.

## Главная Идея

Готовая система состоит из трех работающих частей:

```text
Browser -> frontend static server -> React application
React application -> HTTP API -> C++ backend
C++ backend -> PostgreSQL -> данные системы
```

В `windows-release` эти части разложены так:

```text
windows-release/
  backend/
    farm_association_server.exe
    *.dll
    sql/
      farm_schema.sql
      user.sql

  frontend/
    index.html
    assets/
      *.js
      *.css

  postgresql/
    bin/
    lib/
    share/

  tools/
    serve-frontend.mjs

  start-all.ps1
  stop-all.ps1
  README.md
```

`backend` - это C++ Release-сервер и runtime DLL. `frontend` - результат `npm run build`. `postgresql` - локальные PostgreSQL binaries, чтобы не требовать установленный PostgreSQL в системе.

## Почему Недостаточно Открыть `index.html`

После `npm run build` frontend действительно содержит `index.html`, но это не самостоятельное desktop-приложение. HTML загружает JavaScript/CSS из `assets`, а JavaScript делает HTTP-запросы:

```text
POST /auth/login
POST /auth/register
GET  /users
GET  /api/<resource>
```

Если открыть `frontend/index.html` через двойной клик, браузер использует origin `file://`. Для такой страницы относительные API-запросы и ES modules ведут себя не так, как для обычного сайта:

```text
file:///.../frontend/index.html
file:///auth/login       неверный адрес API
file:///api/farm         неверный адрес API
```

Даже если конкретный браузер покажет первый экран, login/register не являются файловыми операциями. Им нужен HTTP backend.

Поэтому production frontend надо отдавать через HTTP:

```text
http://127.0.0.1:4173
```

В release-пакете это делает маленький Node.js static server `tools/serve-frontend.mjs`.

## Почему Недостаточно Запустить Только `.exe`

`farm_association_server.exe` - это HTTP backend. Он умеет:

```text
1. Читать настройки из environment variables.
2. Подключаться к PostgreSQL.
3. Создавать/инициализировать базу.
4. Создавать builtin admin.
5. Обслуживать /health, /auth, /users, /api.
```

Но сам `.exe` не является базой данных. Ему нужен запущенный PostgreSQL. Если PostgreSQL не слушает нужный порт, сервер не сможет пройти database bootstrap и endpoints регистрации/логина не будут работать.

В release-пакете PostgreSQL запускается локально:

```text
127.0.0.1:15432
```

Данные хранятся в:

```text
windows-release/postgres-data/
```

Эта папка создается при первом запуске.

## Как Собирается Release

Backend собирается как CMake Release build:

```powershell
cmake -S server -B server\build-release -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release -DFARM_SERVER_USE_CHECKED_IN_ODB=ON -DFARM_SERVER_BUILD_TESTS=OFF
cmake --build server\build-release --parallel
```

Флаг `FARM_SERVER_USE_CHECKED_IN_ODB=ON` означает, что сборка берет готовые ODB mapping files из `server/generated/persistence`, а не пытается регенерировать их ODB compiler-ом.

Frontend собирается как production build:

```powershell
$env:VITE_API_BASE_URL = "http://127.0.0.1:8080"
cd client
npm run build
```

`VITE_API_BASE_URL` попадает внутрь собранного JavaScript. Поэтому React-клиент из `windows-release/frontend` обращается к backend напрямую:

```text
http://127.0.0.1:8080/auth/login
http://127.0.0.1:8080/api/farm
```

Без этого параметра frontend использует относительные `/auth`, `/api`, что удобно для Vite proxy в dev-режиме, но неудобно для простого static release.

## Как Реализован Пакет `windows-release`

Пакет был собран как обычная файловая раскладка вокруг уже существующих build-артефактов. Никакой новый backend framework или installer не добавлялся.

Сначала был собран C++ backend:

```powershell
cmake -S server -B server\build-release -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release -DFARM_SERVER_USE_CHECKED_IN_ODB=ON -DFARM_SERVER_BUILD_TESTS=OFF
cmake --build server\build-release --parallel
```

После этого из `server/build-release` в `windows-release/backend` были перенесены:

```text
farm_association_server.exe
*.dll
sql/
```

`*.dll` нужны, потому что server executable динамически зависит от `libpq`, `OpenSSL`, runtime-библиотек PostgreSQL и MSVC runtime. CMake уже копирует PostgreSQL runtime DLL рядом с executable, поэтому release-папка берет их из build-каталога.

Frontend был собран отдельно:

```powershell
$env:VITE_API_BASE_URL = "http://127.0.0.1:8080"
cd client
npm run build
```

Результат `client/dist` был перенесен в:

```text
windows-release/frontend/
```

Важная деталь: `VITE_API_BASE_URL` встраивается в compiled JavaScript на этапе build. Поэтому production frontend из `windows-release/frontend` не ждет Vite proxy и обращается к backend напрямую:

```text
http://127.0.0.1:8080/auth/login
http://127.0.0.1:8080/users
http://127.0.0.1:8080/api/farm
```

Чтобы PostgreSQL не нужно было устанавливать отдельно, в пакет были скопированы runtime/tools из локальных зависимостей:

```text
server/third_party/postgresql/bin   -> windows-release/postgresql/bin
server/third_party/postgresql/lib   -> windows-release/postgresql/lib
server/third_party/postgresql/share -> windows-release/postgresql/share
```

`bin` содержит `initdb.exe`, `pg_ctl.exe`, `pg_isready.exe`, `postgres.exe`, `psql.exe` и нужные DLL. `share` нужен `initdb`, потому что там лежат шаблоны и служебные файлы PostgreSQL.

Итоговая структура создается так:

```text
windows-release/
  backend/      готовый C++ server
  frontend/     готовый static frontend
  postgresql/   локальный PostgreSQL runtime
  tools/        служебные скрипты
  logs/         создается/используется при запуске
```

## Что Делает `start-all.ps1`

Скрипт запускает все части в правильном порядке.

1. Проверяет, что есть `backend/farm_association_server.exe`, `frontend/index.html`, PostgreSQL tools и Node.js.
2. Если `postgres-data` еще нет, запускает `initdb`.
3. Запускает PostgreSQL через `pg_ctl`.
4. Выставляет environment variables для backend:

```text
FARM_DB_HOST=127.0.0.1
FARM_DB_PORT=15432
FARM_DB_USER=postgres
FARM_DB_PASSWORD=password
FARM_DB_MAINTENANCE_NAME=postgres
FARM_DB_SCHEMA_SQL=<release>/backend/sql/farm_schema.sql
FARM_DB_USER_SQL=<release>/backend/sql/user.sql
FARM_SERVER_ADDRESS=127.0.0.1
FARM_SERVER_PORT=8080
```

5. Запускает `backend/farm_association_server.exe`.
6. Ждет успешный ответ:

```text
GET http://127.0.0.1:8080/health
```

7. Запускает static server для `frontend`.
8. Печатает адреса и, если передан `-OpenBrowser`, открывает browser.

Итог:

```text
Frontend: http://127.0.0.1:4173
Backend:  http://127.0.0.1:8080
Database: 127.0.0.1:15432
```

### Реализация `start-all.ps1`

`start-all.ps1` принимает параметры портов:

```powershell
param(
    [int]$DbPort = 15432,
    [int]$BackendPort = 8080,
    [int]$FrontendPort = 4173,
    [switch]$OpenBrowser
)
```

Скрипт вычисляет пути относительно собственной папки, поэтому его можно запускать из `windows-release` без дополнительных переменных:

```text
$Root          -> windows-release
$BackendDir    -> windows-release/backend
$FrontendDir   -> windows-release/frontend
$PostgresDir   -> windows-release/postgresql
$PostgresData  -> windows-release/postgres-data
$LogsDir       -> windows-release/logs
```

При первом запуске он проверяет наличие `postgres-data/PG_VERSION`. Если файла нет, значит кластер еще не создан, и выполняется `initdb`:

```powershell
initdb.exe -D postgres-data -U postgres --pwfile=<temp-password-file> --auth=md5 --encoding=UTF8 --locale=C
```

Пароль локального пользователя PostgreSQL задается как `password`, потому что backend по умолчанию ожидает:

```text
FARM_DB_USER=postgres
FARM_DB_PASSWORD=password
```

После этого PostgreSQL запускается через `pg_ctl`:

```powershell
pg_ctl.exe -D postgres-data -l logs/postgres.log -o "-h 127.0.0.1 -p 15432" start -w
```

Флаг `-w` заставляет `pg_ctl` дождаться готовности сервера БД. Дополнительно используется `pg_isready`, чтобы не стартовать второй PostgreSQL на том же порту.

Backend запускается отдельным hidden-процессом:

```powershell
Start-Process backend/farm_association_server.exe `
  -WorkingDirectory backend `
  -RedirectStandardOutput logs/backend.out.log `
  -RedirectStandardError logs/backend.err.log `
  -WindowStyle Hidden `
  -PassThru
```

Перед запуском backend получает environment variables с портом БД, путями к SQL и HTTP-портом. Эти переменные видит только процесс backend, они не меняют глобальные настройки Windows.

После старта backend скрипт проверяет `/health`:

```powershell
Invoke-RestMethod http://127.0.0.1:8080/health
```

Frontend запускается через Node.js:

```powershell
node tools/serve-frontend.mjs --root frontend --host 127.0.0.1 --port 4173
```

PID backend и frontend записываются в:

```text
logs/backend.pid
logs/frontend.pid
```

Это нужно для `stop-all.ps1`.

### Реализация `serve-frontend.mjs`

`serve-frontend.mjs` - маленький static HTTP server на встроенном Node.js модуле `http`. Он делает четыре вещи:

```text
1. Отдает файлы из windows-release/frontend.
2. Ставит правильный Content-Type для html/js/css/json/png/svg.
3. Не дает выйти за пределы frontend-каталога через ../.
4. Для неизвестных routes возвращает index.html, чтобы React SPA могла работать после refresh.
```

Для assets ставится долгий cache:

```text
Cache-Control: public, max-age=31536000, immutable
```

Для `index.html` ставится:

```text
Cache-Control: no-cache
```

Этот server не проксирует API. Proxy не нужен, потому что frontend собран с абсолютным `VITE_API_BASE_URL`.

## Что Делает `stop-all.ps1`

Скрипт читает pid-файлы из `logs`, останавливает frontend static server, backend server и PostgreSQL:

```text
logs/frontend.pid
logs/backend.pid
postgres-data/postmaster.pid
```

Обычная команда остановки:

```powershell
.\stop-all.ps1
```

### Реализация `stop-all.ps1`

Скрипт читает `logs/frontend.pid` и `logs/backend.pid`, затем вызывает:

```powershell
Stop-Process -Id <pid> -Force
```

PostgreSQL останавливается штатно через `pg_ctl`, а не простым убийством процесса:

```powershell
pg_ctl.exe -D postgres-data stop -m fast -w
```

Так PostgreSQL успевает закрыть соединения и сохранить состояние базы.

## Почему Это Все Еще Не “Один EXE”

Текущая архитектура разделяет:

```text
React frontend
C++ HTTP backend
PostgreSQL database
```

Это нормальная web-схема. Один `.exe` будет возможен только если изменить backend: добавить в C++ сервер отдачу static frontend файлов из `frontend` и оставить PostgreSQL отдельным процессом или встроить другую БД.

Самый простой будущий вариант:

```text
farm_association_server.exe
  /              -> frontend/index.html
  /assets/*      -> frontend assets
  /auth/*        -> API
  /users         -> API
  /api/*         -> API
```

Тогда запуск frontend static server на Node.js станет не нужен, и пользователь будет открывать:

```text
http://127.0.0.1:8080
```

Но PostgreSQL все равно должен быть доступен, пока backend использует PostgreSQL как основное хранилище.

## Типовые Ошибки

`502 Bad Gateway`
: это ошибка Vite dev proxy. В release-сценарии Vite не используется. Если видишь 502, значит запущен dev frontend, а backend не слушает адрес, на который настроен proxy.

`Failed to fetch` в browser
: frontend открыт, но backend недоступен на `http://127.0.0.1:8080`. Проверь `Invoke-RestMethod http://127.0.0.1:8080/health`.

`connection refused` к PostgreSQL
: backend запущен, но PostgreSQL не слушает `FARM_DB_HOST:FARM_DB_PORT`. Проверь `logs/postgres.log` и порт `15432`.

`could not bind IPv4 address ... Permission denied`
: на Windows порт может быть зарезервирован системой. Проверь:

```powershell
netsh interface ipv4 show excludedportrange protocol=tcp
```

и запусти release на другом порту:

```powershell
.\start-all.ps1 -DbPort 15440 -BackendPort 8080 -FrontendPort 4173
```

`MSVCP140.dll` или `VCRUNTIME140.dll` не найдены
: установи Microsoft Visual C++ Redistributable 2015-2022 x64.

## Синтаксис Release-Обвязки

Этот раздел делает упор на синтаксис команд и скриптов, которые используются в готовой Windows-сборке.

### PowerShell: `param`

`start-all.ps1` начинается с описания параметров:

```powershell
param(
    [int]$DbPort = 15432,
    [int]$BackendPort = 8080,
    [int]$FrontendPort = 4173,
    [switch]$OpenBrowser
)
```

Синтаксис `[int]$DbPort = 15432` означает: параметр должен быть числом, а значение по умолчанию равно `15432`.

Синтаксис `[switch]$OpenBrowser` означает флаг без отдельного значения:

```powershell
.\start-all.ps1 -OpenBrowser
```

Переопределение портов выглядит так:

```powershell
.\start-all.ps1 -DbPort 15440 -BackendPort 18080 -FrontendPort 4174
```

### PowerShell: Переменные И Пути

PowerShell-переменные начинаются с `$`:

```powershell
$Root = Split-Path -Parent $MyInvocation.MyCommand.Path
$BackendDir = Join-Path $Root "backend"
$FrontendDir = Join-Path $Root "frontend"
```

`$MyInvocation.MyCommand.Path` - путь к текущему `.ps1` файлу. Поэтому `start-all.ps1` находит свои папки относительно `windows-release`, а не относительно случайного каталога, из которого открыт терминал.

`Join-Path` используется для сборки путей:

```powershell
Join-Path $Root "backend"
```

Это надежнее, чем вручную писать:

```powershell
"$Root\backend"
```

### PowerShell: Проверки И Ошибки

Проверка существования файла:

```powershell
if (!(Test-Path $BackendExe)) {
    throw "Backend executable not found: $BackendExe"
}
```

`Test-Path` возвращает `true` или `false`. Конструкция `!()` означает отрицание. `throw` останавливает скрипт с ошибкой.

Создание каталога:

```powershell
New-Item -ItemType Directory -Force -Path $LogsDir | Out-Null
```

`-Force` не падает, если каталог уже есть. `| Out-Null` скрывает лишний вывод команды.

### PowerShell: Environment Variables

Backend читает настройки из environment variables. В PowerShell они задаются через `$env:`:

```powershell
$env:FARM_DB_HOST = "127.0.0.1"
$env:FARM_DB_PORT = "$DbPort"
$env:FARM_DB_USER = "postgres"
$env:FARM_DB_PASSWORD = "password"
$env:FARM_SERVER_PORT = "$BackendPort"
```

Такие переменные видит текущий PowerShell-процесс и дочерние процессы. Поэтому `farm_association_server.exe`, запущенный через `Start-Process`, получает эти значения.

Число превращается в строку через интерполяцию:

```powershell
"$DbPort"
```

Environment variables всегда передаются как строки.

### PowerShell: Многострочная Команда

В PowerShell обратная кавычка продолжает команду на следующей строке:

```powershell
$BackendProcess = Start-Process -FilePath $BackendExe `
    -WorkingDirectory $BackendDir `
    -RedirectStandardOutput $BackendOut `
    -RedirectStandardError $BackendErr `
    -WindowStyle Hidden `
    -PassThru
```

Ключи `Start-Process`:

```text
-FilePath                 executable
-WorkingDirectory         рабочая папка процесса
-RedirectStandardOutput   файл stdout
-RedirectStandardError    файл stderr
-WindowStyle Hidden       без отдельного видимого окна
-PassThru                 вернуть объект процесса
```

После `-PassThru` можно получить PID:

```powershell
$BackendProcess.Id
```

и записать его в файл:

```powershell
Set-Content -LiteralPath (Join-Path $LogsDir "backend.pid") -Value $BackendProcess.Id -Encoding ASCII
```

### PowerShell: Цикл Ожидания

Backend не становится готовым мгновенно, поэтому `start-all.ps1` ждет `/health`:

```powershell
$BackendReady = $false
for ($i = 0; $i -lt 60; $i++) {
    try {
        Invoke-RestMethod -Uri $HealthUrl -TimeoutSec 2 | Out-Null
        $BackendReady = $true
        break
    } catch {
        Start-Sleep -Milliseconds 500
    }
}
```

`-lt` означает `less than`, то есть “меньше чем”. `try/catch` нужен, потому что первые HTTP-запросы могут падать, пока backend еще запускается.

### PowerShell: Hashtable Для HTTP Headers

HTTP headers задаются hashtable-синтаксисом:

```powershell
@{ "Content-Type" = "application/json" }
```

Пример login-запроса:

```powershell
Invoke-RestMethod `
  -Method Post `
  -Uri http://127.0.0.1:8080/auth/login `
  -Headers @{ "Content-Type" = "application/json" } `
  -Body '{"login":"admin","password":"admin12345"}'
```

`Invoke-RestMethod` превращает JSON-ответ в PowerShell object, поэтому поля читаются так:

```powershell
$response.user.role
$response.token
```

### CMake: `-S`, `-B`, `-D`

Команда configure:

```powershell
cmake -S server -B server\build-release -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release -DFARM_SERVER_USE_CHECKED_IN_ODB=ON -DFARM_SERVER_BUILD_TESTS=OFF
```

Синтаксис:

```text
-S server                         каталог исходников
-B server\build-release           каталог build output
-G Ninja                          генератор build-системы
-DNAME=VALUE                      CMake variable или option
```

Важные `-D` параметры:

```text
CMAKE_CXX_COMPILER=clang++
CMAKE_BUILD_TYPE=Release
FARM_SERVER_USE_CHECKED_IN_ODB=ON
FARM_SERVER_BUILD_TESTS=OFF
```

Команда build:

```powershell
cmake --build server\build-release --parallel
```

`--parallel` разрешает параллельную сборку.

### npm И Vite: Сборка Frontend

Перед `npm run build` задается Vite-переменная:

```powershell
$env:VITE_API_BASE_URL = "http://127.0.0.1:8080"
```

`VITE_` - важный префикс. Vite передает frontend-коду только переменные с таким префиксом.

В `package.json` build script выглядит так:

```json
{
  "scripts": {
    "build": "tsc -b && vite build"
  }
}
```

Синтаксис `&&` означает: выполнить вторую команду только если первая завершилась успешно.

```text
tsc -b      TypeScript build/check
vite build  production bundle
```

В frontend-коде переменная читается так:

```ts
const defaultApiBaseUrl =
  import.meta.env.VITE_API_BASE_URL ?? "";
```

Оператор `??` означает “если слева не `null` и не `undefined`, взять левое, иначе правое”.

Если `VITE_API_BASE_URL` задан, запрос собирается как:

```text
http://127.0.0.1:8080/auth/login
```

Если не задан, остается относительный путь:

```text
/auth/login
```

### Node.js ESM: `serve-frontend.mjs`

Файл `serve-frontend.mjs` использует ESM-синтаксис:

```js
import { createReadStream, existsSync, statSync } from "node:fs";
import { createServer } from "node:http";
import { extname, join, normalize, resolve, sep } from "node:path";
```

Префикс `node:` означает встроенный модуль Node.js.

Аргументы командной строки лежат в `process.argv`:

```js
const args = new Map();
for (let i = 2; i < process.argv.length; i += 2) {
  args.set(process.argv[i], process.argv[i + 1]);
}
```

Для команды:

```powershell
node tools/serve-frontend.mjs --root frontend --host 127.0.0.1 --port 4173
```

получаются значения:

```text
args.get("--root") -> frontend
args.get("--host") -> 127.0.0.1
args.get("--port") -> 4173
```

HTTP server создается через callback:

```js
const server = createServer((request, response) => {
  const filePath = resolveRequestPath(request.url ?? "/");
  // отправка файла или 404
});
```

Синтаксис `request.url ?? "/"` означает: если `request.url` отсутствует, использовать `/`.

Файл отдается потоково:

```js
createReadStream(filePath).pipe(response);
```

`pipe` соединяет поток чтения файла с HTTP response.

### Node.js: Защита Пути

Static server не должен отдавать файлы вне `frontend`. Поэтому путь нормализуется и проверяется:

```js
const candidate = resolve(root, relativePath);
const rootWithSep = root.endsWith(sep) ? root : `${root}${sep}`;

if (candidate !== root && !candidate.startsWith(rootWithSep)) {
  return null;
}
```

Синтаксис:

```js
condition ? valueIfTrue : valueIfFalse
```

это ternary operator.

Проверка `startsWith(rootWithSep)` защищает от путей вроде:

```text
../../secret.txt
```

### PostgreSQL CLI

Создание локального кластера:

```powershell
initdb.exe -D postgres-data -U postgres --pwfile=<file> --auth=md5 --encoding=UTF8 --locale=C
```

Смысл ключей:

```text
-D postgres-data       каталог данных
-U postgres            имя суперпользователя
--pwfile=<file>        файл с паролем
--auth=md5             парольная аутентификация
--encoding=UTF8        кодировка базы
--locale=C             стабильная locale для bootstrap
```

Запуск PostgreSQL:

```powershell
pg_ctl.exe -D postgres-data -l logs/postgres.log -o "-h 127.0.0.1 -p 15432" start -w
```

Смысл ключей:

```text
-D postgres-data       где лежит кластер
-l logs/postgres.log   куда писать лог
-o "..."               аргументы для postgres.exe
start                  запустить сервер
-w                     дождаться запуска
```

Остановка:

```powershell
pg_ctl.exe -D postgres-data stop -m fast -w
```

`-m fast` закрывает активные соединения и завершает PostgreSQL штатно.

## Минимальная Проверка

После запуска:

```powershell
Invoke-RestMethod http://127.0.0.1:8080/health
Invoke-WebRequest http://127.0.0.1:4173
```

Проверка логина:

```powershell
Invoke-RestMethod `
  -Method Post `
  -Uri http://127.0.0.1:8080/auth/login `
  -Headers @{ "Content-Type" = "application/json" } `
  -Body '{"login":"admin","password":"admin12345"}'
```

Ожидаемая роль:

```text
agriculture_admin
```

## Как Проверялась Реализация

После сборки был проверен реальный пользовательский сценарий из `windows-release`:

```powershell
cd windows-release
Set-ExecutionPolicy -Scope Process Bypass
.\start-all.ps1
```

Проверки после запуска:

```powershell
Invoke-RestMethod http://127.0.0.1:8080/health
Invoke-WebRequest http://127.0.0.1:4173
Invoke-RestMethod `
  -Method Post `
  -Uri http://127.0.0.1:8080/auth/login `
  -Headers @{ "Content-Type" = "application/json" } `
  -Body '{"login":"admin","password":"admin12345"}'
```

Фактический результат:

```text
Backend /health: ok
Frontend /: HTTP 200
Admin login: agriculture_admin + JWT token
```

После проверки выполнялась остановка:

```powershell
.\stop-all.ps1
```

И дополнительно проверялось, что не остались процессы `farm_association_server`, `postgres` и frontend `node`, а порты `15432`, `8080`, `4173` освободились.
