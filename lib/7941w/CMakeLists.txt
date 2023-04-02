cmake_minimum_required(VERSION 2.81)

add_library(7941w STATIC 
    ${CMAKE_CURRENT_LIST_DIR}/lib/7941w.c
)
target_include_directories(7941w INTERFACE include ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(7941w pico_stdlib)