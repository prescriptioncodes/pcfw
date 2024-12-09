cmake_minimum_required(VERSION 3.5.0)
project(idfw VERSION 0.1.0 LANGUAGES C)

include_directories(${CMAKE_SOURCE_DIR}/include)

if(LINUX)
add_library(idfw SHARED src/pcfw_linux.c)
endif(LINUX)

target_link_libraries(idfw PUBLIC X11 GL)