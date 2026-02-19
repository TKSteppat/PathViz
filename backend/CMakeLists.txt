cmake_minimum_required(VERSION 3.10)
project(PathViz)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(PathViz
    src/main.cpp
    src/Grid.cpp
    src/BFS.cpp
)