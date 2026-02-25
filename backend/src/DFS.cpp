#include "../include/DFS.h"
#include <vector> // needed for directions

bool DFS::step() {
    if (finished || frontier.empty()) {
        finished = true;
        return false;
    }

    Node* current = frontier.top();
    frontier.pop();

    if (current == goal) {
        finished = true;
        return false;
    }

    addNeighbors(current);
    return true;
}

void DFS::addNeighbors(Node* current) {
    // directions: up, down, left, right
    std::vector<std::pair<int,int>> directions = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}
    };

    for (auto& dir : directions) {
        int nx = current->x + dir.first;
        int ny = current->y + dir.second;

        if (grid.isValid(nx, ny)) {
            Node& neighbor = grid.getNode(nx, ny);
            if (!neighbor.visited && !neighbor.isWall) {
                neighbor.visited = true;
                neighbor.parent = current;
                frontier.push(&neighbor);
            }
        }
    }
}