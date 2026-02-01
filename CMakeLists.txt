cmake_minimum_required(VERSION 3.31)

project(boiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(boiler src/boiler.cpp
                      src/info.cpp
                      src/main.cpp)

target_include_directories(boiler PRIVATE include)
