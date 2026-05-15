farm_add_googletest("${FARM_SERVER_DEPS_DIR}")

add_executable(farm_association_unit_tests)

target_sources(farm_association_unit_tests
    PRIVATE
        unit/controller/AppRouterTests.cpp
        unit/controller/FarmEntityControllerTests.cpp
        unit/controller/FarmEntityHandlerTests.cpp
        unit/controller/HealthHandlerTests.cpp
        unit/controller/UserControllerTests.cpp
        unit/database/BootstrapTests.cpp
        unit/marshalling/DomainTypesTests.cpp
        unit/marshalling/UserMarshallingTests.cpp
        unit/security/JwtServiceTests.cpp
        unit/security/PasswordHasherTests.cpp)

target_include_directories(farm_association_unit_tests
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(farm_association_unit_tests
    PRIVATE
        farm_association_server_core
        GTest::gtest_main)

add_executable(farm_association_integration_tests)

target_sources(farm_association_integration_tests
    PRIVATE
        integration/business/UsersBusinessProcessTests.cpp
        integration/database/DatabaseBackedEndpointTests.cpp
        integration/endpoints/FarmEntityEndpointTests.cpp
        integration/endpoints/HealthEndpointTests.cpp
        integration/endpoints/RequestDispatcherTests.cpp)

target_include_directories(farm_association_integration_tests
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(farm_association_integration_tests
    PRIVATE
        farm_association_server_core
        GTest::gtest_main)

if(WIN32 AND FARM_SERVER_WITH_ODB AND FARM_SERVER_POSTGRESQL_RUNTIME_DLLS)
    foreach(test_target IN ITEMS farm_association_unit_tests farm_association_integration_tests)
        add_custom_command(TARGET ${test_target} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                ${FARM_SERVER_POSTGRESQL_RUNTIME_DLLS}
                "$<TARGET_FILE_DIR:${test_target}>"
            COMMENT "Copying PostgreSQL runtime DLLs for ${test_target}"
            VERBATIM)
    endforeach()
endif()

include(GoogleTest)
gtest_discover_tests(farm_association_unit_tests
    TEST_PREFIX unit.)
gtest_discover_tests(farm_association_integration_tests
    TEST_PREFIX integration.
    PROPERTIES
        ENVIRONMENT "FARM_DB_NAME=fasc_test;FARM_DB_RESET_ON_START=1;FARM_DB_BOOTSTRAP=1"
        RESOURCE_LOCK fasc_test)
