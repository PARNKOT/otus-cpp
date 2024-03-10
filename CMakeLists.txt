cmake_minimum_required(VERSION 3.10)
project(ip_filter)

if(DEFINED ENV{GITHUB_RUN_NUMBER})
    project(${PROJECT_NAME} VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
else()
    project(${PROJECT_NAME} VERSION 0.0.0)
endif()


option(WITH_BOOST_TEST "Whether to build Boost test" OFF)


configure_file(version.hpp.in version.hpp)
include_directories(${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME} ip_filter.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if(WITH_BOOST_TEST)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    add_executable(test_version tests/test_version.cpp)

    set_target_properties(test_version PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    set_target_properties(test_version PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIR}"
    )

    target_include_directories(test_version PRIVATE ${CMAKE_BINARY_DIR})

    target_link_libraries(test_version
        ${Boost_LIBRARIES}
    )
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT e.a.konyakhin@gmail.com)
include(CPack)

if(WITH_BOOST_TEST)
    enable_testing()
    add_test(test_version test_version)
endif()