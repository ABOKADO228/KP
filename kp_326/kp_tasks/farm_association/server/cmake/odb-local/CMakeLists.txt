cmake_minimum_required(VERSION 3.25)

project(odb_local_build LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

set(THIRD_PARTY_DIR
    "${CMAKE_CURRENT_LIST_DIR}/../../third_party"
    CACHE PATH "Farm server third_party directory")

set(LIBODB_SOURCE_DIR "${THIRD_PARTY_DIR}/_sources/libodb-2.4.0")
set(LIBODB_PGSQL_SOURCE_DIR "${THIRD_PARTY_DIR}/_sources/libodb-pgsql-2.4.0")
set(POSTGRESQL_DIR "${THIRD_PARTY_DIR}/postgresql")
set(POSTGRESQL_INCLUDE_DIR "" CACHE PATH "PostgreSQL libpq include directory")
set(GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(LIBODB_GENERATED_CONFIG_H "${GENERATED_INCLUDE_DIR}/odb/details/config.h")
set(LIBODB_PGSQL_GENERATED_CONFIG_H "${GENERATED_INCLUDE_DIR}/odb/pgsql/details/config.h")
set(LIBODB_PGSQL_GENERATED_PUBLIC_CONFIG_H "${GENERATED_INCLUDE_DIR}/odb/pgsql/config.h")
set(LIBODB_INSTALL_DIR
    "${THIRD_PARTY_DIR}/libodb"
    CACHE PATH "Local libodb install directory")
set(LIBODB_PGSQL_INSTALL_DIR
    "${THIRD_PARTY_DIR}/libodb-pgsql"
    CACHE PATH "Local libodb-pgsql install directory")

if(NOT POSTGRESQL_INCLUDE_DIR)
    if(WIN32)
        set(POSTGRESQL_INCLUDE_DIR "${POSTGRESQL_DIR}/include")
    else()
        find_package(PostgreSQL REQUIRED)
        list(GET PostgreSQL_INCLUDE_DIRS 0 POSTGRESQL_INCLUDE_DIR)
    endif()
endif()

if(NOT EXISTS "${POSTGRESQL_INCLUDE_DIR}/libpq-fe.h")
    message(FATAL_ERROR "Missing PostgreSQL libpq headers: ${POSTGRESQL_INCLUDE_DIR}/libpq-fe.h")
endif()

file(MAKE_DIRECTORY
    "${GENERATED_INCLUDE_DIR}/odb/details"
    "${GENERATED_INCLUDE_DIR}/odb/pgsql/details"
    "${LIBODB_INSTALL_DIR}/include"
    "${LIBODB_INSTALL_DIR}/lib"
    "${LIBODB_PGSQL_INSTALL_DIR}/include"
    "${LIBODB_PGSQL_INSTALL_DIR}/lib")

if(WIN32)
    file(WRITE "${LIBODB_GENERATED_CONFIG_H}"
"#ifndef ODB_DETAILS_CONFIG_H
#define ODB_DETAILS_CONFIG_H
#define ODB_THREADS_WIN32
#define ODB_THREADS_TLS_DECLSPEC
#define LIBODB_STATIC_LIB
#endif
")
else()
    file(WRITE "${LIBODB_GENERATED_CONFIG_H}"
"#ifndef ODB_DETAILS_CONFIG_H
#define ODB_DETAILS_CONFIG_H
#define ODB_THREADS_POSIX
#define ODB_THREADS_TLS_KEYWORD
#define LIBODB_STATIC_LIB
#endif
")
endif()

file(WRITE "${LIBODB_PGSQL_GENERATED_CONFIG_H}"
"#ifndef ODB_PGSQL_DETAILS_CONFIG_H
#define ODB_PGSQL_DETAILS_CONFIG_H
#define LIBODB_PGSQL_STATIC_LIB
#endif
")

file(WRITE "${LIBODB_PGSQL_GENERATED_PUBLIC_CONFIG_H}"
"#ifndef ODB_PGSQL_CONFIG_H
#define ODB_PGSQL_CONFIG_H
#define LIBODB_PGSQL_STATIC_LIB
#endif
")

add_custom_target(odb-local-headers
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${LIBODB_SOURCE_DIR}/odb"
        "${LIBODB_INSTALL_DIR}/include/odb"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${LIBODB_PGSQL_SOURCE_DIR}/odb"
        "${LIBODB_PGSQL_INSTALL_DIR}/include/odb"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${LIBODB_GENERATED_CONFIG_H}"
        "${LIBODB_INSTALL_DIR}/include/odb/details/config.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${LIBODB_PGSQL_GENERATED_CONFIG_H}"
        "${LIBODB_PGSQL_INSTALL_DIR}/include/odb/pgsql/details/config.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${LIBODB_PGSQL_GENERATED_PUBLIC_CONFIG_H}"
        "${LIBODB_PGSQL_INSTALL_DIR}/include/odb/pgsql/config.h"
    COMMENT "Copying local ODB headers"
    VERBATIM)

set(LIBODB_SOURCES
    "${LIBODB_SOURCE_DIR}/odb/callback.cxx"
    "${LIBODB_SOURCE_DIR}/odb/connection.cxx"
    "${LIBODB_SOURCE_DIR}/odb/database.cxx"
    "${LIBODB_SOURCE_DIR}/odb/exceptions.cxx"
    "${LIBODB_SOURCE_DIR}/odb/lazy-ptr-impl.cxx"
    "${LIBODB_SOURCE_DIR}/odb/prepared-query.cxx"
    "${LIBODB_SOURCE_DIR}/odb/query-dynamic.cxx"
    "${LIBODB_SOURCE_DIR}/odb/result.cxx"
    "${LIBODB_SOURCE_DIR}/odb/schema-catalog.cxx"
    "${LIBODB_SOURCE_DIR}/odb/section.cxx"
    "${LIBODB_SOURCE_DIR}/odb/session.cxx"
    "${LIBODB_SOURCE_DIR}/odb/statement-processing.cxx"
    "${LIBODB_SOURCE_DIR}/odb/statement.cxx"
    "${LIBODB_SOURCE_DIR}/odb/tracer.cxx"
    "${LIBODB_SOURCE_DIR}/odb/transaction.cxx"
    "${LIBODB_SOURCE_DIR}/odb/vector-impl.cxx"
    "${LIBODB_SOURCE_DIR}/odb/details/buffer.cxx"
    "${LIBODB_SOURCE_DIR}/odb/details/condition.cxx"
    "${LIBODB_SOURCE_DIR}/odb/details/lock.cxx"
    "${LIBODB_SOURCE_DIR}/odb/details/mutex.cxx"
    "${LIBODB_SOURCE_DIR}/odb/details/shared-ptr/base.cxx")

if(WIN32)
    list(APPEND LIBODB_SOURCES
        "${LIBODB_SOURCE_DIR}/odb/details/win32/condition.cxx"
        "${LIBODB_SOURCE_DIR}/odb/details/win32/dll.cxx"
        "${LIBODB_SOURCE_DIR}/odb/details/win32/exceptions.cxx"
        "${LIBODB_SOURCE_DIR}/odb/details/win32/init.cxx"
        "${LIBODB_SOURCE_DIR}/odb/details/win32/once.cxx"
        "${LIBODB_SOURCE_DIR}/odb/details/win32/thread.cxx"
        "${LIBODB_SOURCE_DIR}/odb/details/win32/tls.cxx")
elseif(UNIX)
    list(APPEND LIBODB_SOURCES
        "${LIBODB_SOURCE_DIR}/odb/details/posix/exceptions.cxx"
        "${LIBODB_SOURCE_DIR}/odb/details/posix/thread.cxx")
endif()

add_library(odb STATIC ${LIBODB_SOURCES})
add_dependencies(odb odb-local-headers)
target_compile_definitions(odb PUBLIC LIBODB_STATIC_LIB)
target_include_directories(odb PUBLIC "${GENERATED_INCLUDE_DIR}" "${LIBODB_SOURCE_DIR}")
target_link_libraries(odb PUBLIC Threads::Threads)

set_target_properties(odb PROPERTIES
    OUTPUT_NAME odb
    ARCHIVE_OUTPUT_DIRECTORY "${LIBODB_INSTALL_DIR}/lib")

foreach(config DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set_target_properties(odb PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_${config} "${LIBODB_INSTALL_DIR}/lib")
endforeach()

set(LIBODB_PGSQL_SOURCES
    "${LIBODB_PGSQL_SOURCE_DIR}/odb/pgsql/auto-handle.cxx"
    "${LIBODB_PGSQL_SOURCE_DIR}/odb/pgsql/connection-factory.cxx"
    "${LIBODB_PGSQL_SOURCE_DIR}/odb/pgsql/connection.cxx"
    "${LIBODB_PGSQL_SOURCE_DIR}/odb/pgsql/database.cxx"
    "${LIBODB_PGSQL_SOURCE_DIR}/odb/pgsql/error.cxx"
    "${LIBODB_PGSQL_SOURCE_DIR}/odb/pgsql/exceptions.cxx"
    "${LIBODB_PGSQL_SOURCE_DIR}/odb/pgsql/prepared-query.cxx"
    "${LIBODB_PGSQL_SOURCE_DIR}/odb/pgsql/query-const-expr.cxx"
    "${LIBODB_PGSQL_SOURCE_DIR}/odb/pgsql/query-dynamic.cxx"
    "${LIBODB_PGSQL_SOURCE_DIR}/odb/pgsql/query.cxx"
    "${LIBODB_PGSQL_SOURCE_DIR}/odb/pgsql/simple-object-statements.cxx"
    "${LIBODB_PGSQL_SOURCE_DIR}/odb/pgsql/statement.cxx"
    "${LIBODB_PGSQL_SOURCE_DIR}/odb/pgsql/statements-base.cxx"
    "${LIBODB_PGSQL_SOURCE_DIR}/odb/pgsql/tracer.cxx"
    "${LIBODB_PGSQL_SOURCE_DIR}/odb/pgsql/traits.cxx"
    "${LIBODB_PGSQL_SOURCE_DIR}/odb/pgsql/transaction-impl.cxx"
    "${LIBODB_PGSQL_SOURCE_DIR}/odb/pgsql/transaction.cxx"
    "${LIBODB_PGSQL_SOURCE_DIR}/odb/pgsql/details/endian-traits.cxx"
    "${LIBODB_PGSQL_SOURCE_DIR}/odb/pgsql/details/options.cxx")

add_library(odb-pgsql STATIC ${LIBODB_PGSQL_SOURCES})
add_dependencies(odb-pgsql odb-local-headers)
target_compile_definitions(odb-pgsql PUBLIC LIBODB_PGSQL_STATIC_LIB LIBODB_STATIC_LIB)
target_include_directories(odb-pgsql
    PUBLIC
        "${GENERATED_INCLUDE_DIR}"
        "${LIBODB_SOURCE_DIR}"
        "${LIBODB_PGSQL_SOURCE_DIR}"
        "${POSTGRESQL_INCLUDE_DIR}")
target_link_libraries(odb-pgsql PUBLIC odb)

set_target_properties(odb-pgsql PROPERTIES
    OUTPUT_NAME odb-pgsql
    ARCHIVE_OUTPUT_DIRECTORY "${LIBODB_PGSQL_INSTALL_DIR}/lib")

foreach(config DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set_target_properties(odb-pgsql PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_${config} "${LIBODB_PGSQL_INSTALL_DIR}/lib")
endforeach()
