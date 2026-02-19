#include <iostream>
#include <thread>
#include <chrono>
#include "../include/Grid.h"
#include "../include/BFS.h"

int main() {
    Grid grid(10, 10);

    // Add some walls
    grid.getNode(3, 3).isWall = true;
    grid.getNode(3, 4).isWall = true;
    grid.getNode(3, 5).isWall = true;
    grid.getNode(4, 5).isWall = true;
    grid.getNode(5, 5).isWall = true;

    Node* start = &grid.getNode(0, 0);
    Node* goal = &grid.getNode(9, 9);

    BFS bfs(grid, start, goal);

    while (!bfs.isFinished()) {
        bfs.step();

        system("clear"); // use "cls" on Windows
        grid.print(start, goal);

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    std::cout << "Search complete.\n";
    return 0;
}