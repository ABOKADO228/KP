cmake_minimum_required(VERSION 3.25)

project(farm_association_server VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(FARM_SERVER_WITH_ODB "Build with ODB PostgreSQL support" ON)
option(FARM_SERVER_ODB_STATIC "Link ODB runtime as static libraries" ON)
option(FARM_SERVER_BUILD_LOCAL_ODB_DEPS
    "Build local libodb/libodb-pgsql archives into server/third_party when they are missing" ON)
option(FARM_SERVER_BUILD_TESTS "Build unit and integration tests" ON)

set(FARM_SERVER_DEPS_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party"
    CACHE PATH "Directory with local project dependencies")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(FarmLocalDependencies)

farm_detect_platform(FARM_SERVER_PLATFORM)
message(STATUS "Farm server target platform: ${FARM_SERVER_PLATFORM}")
message(STATUS "Farm server local dependencies: ${FARM_SERVER_DEPS_DIR}")

find_package(Threads REQUIRED)

farm_add_boost_headers(farm_boost "${FARM_SERVER_DEPS_DIR}")
farm_add_nlohmann_json(farm_nlohmann_json "${FARM_SERVER_DEPS_DIR}")
farm_add_fmt(farm_fmt "${FARM_SERVER_DEPS_DIR}")

file(GLOB FARM_SERVER_ENTITY_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/controllers/app/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/controllers/http/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/handling/*.cpp")
list(FILTER FARM_SERVER_ENTITY_SOURCES EXCLUDE REGEX ".*/Users\\.cpp$")
list(FILTER FARM_SERVER_ENTITY_SOURCES EXCLUDE REGEX ".*/Health\\.cpp$")

if(WIN32)
    set(OPENSSL_INCLUDE_DIR "${FARM_SERVER_DEPS_DIR}/postgresql/include")
    farm_require_path("${OPENSSL_INCLUDE_DIR}/openssl/evp.h" "OpenSSL headers")
    set(OPENSSL_CRYPTO_LIBRARY "${FARM_SERVER_DEPS_DIR}/postgresql/lib/libcrypto.lib")
    farm_require_path("${OPENSSL_CRYPTO_LIBRARY}" "OpenSSL libcrypto import library")
    farm_add_imported_library(FarmOpenSSL::Crypto "${OPENSSL_CRYPTO_LIBRARY}" "${OPENSSL_INCLUDE_DIR}")
else()
    set(FARM_SERVER_LOCAL_OPENSSL_INCLUDE_DIR "${FARM_SERVER_DEPS_DIR}/openssl/include")
    if(EXISTS "${FARM_SERVER_LOCAL_OPENSSL_INCLUDE_DIR}/openssl/evp.h")
        set(OPENSSL_INCLUDE_DIR "${FARM_SERVER_LOCAL_OPENSSL_INCLUDE_DIR}")
        farm_find_local_library(OPENSSL_CRYPTO_LIBRARY "${FARM_SERVER_DEPS_DIR}" "libcrypto"
            REQUIRED
            NAMES crypto libcrypto
            PATHS
                "${FARM_SERVER_DEPS_DIR}/postgresql/lib"
                "${FARM_SERVER_DEPS_DIR}/openssl/lib")
        farm_add_imported_library(FarmOpenSSL::Crypto "${OPENSSL_CRYPTO_LIBRARY}" "${OPENSSL_INCLUDE_DIR}")
    else()
        find_package(OpenSSL REQUIRED COMPONENTS Crypto)
        add_library(FarmOpenSSL::Crypto INTERFACE IMPORTED)
        target_link_libraries(FarmOpenSSL::Crypto INTERFACE OpenSSL::Crypto)
    endif()
endif()

set(FARM_SERVER_USER_MODEL "${CMAKE_CURRENT_SOURCE_DIR}/include/persistence/User.hpp")
if(WIN32)
    find_program(FARM_SERVER_ODB_COMPILER
        NAMES odb.exe odb
        HINTS
            "${FARM_SERVER_DEPS_DIR}/odb/bin/${FARM_SERVER_PLATFORM}"
            "${FARM_SERVER_DEPS_DIR}/odb/bin"
        NO_DEFAULT_PATH)
else()
    find_program(FARM_SERVER_ODB_COMPILER
        NAMES odb
        HINTS
            "${FARM_SERVER_DEPS_DIR}/odb/bin/${FARM_SERVER_PLATFORM}"
            "${FARM_SERVER_DEPS_DIR}/odb/bin"
        NO_DEFAULT_PATH)
endif()

if(FARM_SERVER_ODB_COMPILER)
    message(STATUS "Farm server ODB compiler: ${FARM_SERVER_ODB_COMPILER}")
    set(FARM_SERVER_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
    set(FARM_SERVER_GENERATED_PERSISTENCE_DIR "${FARM_SERVER_GENERATED_DIR}/persistence")
    set(FARM_SERVER_USER_ODB_CXX "${FARM_SERVER_GENERATED_PERSISTENCE_DIR}/user-odb.cxx")
    set(FARM_SERVER_USER_ODB_HXX "${FARM_SERVER_GENERATED_PERSISTENCE_DIR}/user-odb.hxx")
    set(FARM_SERVER_USER_ODB_IXX "${FARM_SERVER_GENERATED_PERSISTENCE_DIR}/user-odb.ixx")
    set(FARM_SERVER_USER_SQL "${FARM_SERVER_GENERATED_PERSISTENCE_DIR}/user.sql")

    add_custom_command(
        OUTPUT
            "${FARM_SERVER_USER_ODB_CXX}"
            "${FARM_SERVER_USER_ODB_HXX}"
            "${FARM_SERVER_USER_ODB_IXX}"
            "${FARM_SERVER_USER_SQL}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${FARM_SERVER_GENERATED_PERSISTENCE_DIR}"
        COMMAND "${FARM_SERVER_ODB_COMPILER}"
            --std c++14
            --database pgsql
            --generate-query
            --generate-schema
            --output-dir "${FARM_SERVER_GENERATED_PERSISTENCE_DIR}"
            --input-name user
            "${FARM_SERVER_USER_MODEL}"
        DEPENDS
            "${FARM_SERVER_USER_MODEL}"
            "${FARM_SERVER_ODB_COMPILER}"
        COMMENT "Generating ODB mapping for persistence/User.hpp"
        VERBATIM)

    set_source_files_properties(
        "${FARM_SERVER_USER_ODB_CXX}"
        "${FARM_SERVER_USER_ODB_HXX}"
        "${FARM_SERVER_USER_ODB_IXX}"
        "${FARM_SERVER_USER_SQL}"
        PROPERTIES GENERATED TRUE)
else()
    message(STATUS "Farm server ODB compiler not found; using checked-in generated persistence files")
    set(FARM_SERVER_GENERATED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/generated")
    set(FARM_SERVER_GENERATED_PERSISTENCE_DIR "${FARM_SERVER_GENERATED_DIR}/persistence")
    set(FARM_SERVER_USER_ODB_CXX "${FARM_SERVER_GENERATED_PERSISTENCE_DIR}/user-odb.cxx")
    set(FARM_SERVER_USER_ODB_HXX "${FARM_SERVER_GENERATED_PERSISTENCE_DIR}/user-odb.hxx")
    set(FARM_SERVER_USER_ODB_IXX "${FARM_SERVER_GENERATED_PERSISTENCE_DIR}/user-odb.ixx")
    set(FARM_SERVER_USER_SQL "${FARM_SERVER_GENERATED_PERSISTENCE_DIR}/user.sql")

    farm_require_path("${FARM_SERVER_USER_ODB_CXX}" "checked-in ODB user source")
    farm_require_path("${FARM_SERVER_USER_ODB_HXX}" "checked-in ODB user header")
    farm_require_path("${FARM_SERVER_USER_ODB_IXX}" "checked-in ODB user inline header")
    farm_require_path("${FARM_SERVER_USER_SQL}" "checked-in ODB user SQL schema")
endif()

set(FARM_SERVER_CORE_SOURCES
        src/controllers/app/Users.cpp
        src/controllers/http/Users.cpp
        src/database/Bootstrap.cpp
        src/database/Database.cpp
        src/handling/Health.cpp
        src/handling/Users.cpp
        ${FARM_SERVER_ENTITY_SOURCES}
        src/security/Encoding.cpp
        src/security/JwtService.cpp
        src/security/PasswordHasher.cpp
        src/server/core/AppRouter.cpp
        src/server/core/RequestDispatcher.cpp
        src/server/core/Server.cpp
        "${FARM_SERVER_USER_ODB_CXX}")

add_library(farm_association_server_core STATIC)

target_sources(farm_association_server_core
    PRIVATE
        ${FARM_SERVER_CORE_SOURCES})

target_include_directories(farm_association_server_core
    PUBLIC
        "${FARM_SERVER_GENERATED_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/persistence"
        "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(farm_association_server_core
    PUBLIC
        Threads::Threads
        farm_boost
        farm_nlohmann_json
        farm_fmt
        FarmOpenSSL::Crypto)

target_compile_definitions(farm_association_server_core
    PUBLIC
        FARM_SERVER_BUILD_CONFIG="$<CONFIG>"
        FARM_SERVER_DEFAULT_DB_NAME="$<IF:$<CONFIG:Debug>,fasc_test,fasc_db>"
        FARM_SERVER_RESET_DATABASE_ON_START=$<IF:$<CONFIG:Debug>,1,0>
        FARM_SERVER_DOMAIN_SQL_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../database/dump-mydb-sprinthost-public.sql"
        FARM_SERVER_USER_SQL_PATH="${FARM_SERVER_USER_SQL}")

if(WIN32)
    target_compile_definitions(farm_association_server_core PUBLIC _WIN32_WINNT=0x0601)
    target_link_libraries(farm_association_server_core PUBLIC ws2_32 mswsock)
endif()

if(FARM_SERVER_WITH_ODB)
    if(FARM_SERVER_BUILD_LOCAL_ODB_DEPS)
        farm_add_odb_local_build(farm_odb_local_dependencies "${FARM_SERVER_DEPS_DIR}")

        set(ODB_LIBRARY "${farm_odb_local_dependencies_ODB_LIBRARY}")
        set(ODB_PGSQL_LIBRARY "${farm_odb_local_dependencies_ODB_PGSQL_LIBRARY}")
        set(ODB_INCLUDE_DIR "${farm_odb_local_dependencies_ODB_INCLUDE_DIR}")
        set(ODB_PGSQL_INCLUDE_DIR "${farm_odb_local_dependencies_ODB_PGSQL_INCLUDE_DIR}")
        set(POSTGRESQL_INCLUDE_DIR "${farm_odb_local_dependencies_POSTGRESQL_INCLUDE_DIR}")
    else()
        set(ODB_INCLUDE_DIR "${FARM_SERVER_DEPS_DIR}/libodb/include")
        set(ODB_PGSQL_INCLUDE_DIR "${FARM_SERVER_DEPS_DIR}/libodb-pgsql/include")
        set(POSTGRESQL_INCLUDE_DIR "${FARM_SERVER_DEPS_DIR}/postgresql/include")

        farm_require_path("${ODB_INCLUDE_DIR}/odb/database.hxx" "ODB runtime headers")
        farm_require_path("${ODB_PGSQL_INCLUDE_DIR}/odb/pgsql/database.hxx" "ODB PostgreSQL headers")
        farm_require_path("${POSTGRESQL_INCLUDE_DIR}/libpq-fe.h" "PostgreSQL libpq headers")

        farm_find_local_library(ODB_LIBRARY "${FARM_SERVER_DEPS_DIR}" "libodb"
            REQUIRED
            NAMES odb libodb odb-2.4 odb.lib libodb.lib
            PATHS
                "${FARM_SERVER_DEPS_DIR}/libodb/lib"
                "${FARM_SERVER_DEPS_DIR}/odb/lib")

        farm_find_local_library(ODB_PGSQL_LIBRARY "${FARM_SERVER_DEPS_DIR}" "libodb-pgsql"
            REQUIRED
            NAMES odb-pgsql libodb-pgsql odb-pgsql-2.4 odb-pgsql.lib libodb-pgsql.lib
            PATHS
                "${FARM_SERVER_DEPS_DIR}/libodb-pgsql/lib")
    endif()

    if(WIN32 AND EXISTS "${FARM_SERVER_DEPS_DIR}/postgresql/lib/libpq.lib")
        set(POSTGRESQL_LIBRARY "${FARM_SERVER_DEPS_DIR}/postgresql/lib/libpq.lib")
        farm_add_imported_library(PostgreSQL::pq "${POSTGRESQL_LIBRARY}" "${POSTGRESQL_INCLUDE_DIR}")
    elseif(NOT WIN32)
        farm_find_local_library(POSTGRESQL_LIBRARY "${FARM_SERVER_DEPS_DIR}" "libpq"
            NAMES pq libpq pq.lib libpq.lib
            PATHS
                "${FARM_SERVER_DEPS_DIR}/postgresql/lib"
                "${FARM_SERVER_DEPS_DIR}/libpq/lib")

        if(POSTGRESQL_LIBRARY)
            set(POSTGRESQL_INCLUDE_DIR "${FARM_SERVER_DEPS_DIR}/postgresql/include")
            farm_require_path("${POSTGRESQL_INCLUDE_DIR}/libpq-fe.h" "PostgreSQL libpq headers")
            farm_add_imported_library(PostgreSQL::pq "${POSTGRESQL_LIBRARY}" "${POSTGRESQL_INCLUDE_DIR}")
        else()
            find_package(PostgreSQL REQUIRED)
            add_library(PostgreSQL::pq INTERFACE IMPORTED)
            target_link_libraries(PostgreSQL::pq INTERFACE PostgreSQL::PostgreSQL)
        endif()
    else()
        farm_find_local_library(POSTGRESQL_LIBRARY "${FARM_SERVER_DEPS_DIR}" "libpq"
            REQUIRED
            NAMES pq libpq pq.lib libpq.lib
            PATHS
                "${FARM_SERVER_DEPS_DIR}/postgresql/lib"
                "${FARM_SERVER_DEPS_DIR}/libpq/lib")
        farm_add_imported_library(PostgreSQL::pq "${POSTGRESQL_LIBRARY}" "${POSTGRESQL_INCLUDE_DIR}")
    endif()

    farm_add_imported_library(ODB::odb "${ODB_LIBRARY}" "${ODB_INCLUDE_DIR}")
    farm_add_imported_library(ODB::pgsql "${ODB_PGSQL_LIBRARY}" "${ODB_PGSQL_INCLUDE_DIR}")

    if(TARGET farm_odb_local_dependencies)
        add_dependencies(farm_association_server_core farm_odb_local_dependencies)
    endif()

    target_compile_definitions(farm_association_server_core PUBLIC FARM_SERVER_WITH_ODB=1)

    if(FARM_SERVER_ODB_STATIC)
        target_compile_definitions(farm_association_server_core
            PUBLIC
                LIBODB_STATIC_LIB
                LIBODB_PGSQL_STATIC_LIB
                _HAS_AUTO_PTR_ETC=1
                _CRT_SECURE_NO_WARNINGS)
    endif()

    target_link_libraries(farm_association_server_core PUBLIC ODB::odb ODB::pgsql PostgreSQL::pq)

    farm_add_runtime_paths(farm_association_server_core
        "${FARM_SERVER_DEPS_DIR}/odb/bin"
        "${FARM_SERVER_DEPS_DIR}/libodb/bin"
        "${FARM_SERVER_DEPS_DIR}/libodb-pgsql/bin"
        "${FARM_SERVER_DEPS_DIR}/postgresql/bin"
        "${FARM_SERVER_DEPS_DIR}/postgresql/lib")

    if(WIN32)
        file(GLOB FARM_SERVER_POSTGRESQL_RUNTIME_DLLS
            CONFIGURE_DEPENDS
            "${FARM_SERVER_DEPS_DIR}/postgresql/bin/*.dll")
    endif()
endif()

add_executable(farm_association_server)

target_sources(farm_association_server
    PRIVATE
        src/Main.cpp)

target_link_libraries(farm_association_server
    PRIVATE
        farm_association_server_core)

add_custom_command(TARGET farm_association_server POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory
        "$<TARGET_FILE_DIR:farm_association_server>/sql"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/../database/dump-mydb-sprinthost-public.sql"
        "$<TARGET_FILE_DIR:farm_association_server>/sql/farm_schema.sql"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${FARM_SERVER_USER_SQL}"
        "$<TARGET_FILE_DIR:farm_association_server>/sql/user.sql"
    COMMENT "Copying SQL bootstrap scripts for farm_association_server"
    VERBATIM)

if(FARM_SERVER_WITH_ODB)
    if(WIN32 AND FARM_SERVER_POSTGRESQL_RUNTIME_DLLS)
        add_custom_command(TARGET farm_association_server POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                ${FARM_SERVER_POSTGRESQL_RUNTIME_DLLS}
                "$<TARGET_FILE_DIR:farm_association_server>"
            COMMENT "Copying PostgreSQL runtime DLLs for farm_association_server"
            VERBATIM)
    endif()
endif()

enable_testing()
add_test(NAME farm_association_server_version
    COMMAND farm_association_server --version)

if(FARM_SERVER_BUILD_TESTS)
    add_subdirectory(tests)
endif()
