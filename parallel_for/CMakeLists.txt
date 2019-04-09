cmake_minimum_required(VERSION 2.8)

project(parallel_for)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GNULINUX")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -lpthread -ltbb")

add_executable(${PROJECT_NAME} "main.cpp")
