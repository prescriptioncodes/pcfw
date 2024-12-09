cmake_minimum_required(VERSION 3.5.0)
project(idfw VERSION 0.1.0 LANGUAGES CXX)

include_directories(${CMAKE_SOURCE_DIR}/include)

if(LINUX)
add_library(pcfw SHARED src/pcfw_linux.cpp)
endif(LINUX)

target_link_libraries(pcfw PUBLIC X11 GL)