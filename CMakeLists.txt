#############
## WARNING ##
#############
# This file is only for C++ development from Clion, it doesn't permit to build the game

cmake_minimum_required(VERSION 3.6)

project(HotelManager)
include_directories(godotengine/core)

set(SOURCE_FILES
	godotengine/core/reference.cpp)

add_executable(HotelManager ${SOURCE_FILES})
