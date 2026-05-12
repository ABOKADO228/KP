param(
    [switch]$Force
)

$ErrorActionPreference = "Stop"

$Root = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$Deps = Join-Path $Root "server\third_party"
$Downloads = Join-Path $Deps "_downloads"
$Sources = Join-Path $Deps "_sources"

function Ensure-Directory($Path) {
    if (-not (Test-Path -LiteralPath $Path)) {
        New-Item -ItemType Directory -Path $Path | Out-Null
    }
}

function Download-File($Url, $Output) {
    if ((Test-Path -LiteralPath $Output) -and -not $Force) {
        Write-Host "exists $Output"
        return
    }

    Write-Host "download $Url"
    Invoke-WebRequest -Uri $Url -OutFile $Output
}

function Expand-Zip($Archive, $Destination) {
    if ((Test-Path -LiteralPath $Destination) -and -not $Force) {
        Write-Host "exists $Destination"
        return
    }

    if (Test-Path -LiteralPath $Destination) {
        Remove-Item -LiteralPath $Destination -Recurse -Force
    }

    Ensure-Directory $Sources
    Write-Host "extract $Archive"
    Expand-Archive -LiteralPath $Archive -DestinationPath $Sources -Force
}

function Copy-Directory($From, $To) {
    if (Test-Path -LiteralPath $To) {
        Remove-Item -LiteralPath $To -Recurse -Force
    }

    Ensure-Directory (Split-Path -Parent $To)
    Copy-Item -LiteralPath $From -Destination $To -Recurse
}

Ensure-Directory $Deps
Ensure-Directory $Downloads
Ensure-Directory $Sources

$packages = @(
    @{
        Name = "boost_1_90_0"
        Url = "https://archives.boost.io/release/1.90.0/source/boost_1_90_0.zip"
        Archive = "boost_1_90_0.zip"
        Source = "boost_1_90_0"
    },
    @{
        Name = "nlohmann-json-3.12.0"
        Url = "https://github.com/nlohmann/json/archive/refs/tags/v3.12.0.zip"
        Archive = "nlohmann-json-3.12.0.zip"
        Source = "json-3.12.0"
    },
    @{
        Name = "fmt-12.1.0"
        Url = "https://github.com/fmtlib/fmt/archive/refs/tags/12.1.0.zip"
        Archive = "fmt-12.1.0.zip"
        Source = "fmt-12.1.0"
    },
    @{
        Name = "googletest-1.17.0"
        Url = "https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip"
        Archive = "googletest-1.17.0.zip"
        Source = "googletest-1.17.0"
    },
    @{
        Name = "odb-2.4.0-i686-windows"
        Url = "https://www.codesynthesis.com/download/odb/2.4/odb-2.4.0-i686-windows.zip"
        Archive = "odb-2.4.0-i686-windows.zip"
        Source = "odb-2.4.0-i686-windows"
    },
    @{
        Name = "libodb-2.4.0"
        Url = "https://www.codesynthesis.com/download/odb/2.4/libodb-2.4.0.zip"
        Archive = "libodb-2.4.0.zip"
        Source = "libodb-2.4.0"
    },
    @{
        Name = "libodb-pgsql-2.4.0"
        Url = "https://www.codesynthesis.com/download/odb/2.4/libodb-pgsql-2.4.0.zip"
        Archive = "libodb-pgsql-2.4.0.zip"
        Source = "libodb-pgsql-2.4.0"
    },
    @{
        Name = "postgresql-18.3-1-windows-x64-binaries"
        Url = "https://get.enterprisedb.com/postgresql/postgresql-18.3-1-windows-x64-binaries.zip"
        Archive = "postgresql-18.3-1-windows-x64-binaries.zip"
        Source = "pgsql"
    }
)

foreach ($package in $packages) {
    $archive = Join-Path $Downloads $package.Archive
    Download-File $package.Url $archive
    Expand-Zip $archive (Join-Path $Sources $package.Source)
}

Copy-Directory (Join-Path $Sources "boost_1_90_0\boost") (Join-Path $Deps "boost\include\boost")
Copy-Directory (Join-Path $Sources "json-3.12.0\include\nlohmann") (Join-Path $Deps "nlohmann_json\include\nlohmann")
Copy-Directory (Join-Path $Sources "odb-2.4.0-i686-windows\bin") (Join-Path $Deps "odb\bin")
Copy-Directory (Join-Path $Sources "pgsql") (Join-Path $Deps "postgresql")

Write-Host ""
Write-Host "Dependencies are ready in $Deps"
Write-Host "Configure example:"
Write-Host "  cmake -S server -B server\build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug"
