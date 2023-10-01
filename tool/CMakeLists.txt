cmake_minimum_required(VERSION 3.25.0)

project(DMAssetTool
        VERSION 1.0.1
        DESCRIPTION "Dublikunt asset packer"
        LANGUAGES CXX
)

file(GLOB_RECURSE packer_headers CONFIGURE_DEPENDS "./include/*.hpp")
file(GLOB_RECURSE packer_sources CONFIGURE_DEPENDS "./src/*.cpp")

add_executable(DMAssetTool ${packer_headers} ${packer_sources})
target_include_directories(DMAssetTool PUBLIC include/)

set_target_properties(DMAssetTool PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(DMAssetTool PROPERTIES SOVERSION 1)

include(FetchContent)

FetchContent_Declare(
    loguru
	GIT_REPOSITORY https://github.com/emilk/loguru.git
	GIT_TAG master
)

set(LOGURU_WITH_STREAMS ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(loguru)

target_link_libraries(DMAssetTool PUBLIC 
    loguru::loguru
)
