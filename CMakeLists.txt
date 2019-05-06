cmake_minimum_required(VERSION 2.8.8)
project(KAudio3D)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	find_path(OPENAL_INCLUDE_DIRS NAMES "AL/al.h" PATHS "deps/include")
	find_library(OPENAL_LIBRARIES "OpenAL32.lib" PATHS "deps/libs")
else()
	find_package(OpenAL REQUIRED)
endif()

include_directories(${OPENAL_INCLUDE_DIRS})

aux_source_directory(. SRC_LIST)

add_library(${PROJECT_NAME} STATIC ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} ${OPENAL_LIBRARIES})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	target_compile_options(${PROJECT_NAME} PRIVATE "-Wall")
	target_compile_options(${PROJECT_NAME} PRIVATE "-pedantic")
endif()

# Extract from Supertux 0.5.1 cmake (with modifications)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	# http://www.guyrutenberg.com/2014/01/05/enabling-c11-c0x-in-cmake/
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	if(COMPILER_SUPPORTS_CXX11)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	else(COMPILER_SUPPORTS_CXX11)
		CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
		if(COMPILER_SUPPORTS_CXX0X)
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
		else(COMPILER_SUPPORTS_CXX0X)
			message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
		endif(COMPILER_SUPPORTS_CXX0X)
	endif(COMPILER_SUPPORTS_CXX11)

	SET(CMAKE_C_FLAGS_RELEASE "-O2 -g" CACHE STRING "Release flags")
	SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -g" CACHE STRING "Release flags")
	SET(CMAKE_C_FLAGS_DEBUG "-O0 -g" CACHE STRING "Debug flags")
	SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g" CACHE STRING "Debug flags")
	SET(CMAKE_C_FLAGS_PROFILE "-pg" CACHE STRING "Profile flags")
	SET(CMAKE_CXX_FLAGS_PROFILE "-pg" CACHE STRING "Profile flags")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -funit-at-a-time")
endif()

