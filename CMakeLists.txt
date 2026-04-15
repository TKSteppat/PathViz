cmake_minimum_required(VERSION 3.16)
project(PathViz)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Include backend headers
include_directories(backend/include)

add_executable(PathViz
    backend/src/main.cpp
    backend/src/grid.cpp
    backend/src/BFS.cpp
    backend/src/DFS.cpp
    backend/src/Dijkstra.cpp
    backend/src/pathfinder.cpp
)

target_link_libraries(PathViz sfml-graphics sfml-window sfml-system)