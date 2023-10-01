cmake_minimum_required(VERSION 3.25.0)

project(DMAssetServer
        VERSION 1.0.1
        DESCRIPTION "Dublikunt asset server"
        LANGUAGES CXX
)

file(GLOB_RECURSE server_headers CONFIGURE_DEPENDS "./include/*.hpp")
file(GLOB_RECURSE server_sources CONFIGURE_DEPENDS "./src/*.cpp")

add_library(DMAssetServer ${server_headers} ${server_sources})
target_include_directories(DMAssetServer PUBLIC include/)

set_target_properties(DMAssetServer PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(DMAssetServer PROPERTIES SOVERSION 1)
