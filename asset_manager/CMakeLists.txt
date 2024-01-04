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

FetchContent_Declare(
        brotl
        GIT_REPOSITORY https://github.com/google/brotli.git
        GIT_TAG master
)

set(BROTLI_DISABLE_TESTS ON CACHE BOOL "" FORCE)
set(BROTLI_BUILD_TOOLS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(brotl)

target_link_libraries(DMAssetManager PRIVATE brotlicommon) 

if(DMASSET_ENABLE_ENCODER)
        target_link_libraries(DMAssetManager PRIVATE brotlidec) 
endif()

if(DMASSET_ENABLE_DECODER)
        target_link_libraries(DMAssetManager PRIVATE brotlienc)   
endif()
