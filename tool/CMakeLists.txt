file(GLOB_RECURSE packer_sources CONFIGURE_DEPENDS "./src/*.cpp")

add_executable(DMAssetTool ${packer_sources})
target_include_directories(DMAssetTool PUBLIC include/)

target_link_libraries(DMAssetTool PRIVATE 
        DMAssetManager
)
target_include_directories(DMAssetTool PRIVATE 
        DMAssetManager
)
