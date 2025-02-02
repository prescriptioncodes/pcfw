cmake_minimum_required(VERSION 3.25.0)
project(pcfw VERSION 4 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(pcfw SHARED source/pc/framework.cpp source/pc/framework_windows.cpp source/pc/framework_linux.cpp)

target_include_directories(pcfw PUBLIC include)


if(UNIX)
	target_link_libraries(pcfw PUBLIC X11 GL pclog)
elseif(WIN32)
	add_definitions(-DPCFW_EXPORTS)
	set_target_properties(pcfw PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
	target_link_libraries(pcfw PUBLIC opengl32 pclog)
endif()

if(LINUX)
	install(TARGETS pcfw DESTINATION lib)
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION /include/ FILES_MATCHING PATTERN "*.hpp")

	set(CPACK_GENERATOR "TGZ")
	set(CPACK_PACKAGE_NAME "libpcfw")
	set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
	set(CPACK_PACKAGE_VENDOR "okanauta")
	set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/prescriptioncodes/idfw")
	set(CPACK_PACKAGE_DESCRIPTION "A library made with XLib/C++ to use with OpenGL.")
	set(CPACK_PACKAGE_CONTACT "oknauta@proton.me")
	set(CPACK_PACKAGE_LICENSE "mit")

	include(CPack)
endif(LINUX)
