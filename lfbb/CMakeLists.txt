# Lock Free Bipartite Buffer library CMakeLists
# Djordje Nedic 2022

cmake_minimum_required(VERSION 3.16)

project(lfbb
  VERSION 1.0.0
  LANGUAGES C
)

set(SOURCES
  src/lfbb.c
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC c_std_11)

target_include_directories(${PROJECT_NAME}
PUBLIC
  inc
)
