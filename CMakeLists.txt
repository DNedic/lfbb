# Project CMakeLists
# Djordje Nedic 2022

cmake_minimum_required(VERSION 3.16)

project(lfbb
  VERSION 1.2.1
  LANGUAGES C CXX
)

add_subdirectory(lfbb)

# Only build tests if we're actually working on the library,
# not when the library is being used in a project
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  add_subdirectory(tests)
endif()
