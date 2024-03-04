cmake_minimum_required(VERSION 3.10)
project(01_homework)

if(DEFINED ENV{GITHUB_RUN_NUMBER})
    project(01_hometask VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
else()
    project(01_hometask VERSION 0.0.0)
endif()

configure_file(version.hpp.in version.hpp)
include_directories(${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME} main.cpp)

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