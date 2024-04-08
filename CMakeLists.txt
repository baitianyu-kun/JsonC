cmake_minimum_required(VERSION 3.27)
project(JsonC)

set(CMAKE_CXX_STANDARD 17)

add_executable(JsonC
        include/json_impl.h
        src/json_impl.cpp
        include/parser.h
        src/parser.cpp
        src/test.cpp
        include/status.h)
