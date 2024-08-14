cmake_minimum_required(VERSION 3.10)
project(application)

if(DEFINED ENV{GITHUB_RUN_NUMBER})
    project(${PROJECT_NAME} VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
else()
    project(${PROJECT_NAME} VERSION 0.0.0)
endif()


option(WITH_BOOST_TEST "Whether to build Boost test" ON)

configure_file(version.hpp.in version.hpp)

#find_package(yaml-cpp REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

include(FetchContent)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
)
FetchContent_GetProperties(yaml-cpp)

if(NOT yaml-cpp_POPULATED)
  message(STATUS "Fetching yaml-cpp...")
  FetchContent_Populate(yaml-cpp)
  add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()

include_directories(${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/3rdparty)

add_executable(${PROJECT_NAME}
               src/main.cpp
               src/YamlReader.cpp
               src/Field.cpp
               src/CppGenerator.cpp)

target_link_libraries(${PROJECT_NAME} yaml-cpp::yaml-cpp fmt ${Boost_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)


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