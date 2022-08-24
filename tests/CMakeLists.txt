# Tests CMakeLists
# Djordje Nedic 2022

cmake_minimum_required(VERSION 3.16)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.1.0
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests tests.cpp)

target_link_libraries(tests
PRIVATE
  ${PROJECT_NAME}
  Catch2::Catch2WithMain
)
