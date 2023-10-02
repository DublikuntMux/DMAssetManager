cmake_minimum_required(VERSION 3.25.0)

project(DMAssetManager
        VERSION 1.0.1
        DESCRIPTION "Dublikunt asset manager"
        LANGUAGES CXX
)

option(DMASSET_ENABLE_ENCODER "Enable asset encoding" ON)
option(DMASSET_ENABLE_DECODER "Enable asset decoding" ON)

if(DMASSET_ENABLE_ENCODER)
        set(DM_PACKER_ASSET_FILE 
                include/packer.hpp
                src/packer.cpp
        )
endif()

if(DMASSET_ENABLE_DECODER)
        set(DM_MANAGER_ASSET_FILE 
                include/manager.hpp
                src/manager.cpp
        )
endif()

set(DM_COMMON_ASSET_FILE 
        include/asset.hpp
)

add_library(DMAssetManager ${DM_COMMON_ASSET_FILE} ${DM_MANAGER_ASSET_FILE} ${DM_PACKER_ASSET_FILE})
target_include_directories(DMAssetManager PUBLIC include/)

set_target_properties(DMAssetManager PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(DMAssetManager PROPERTIES SOVERSION 1)

if(DMASSET_ENABLE_ENCODER)
        find_package(unofficial-brotli CONFIG REQUIRED)
elseif(DMASSET_ENABLE_DECODER)
        find_package(unofficial-brotli CONFIG REQUIRED)
endif()

if(DMASSET_ENABLE_ENCODER)
        target_link_libraries(DMAssetManager PRIVATE unofficial::brotli::brotlidec) 
endif()

if(DMASSET_ENABLE_DECODER)
        target_link_libraries(DMAssetManager PRIVATE unofficial::brotli::brotlienc)   
endif()
