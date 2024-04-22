cmake_minimum_required(VERSION 3.27)
project(JsonC)

set(CMAKE_CXX_STANDARD 17)

add_executable(JsonC
        include/parser.h
        src/parser.cpp
        include/status.h
        include/reflection.h
        include/cars.h
        src/TestReflect.cpp
)