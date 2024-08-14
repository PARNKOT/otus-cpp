cmake_minimum_required(VERSION 3.10)
project(tests)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
add_executable(test_generator test_generator.cpp)

set_target_properties(test_generator PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

set_target_properties(test_generator PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIR}"
)

target_include_directories(test_generator PUBLIC ${CMAKE_SOURCE_DIR}/..)

target_link_libraries(test_generator ${Boost_LIBRARIES})
