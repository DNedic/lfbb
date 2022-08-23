# Tests CMakeLists for LFBB
# Djordje Nedic 2022

cmake_minimum_required(VERSION 3.16)

project(tests)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.1.0
)

FetchContent_MakeAvailable(Catch2)

add_subdirectory(../lfbb lfbb)

add_executable(${PROJECT_NAME} tests.cpp)

target_link_libraries(${PROJECT_NAME}
PRIVATE
  lfbb
  Catch2::Catch2WithMain
)
