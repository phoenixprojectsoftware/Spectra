cmake_minimum_required(VERSION 3.23)
project(png2tga)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
	add_executable (png2tga png2tga.cpp resource.rc)
else()
	add_executable(png2tga png2tga.cpp)
endif()