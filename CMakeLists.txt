cmake_minimum_required(VERSION 3.13)
 
include(pico_sdk_import.cmake)
 
project(pux C CXX ASM)
 
pico_sdk_init()

add_executable(pux
  main.cpp
)

target_link_libraries(pux
  pico_stdlib
)

pico_add_extra_outputs(pux)
