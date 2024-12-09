cmake_minimum_required(VERSION 3.5.0)
project(idfw VERSION 0.1.0 LANGUAGES CXX)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(pcfw SHARED src/pcfw_linux.cpp)

target_link_libraries(pcfw PUBLIC X11 GL)

install(TARGETS pcfw DESTINATION /usr/lib)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION /usr/include/pcfw
    FILES_MATCHING PATTERN "*.hpp"
)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "pcfw")
set(CPACK_PACKAGE_VERSION "0.1.1")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/prescriptioncodes/idfw")
set(CPACK_PACKAGE_DESCRIPTION "A library made with XLib/C++ to use with OpenGL.")
set(CPACK_PACKAGE_CONTACT "oknauta@proton.me")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "oknauta <oknauta@proton.me>")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_PACKAGE_LICENSE "MIT")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libx11-dev, libgl-dev")

include(CPack)