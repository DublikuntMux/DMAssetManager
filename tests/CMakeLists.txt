cmake_minimum_required(VERSION 3.25.0)

project(AssetTests
        VERSION 1.0.1
        DESCRIPTION "Dublikunt asset server tests"
        LANGUAGES CXX
)

file(GLOB_RECURSE tests_sources CONFIGURE_DEPENDS "./src/*.cpp")

add_executable(AssetTests ${tests_sources})

set_target_properties(AssetTests PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(AssetTests PROPERTIES SOVERSION 1)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

target_link_libraries(AssetTests PRIVATE 
        GTest::gtest_main
        DMAssetManager
)
target_include_directories(AssetTests PRIVATE 
        DMAssetManager
)

include(GoogleTest)
gtest_discover_tests(AssetTests)

add_custom_command(
        TARGET AssetTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/tests/test_data
                ${CMAKE_CURRENT_BINARY_DIR}/test_data
)
