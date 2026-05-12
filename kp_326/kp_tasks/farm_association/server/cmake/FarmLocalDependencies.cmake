include_guard(GLOBAL)

function(farm_detect_platform out_var)
    if(WIN32)
        set(platform "windows")
    elseif(APPLE)
        set(platform "macos")
    elseif(UNIX)
        set(platform "linux")
    else()
        message(FATAL_ERROR "Unsupported target platform: ${CMAKE_SYSTEM_NAME}")
    endif()

    set(${out_var} "${platform}" PARENT_SCOPE)
endfunction()

function(farm_require_path path description)
    if(NOT EXISTS "${path}")
        message(FATAL_ERROR
            "Missing local dependency path: ${path}\n"
            "Expected: ${description}\n"
            "Install the dependency inside server/third_party; system packages are intentionally not used.")
    endif()
endfunction()

function(farm_add_boost_headers target deps_dir)
    set(include_dir "${deps_dir}/boost/include")
    farm_require_path("${include_dir}/boost/version.hpp" "full Boost headers")

    add_library(${target} INTERFACE)
    target_include_directories(${target} INTERFACE "${include_dir}")
    target_compile_definitions(${target} INTERFACE
        BOOST_ERROR_CODE_HEADER_ONLY
        BOOST_SYSTEM_NO_LIB)

    if(EXISTS "${deps_dir}/boost/lib")
        target_link_directories(${target} INTERFACE "${deps_dir}/boost/lib")
    endif()
endfunction()

function(farm_add_nlohmann_json target deps_dir)
    set(include_dir "${deps_dir}/nlohmann_json/include")
    farm_require_path("${include_dir}/nlohmann/json.hpp" "nlohmann/json headers")

    add_library(${target} INTERFACE)
    target_include_directories(${target} INTERFACE "${include_dir}")
endfunction()

function(farm_find_local_program out_var deps_dir program_name)
    farm_detect_platform(platform)

    find_program(found_program
        NAMES "${program_name}" "${program_name}.exe"
        HINTS
            "${deps_dir}/${program_name}/bin/${platform}"
            "${deps_dir}/${program_name}/bin"
        NO_DEFAULT_PATH)

    if(NOT found_program)
        message(FATAL_ERROR
            "Missing local program '${program_name}'.\n"
            "Expected it under ${deps_dir}/${program_name}/bin or ${deps_dir}/${program_name}/bin/${platform}.")
    endif()

    set(${out_var} "${found_program}" PARENT_SCOPE)
endfunction()

function(farm_add_odb_local_build target deps_dir)
    farm_detect_platform(platform)

    set(odb_source_dir "${deps_dir}/_sources/libodb-2.4.0")
    set(odb_pgsql_source_dir "${deps_dir}/_sources/libodb-pgsql-2.4.0")
    set(postgresql_dir "${deps_dir}/postgresql")
    set(local_build_project "${CMAKE_CURRENT_SOURCE_DIR}/cmake/odb-local")

    farm_require_path("${local_build_project}/CMakeLists.txt" "local ODB dependency build project")
    farm_require_path("${odb_source_dir}/odb/database.hxx" "libodb sources")
    farm_require_path("${odb_pgsql_source_dir}/odb/pgsql/database.hxx" "libodb-pgsql sources")
    farm_require_path("${postgresql_dir}/include/libpq-fe.h" "PostgreSQL libpq headers")

    file(MAKE_DIRECTORY
        "${deps_dir}/libodb/include"
        "${deps_dir}/libodb/lib"
        "${deps_dir}/libodb-pgsql/include"
        "${deps_dir}/libodb-pgsql/lib")

    set(build_dir
        "${deps_dir}/_build/odb-local/${platform}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_SYSTEM_PROCESSOR}")
    set(stamp_file "${build_dir}/farm-odb-local.stamp")

    set(odb_library
        "${deps_dir}/libodb/lib/${CMAKE_STATIC_LIBRARY_PREFIX}odb${CMAKE_STATIC_LIBRARY_SUFFIX}")
    set(odb_pgsql_library
        "${deps_dir}/libodb-pgsql/lib/${CMAKE_STATIC_LIBRARY_PREFIX}odb-pgsql${CMAKE_STATIC_LIBRARY_SUFFIX}")

    set(configure_args
        -S "${local_build_project}"
        -B "${build_dir}"
        -G "${CMAKE_GENERATOR}"
        "-DTHIRD_PARTY_DIR=${deps_dir}"
        "-DCMAKE_CXX_STANDARD=14")

    if(CMAKE_CXX_COMPILER)
        list(APPEND configure_args "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
    endif()

    if(CMAKE_GENERATOR_PLATFORM)
        list(APPEND configure_args -A "${CMAKE_GENERATOR_PLATFORM}")
    endif()

    if(CMAKE_GENERATOR_TOOLSET)
        list(APPEND configure_args -T "${CMAKE_GENERATOR_TOOLSET}")
    endif()

    if(NOT CMAKE_CONFIGURATION_TYPES AND CMAKE_BUILD_TYPE)
        list(APPEND configure_args "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
    endif()

    set(build_args --build "${build_dir}")
    if(CMAKE_CONFIGURATION_TYPES)
        list(APPEND build_args --config "$<CONFIG>")
    endif()

    add_custom_command(
        OUTPUT
            "${stamp_file}"
        COMMAND "${CMAKE_COMMAND}" ${configure_args}
        COMMAND "${CMAKE_COMMAND}" ${build_args}
        COMMAND "${CMAKE_COMMAND}" -E touch "${stamp_file}"
        DEPENDS
            "${local_build_project}/CMakeLists.txt"
        BYPRODUCTS
            "${odb_library}"
            "${odb_pgsql_library}"
        COMMENT "Building local ODB runtime dependencies once"
        VERBATIM)

    add_custom_target(${target} DEPENDS "${stamp_file}")

    set(${target}_ODB_LIBRARY "${odb_library}" PARENT_SCOPE)
    set(${target}_ODB_PGSQL_LIBRARY "${odb_pgsql_library}" PARENT_SCOPE)
    set(${target}_ODB_INCLUDE_DIR "${deps_dir}/libodb/include" PARENT_SCOPE)
    set(${target}_ODB_PGSQL_INCLUDE_DIR "${deps_dir}/libodb-pgsql/include" PARENT_SCOPE)
    set(${target}_POSTGRESQL_INCLUDE_DIR "${postgresql_dir}/include" PARENT_SCOPE)
endfunction()

function(farm_add_runtime_paths target)
    set(paths ${ARGN})

    if(APPLE)
        set_target_properties(${target} PROPERTIES
            BUILD_RPATH "${paths}"
            INSTALL_RPATH "@loader_path;${paths}")
    elseif(UNIX)
        set_target_properties(${target} PROPERTIES
            BUILD_RPATH "${paths}"
            INSTALL_RPATH "$ORIGIN;${paths}")
    endif()
endfunction()

function(farm_add_fmt target deps_dir)
set(source_dir "${deps_dir}/_sources/fmt-12.1.0" )
set(build_dir "${deps_dir}/_build/fmt")

farm_require_path("${source_dir}/CMakeLists.txt" "fmt sources")    
farm_require_path("${source_dir}/include/fmt/core.h" "fmt headers")

set(FMT_DOC OFF CACHE BOOL "" FORCE)
set(FMT_INSTALL OFF CACHE BOOL "" FORCE)
set(FMT_TEST OFF CACHE BOOL "" FORCE)

if(NOT TARGET fmt::fmt)
        add_subdirectory("${source_dir}" "${build_dir}" EXCLUDE_FROM_ALL)
endif()

add_library(${target} INTERFACE)
target_link_libraries(${target} INTERFACE fmt::fmt)
endfunction()

function(farm_add_googletest deps_dir)
    set(source_dir "${deps_dir}/_sources/googletest-1.17.0")
    set(build_dir "${deps_dir}/_build/googletest")

    farm_require_path("${source_dir}/CMakeLists.txt" "GoogleTest sources")
    farm_require_path("${source_dir}/googletest/include/gtest/gtest.h" "GoogleTest headers")

    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    if(NOT TARGET GTest::gtest_main)
        add_subdirectory("${source_dir}" "${build_dir}" EXCLUDE_FROM_ALL)
    endif()
endfunction()

function(farm_find_local_library out_var deps_dir package_name)
    set(options REQUIRED)
    set(one_value_args)
    set(multi_value_args NAMES PATHS)
    cmake_parse_arguments(arg "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

    farm_detect_platform(platform)

    set(search_paths "")
    foreach(path IN LISTS arg_PATHS)
        list(APPEND search_paths
            "${path}/${platform}"
            "${path}/${CMAKE_SYSTEM_PROCESSOR}"
            "${path}")
    endforeach()

    find_library(found_library
        NAMES ${arg_NAMES}
        HINTS ${search_paths}
        NO_DEFAULT_PATH)

    if(arg_REQUIRED AND NOT found_library)
        message(FATAL_ERROR
            "Missing local library '${package_name}'.\n"
            "Searched names: ${arg_NAMES}\n"
            "Searched paths: ${search_paths}")
    endif()

    set(${out_var} "${found_library}" PARENT_SCOPE)
endfunction()

function(farm_add_imported_library target library_path include_dirs)
    add_library(${target} UNKNOWN IMPORTED)
    set_target_properties(${target} PROPERTIES
        IMPORTED_LOCATION "${library_path}"
        INTERFACE_INCLUDE_DIRECTORIES "${include_dirs}")
endfunction()
