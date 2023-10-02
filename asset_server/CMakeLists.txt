cmake_minimum_required(VERSION 3.25.0)

project(DMAssetServer
        VERSION 1.0.1
        DESCRIPTION "Dublikunt asset server"
        LANGUAGES CXX
)

option(DMASSET_ENABLE_ENCODER "Enable asset encoding" ON)
option(DMASSET_ENABLE_DECODER "Enable asset decoding" ON)

if(DMASSET_ENABLE_DECODER)
    add_compile_definitions(ENABLE_DECODER)
endif()

if(DMASSET_ENABLE_ENCODER)
    add_compile_definitions(ENABLE_ENCODER)
endif()

file(GLOB_RECURSE server_headers CONFIGURE_DEPENDS "./include/*.hpp")
file(GLOB_RECURSE server_sources CONFIGURE_DEPENDS "./src/*.cpp")

add_library(DMAssetServer ${server_headers} ${server_sources})
target_include_directories(DMAssetServer PUBLIC include/)

set_target_properties(DMAssetServer PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(DMAssetServer PROPERTIES SOVERSION 1)

if(${DMASSET_ENABLE_ENCODER})
        find_package(unofficial-brotli CONFIG REQUIRED)
elseif(${DMASSET_ENABLE_DECODER})
        find_package(unofficial-brotli CONFIG REQUIRED)
endif()

if(${DMASSET_ENABLE_ENCODER})
        target_link_libraries(DMAssetServer PRIVATE unofficial::brotli::brotlidec) 
endif()

if(${DMASSET_ENABLE_DECODER})
        target_link_libraries(DMAssetServer PRIVATE unofficial::brotli::brotlienc)   
endif()
