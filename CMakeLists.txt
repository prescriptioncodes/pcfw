cmake_minimum_required(VERSION 3.5.0)
project(idfw VERSION 0.1.0 LANGUAGES C)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(idfw SHARED src/pcfw.c)

target_link_libraries(idfw PUBLIC X11 GL)