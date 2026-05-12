#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DEPS="$ROOT/server/third_party"
DOWNLOADS="$DEPS/_downloads"
SOURCES="$DEPS/_sources"
FORCE="${FORCE:-0}"

need_cmd() {
  if ! command -v "$1" >/dev/null 2>&1; then
    echo "Missing command: $1" >&2
    exit 1
  fi
}

download() {
  local url="$1"
  local output="$2"

  if [[ -f "$output" && "$FORCE" != "1" ]]; then
    echo "exists $output"
    return
  fi

  echo "download $url"
  curl -L "$url" -o "$output"
}

extract_zip() {
  local archive="$1"
  local expected="$2"

  if [[ -d "$expected" && "$FORCE" != "1" ]]; then
    echo "exists $expected"
    return
  fi

  rm -rf "$expected"
  echo "extract $archive"
  unzip -q "$archive" -d "$SOURCES"
}

extract_tar_bz2() {
  local archive="$1"
  local expected="$2"

  if [[ -d "$expected" && "$FORCE" != "1" ]]; then
    echo "exists $expected"
    return
  fi

  rm -rf "$expected"
  echo "extract $archive"
  tar -xjf "$archive" -C "$SOURCES"
}

need_cmd curl
need_cmd unzip
need_cmd tar
need_cmd cmake

mkdir -p "$DOWNLOADS" "$SOURCES"

if command -v pacman >/dev/null 2>&1; then
  missing=()
  for pkg in base-devel cmake ninja clang postgresql-libs; do
    if ! pacman -Q "$pkg" >/dev/null 2>&1; then
      missing+=("$pkg")
    fi
  done

  if (( ${#missing[@]} > 0 )); then
    echo "Install required Manjaro/Arch packages first:"
    echo "  sudo pacman -S --needed ${missing[*]}"
    exit 1
  fi
fi

download "https://archives.boost.io/release/1.90.0/source/boost_1_90_0.zip" "$DOWNLOADS/boost_1_90_0.zip"
extract_zip "$DOWNLOADS/boost_1_90_0.zip" "$SOURCES/boost_1_90_0"

download "https://github.com/nlohmann/json/archive/refs/tags/v3.12.0.zip" "$DOWNLOADS/nlohmann-json-3.12.0.zip"
extract_zip "$DOWNLOADS/nlohmann-json-3.12.0.zip" "$SOURCES/json-3.12.0"

download "https://github.com/fmtlib/fmt/archive/refs/tags/12.1.0.zip" "$DOWNLOADS/fmt-12.1.0.zip"
extract_zip "$DOWNLOADS/fmt-12.1.0.zip" "$SOURCES/fmt-12.1.0"

download "https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip" "$DOWNLOADS/googletest-1.17.0.zip"
extract_zip "$DOWNLOADS/googletest-1.17.0.zip" "$SOURCES/googletest-1.17.0"

download "https://www.codesynthesis.com/download/odb/2.4/odb-2.4.0-x86_64-linux-gnu.tar.bz2" "$DOWNLOADS/odb-2.4.0-x86_64-linux-gnu.tar.bz2"
extract_tar_bz2 "$DOWNLOADS/odb-2.4.0-x86_64-linux-gnu.tar.bz2" "$SOURCES/odb-2.4.0-x86_64-linux-gnu"

download "https://www.codesynthesis.com/download/odb/2.4/libodb-2.4.0.tar.bz2" "$DOWNLOADS/libodb-2.4.0.tar.bz2"
extract_tar_bz2 "$DOWNLOADS/libodb-2.4.0.tar.bz2" "$SOURCES/libodb-2.4.0"

download "https://www.codesynthesis.com/download/odb/2.4/libodb-pgsql-2.4.0.tar.bz2" "$DOWNLOADS/libodb-pgsql-2.4.0.tar.bz2"
extract_tar_bz2 "$DOWNLOADS/libodb-pgsql-2.4.0.tar.bz2" "$SOURCES/libodb-pgsql-2.4.0"

rm -rf "$DEPS/boost" "$DEPS/nlohmann_json" "$DEPS/odb" "$DEPS/postgresql"
mkdir -p "$DEPS/boost/include" "$DEPS/nlohmann_json/include" "$DEPS/odb/bin" "$DEPS/postgresql/include" "$DEPS/postgresql/lib"

cp -R "$SOURCES/boost_1_90_0/boost" "$DEPS/boost/include/"
cp -R "$SOURCES/json-3.12.0/include/nlohmann" "$DEPS/nlohmann_json/include/"
cp "$SOURCES/odb-2.4.0-x86_64-linux-gnu/bin/odb" "$DEPS/odb/bin/odb"
chmod +x "$DEPS/odb/bin/odb"

if [[ -f /usr/include/postgresql/libpq-fe.h ]]; then
  cp -R /usr/include/postgresql/* "$DEPS/postgresql/include/"
elif [[ -f /usr/include/libpq-fe.h ]]; then
  cp /usr/include/libpq-fe.h "$DEPS/postgresql/include/"
else
  echo "Cannot find libpq-fe.h. Install postgresql-libs/postgresql development headers." >&2
  exit 1
fi

libpq="$(ldconfig -p 2>/dev/null | awk '/libpq\.so/{print $NF; exit}')"
if [[ -z "${libpq:-}" && -f /usr/lib/libpq.so ]]; then
  libpq="/usr/lib/libpq.so"
fi

if [[ -z "${libpq:-}" ]]; then
  echo "Cannot find libpq.so. Install postgresql-libs." >&2
  exit 1
fi

ln -sf "$libpq" "$DEPS/postgresql/lib/libpq.so"

echo
echo "Dependencies are ready in $DEPS"
echo "Configure example:"
echo "  cmake -S server -B server/build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug"
