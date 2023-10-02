cmake_minimum_required(VERSION 3.25.0)
project(DMAsset
        VERSION 1.0.1
        LANGUAGES CXX
)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

include("cmake/Cache.cmake")
include("cmake/CompilerWarnings.cmake")
include("cmake/StaticAnalyzers.cmake")
include("cmake/InterproceduralOptimization.cmake") 

option(DMASSET_ENABLE_IPO "Enable IPO/LTO" OFF)
option(DMASSET_ENABLE_CACHE "Enable ccache" OFF)
option(DMASSET_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
option(DMASSET_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
option(DMASSET_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
option(DMASSET_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
option(DMASSET_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
option(DMASSET_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)

if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

if(DMASSET_ENABLE_IPO)
    enable_ipo()
endif()

if(DMASSET_ENABLE_CACHE)
    enable_cache()
endif()

add_subdirectory("asset_manager")
add_subdirectory("tool")
add_subdirectory("tests")

set_project_warnings(
    DMAssetManager
    ${DMASSET_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
)
set_project_warnings(
    DMAssetTool
    ${DMASSET_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
)
set_project_warnings(
    AssetTests
    ${DMASSET_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
)

enable_sanitizers(
    DMAssetManager
    ${DMASSET_ENABLE_SANITIZER_ADDRESS}
    ${DMASSET_ENABLE_SANITIZER_LEAK}
    ${DMASSET_ENABLE_SANITIZER_UNDEFINED}
    ${DMASSET_ENABLE_SANITIZER_THREAD}
    ${DMASSET_ENABLE_SANITIZER_MEMORY}
)
enable_sanitizers(
    DMAssetTool
    ${DMASSET_ENABLE_SANITIZER_ADDRESS}
    ${DMASSET_ENABLE_SANITIZER_LEAK}
    ${DMASSET_ENABLE_SANITIZER_UNDEFINED}
    ${DMASSET_ENABLE_SANITIZER_THREAD}
    ${DMASSET_ENABLE_SANITIZER_MEMORY}
)
enable_sanitizers(
    AssetTests
    ${DMASSET_ENABLE_SANITIZER_ADDRESS}
    ${DMASSET_ENABLE_SANITIZER_LEAK}
    ${DMASSET_ENABLE_SANITIZER_UNDEFINED}
    ${DMASSET_ENABLE_SANITIZER_THREAD}
    ${DMASSET_ENABLE_SANITIZER_MEMORY}
)
