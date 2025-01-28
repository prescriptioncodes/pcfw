cmake_minimum_required(VERSION 3.25.0)
project(pcfw VERSION 3 LANGUAGES CXX)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(pcfw SHARED
            source/pc/framework.cpp
            source/pc/framework_windows.cpp
            source/pc/framework_linux.cpp)

if(UNIX)
    target_link_libraries(pcfw PUBLIC X11 GL pclog)
elseif(WIN32)
    add_definitions(-DPCFW_EXPORTS)
    set_target_properties(pcfw PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
    target_link_libraries(pcfw PUBLIC opengl32 pclog)
endif()


if(LINUX)
install(TARGETS pcfw DESTINATION /usr/lib)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION /usr/include/
    FILES_MATCHING PATTERN "*.hpp"
)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "pcfw")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR "okanauta")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/prescriptioncodes/idfw")
set(CPACK_PACKAGE_DESCRIPTION "A library made with XLib/C++ to use with OpenGL.")
set(CPACK_PACKAGE_CONTACT "oknauta@proton.me")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "oknauta <oknauta@proton.me>")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_PACKAGE_LICENSE "mit")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libx11-dev, libgl-dev")

include(CPack)

endif(LINUX)