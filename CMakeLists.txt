cmake_minimum_required(VERSION 3.23)
project(Spectra)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
	add_executable (Spectra png2tga.cpp resource.rc)
else()
	add_executable(Spectra png2tga.cpp)
endif()