# Project CMakeLists
# Djordje Nedic 2022

cmake_minimum_required(VERSION 3.16)

project(lfbb
    VERSION 1.3.5
    LANGUAGES C CXX
)

add_subdirectory(lfbb)

# Library configuration
if (DEFINED LFBB_MULTICORE_HOSTED)
    target_compile_definitions(${PROJECT_NAME} PRIVATE LFBB_MULTICORE_HOSTED)
endif()

if (DEFINED LFBB_CACHELINE_LENGTH)
    target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        LFBB_CACHELINE_LENGTH=${LFBB_CACHELINE_LENGTH}
    )
endif()

# Only build tests if we're actually working on the library,
# not when the library is being used in a project
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    enable_testing()
    add_subdirectory(tests)
endif()
