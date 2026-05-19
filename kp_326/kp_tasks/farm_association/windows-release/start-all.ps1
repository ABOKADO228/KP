param(
    [int]$DbPort = 15432,
    [int]$BackendPort = 8080,
    [int]$FrontendPort = 4173,
    [switch]$OpenBrowser
)

$ErrorActionPreference = "Stop"

$Root = Split-Path -Parent $MyInvocation.MyCommand.Path
$BackendDir = Join-Path $Root "backend"
$FrontendDir = Join-Path $Root "frontend"
$PostgresDir = Join-Path $Root "postgresql"
$PostgresData = Join-Path $Root "postgres-data"
$LogsDir = Join-Path $Root "logs"
$BackendExe = Join-Path $BackendDir "farm_association_server.exe"
$PgBin = Join-Path $PostgresDir "bin"
$PgCtl = Join-Path $PgBin "pg_ctl.exe"
$InitDb = Join-Path $PgBin "initdb.exe"
$PgReady = Join-Path $PgBin "pg_isready.exe"
$ServeScript = Join-Path $Root "tools\serve-frontend.mjs"

New-Item -ItemType Directory -Force -Path $LogsDir | Out-Null

if (!(Test-Path $BackendExe)) {
    throw "Backend executable not found: $BackendExe"
}
if (!(Test-Path (Join-Path $FrontendDir "index.html"))) {
    throw "Frontend build not found: $FrontendDir"
}
if (!(Test-Path $PgCtl) -or !(Test-Path $InitDb)) {
    throw "PostgreSQL tools not found in $PgBin"
}
if (!(Get-Command node -ErrorAction SilentlyContinue)) {
    throw "Node.js is required to serve frontend. Install Node.js or run frontend with another static HTTP server."
}

if (!(Test-Path (Join-Path $PostgresData "PG_VERSION"))) {
    New-Item -ItemType Directory -Force -Path $PostgresData | Out-Null
    $PasswordFile = Join-Path $LogsDir "postgres-password.txt"
    Set-Content -LiteralPath $PasswordFile -Value "password" -NoNewline -Encoding ASCII
    & $InitDb -D $PostgresData -U postgres --pwfile=$PasswordFile --auth=md5 --encoding=UTF8 --locale=C
    if ($LASTEXITCODE -ne 0) {
        throw "PostgreSQL initdb failed with code $LASTEXITCODE"
    }
    Remove-Item -LiteralPath $PasswordFile -Force
}

& $PgReady -h 127.0.0.1 -p $DbPort | Out-Null
if ($LASTEXITCODE -ne 0) {
    $PgLog = Join-Path $LogsDir "postgres.log"
    & $PgCtl -D $PostgresData -l $PgLog -o "-h 127.0.0.1 -p $DbPort" start -w
    if ($LASTEXITCODE -ne 0) {
        throw "PostgreSQL start failed with code $LASTEXITCODE. See $PgLog"
    }
}

$env:FARM_DB_HOST = "127.0.0.1"
$env:FARM_DB_PORT = "$DbPort"
$env:FARM_DB_USER = "postgres"
$env:FARM_DB_PASSWORD = "password"
$env:FARM_DB_MAINTENANCE_NAME = "postgres"
$env:FARM_DB_SCHEMA_SQL = (Join-Path $BackendDir "sql\farm_schema.sql")
$env:FARM_DB_USER_SQL = (Join-Path $BackendDir "sql\user.sql")
$env:FARM_SERVER_ADDRESS = "127.0.0.1"
$env:FARM_SERVER_PORT = "$BackendPort"

$BackendOut = Join-Path $LogsDir "backend.out.log"
$BackendErr = Join-Path $LogsDir "backend.err.log"
$BackendProcess = Start-Process -FilePath $BackendExe `
    -WorkingDirectory $BackendDir `
    -RedirectStandardOutput $BackendOut `
    -RedirectStandardError $BackendErr `
    -WindowStyle Hidden `
    -PassThru
Set-Content -LiteralPath (Join-Path $LogsDir "backend.pid") -Value $BackendProcess.Id -Encoding ASCII

$HealthUrl = "http://127.0.0.1:$BackendPort/health"
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
if (!$BackendReady) {
    throw "Backend did not answer $HealthUrl. See $BackendOut and $BackendErr"
}

$FrontendOut = Join-Path $LogsDir "frontend.out.log"
$FrontendErr = Join-Path $LogsDir "frontend.err.log"
$FrontendProcess = Start-Process -FilePath "node" `
    -ArgumentList @($ServeScript, "--root", $FrontendDir, "--host", "127.0.0.1", "--port", "$FrontendPort") `
    -WorkingDirectory $Root `
    -RedirectStandardOutput $FrontendOut `
    -RedirectStandardError $FrontendErr `
    -WindowStyle Hidden `
    -PassThru
Set-Content -LiteralPath (Join-Path $LogsDir "frontend.pid") -Value $FrontendProcess.Id -Encoding ASCII

$FrontendUrl = "http://127.0.0.1:$FrontendPort"
for ($i = 0; $i -lt 20; $i++) {
    try {
        Invoke-WebRequest -Uri $FrontendUrl -TimeoutSec 2 | Out-Null
        break
    } catch {
        Start-Sleep -Milliseconds 500
    }
}

Write-Host ""
Write-Host "Farm Association started"
Write-Host "Frontend: $FrontendUrl"
Write-Host "Backend health: $HealthUrl"
Write-Host "Builtin admin: admin / admin12345"
Write-Host "Logs: $LogsDir"
Write-Host "Stop: .\stop-all.ps1"

if ($OpenBrowser) {
    Start-Process $FrontendUrl
}
