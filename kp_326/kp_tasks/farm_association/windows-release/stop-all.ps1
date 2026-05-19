$ErrorActionPreference = "Continue"

$Root = Split-Path -Parent $MyInvocation.MyCommand.Path
$LogsDir = Join-Path $Root "logs"
$PostgresData = Join-Path $Root "postgres-data"
$PgCtl = Join-Path $Root "postgresql\bin\pg_ctl.exe"

foreach ($name in @("frontend", "backend")) {
    $PidFile = Join-Path $LogsDir "$name.pid"
    if (Test-Path $PidFile) {
        $ProcessId = Get-Content -LiteralPath $PidFile | Select-Object -First 1
        if ($ProcessId) {
            Stop-Process -Id ([int]$ProcessId) -Force -ErrorAction SilentlyContinue
        }
        Remove-Item -LiteralPath $PidFile -Force -ErrorAction SilentlyContinue
    }
}

if ((Test-Path $PgCtl) -and (Test-Path (Join-Path $PostgresData "PG_VERSION"))) {
    & $PgCtl -D $PostgresData stop -m fast -w
}

Write-Host "Farm Association stopped"
