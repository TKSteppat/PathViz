# PathViz

PathViz is an interactive visualization tool for pathfinding algorithms designed to show how algorithms explore and solve shortest-path problems step by step. Users can create a grid-based environment, place obstacles, and select a pathfinding algorithm to visualize its execution. Instead of only displaying the final path, the system exposes intermediate states such as frontier expansion, node exploration, and pathfinding decisions.

This project is being developed as a senior Computer Science capstone project by Tyler Steppat.

## Features

- Interactive 2D grid editor  
- Click to place walls, right-click to remove walls  
- Drag start (green) and goal (red) nodes  
- Step-by-step pathfinding visualization  
- Press `Space` to start/pause the algorithm  
- Press `R` to reset the grid and clear previous searches  

## Algorithms

- Breadth-First Search (BFS)  
- Depth-First Search (DFS)  
- Dijkstra’s Algorithm (planned)

## Tech Stack

**Backend**
- C++ (C++17 / C++20)
- STL data structures
- SFML for graphics

**Frontend**
- Integrated via SFML window (no separate web UI for MVP)

## Building and Running

### Prerequisites
- C++17 compatible compiler  
- CMake (3.10+)  
- SFML 2.5+  
- Windows, Linux, or WSL/WSLg environment

### Build Steps
1. Clone the repository and enter it:

   ```bash
   git clone <your-repo-url>
   cd PathViz
   ```

2. Create and enter the build directory:

   ```bash
   mkdir build
   cd build
   ```

3. Configure the project with CMake:

   ```bash
   cmake ..
   ```

4. Build the project:

   ```bash
   cmake --build .
   ```

## Running

From the PathViz directory, run:

   ```bash
   ./build/PathViz
   ```

## Controls:

Left-Click: Place walls

Right-Click: Remove walls

Drag Green/Red nodes: Move start/goal positions

Spacebar: Start/pause the selected algorithm

R: Reset the grid

## Status

BFS and DFS fully implemented and interactive

Wall placement, node dragging, and visualization working

Dijkstra’s Algorithm planned for future releases

## Author

Tyler Steppat
