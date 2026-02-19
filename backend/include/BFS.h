#ifndef BFS_H
#define BFS_H

#include <queue>
#include "Grid.h"

class BFS {
public:
    BFS(Grid& grid, Node* start, Node* goal);

    bool step();          // Executes one BFS iteration
    bool isFinished() const;

private:
    Grid& grid;
    Node* start;
    Node* goal;

    std::queue<Node*> frontier;
    bool finished = false;

    void addNeighbors(Node* current);
};

#endif