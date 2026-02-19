#include "../include/BFS.h"

BFS::BFS(Grid& g, Node* s, Node* go)
    : grid(g), start(s), goal(go)
{
    frontier.push(start);
    start->visited = true;
}

bool BFS::isFinished() const {
    return finished;
}

bool BFS::step() {
    if (finished || frontier.empty()) {
        finished = true;
        return false;
    }

    Node* current = frontier.front();
    frontier.pop();

    if (current == goal) {
        finished = true;
        return false;
    }

    addNeighbors(current);
    return true;
}

void BFS::addNeighbors(Node* current) {
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    for (int i = 0; i < 4; i++) {
        int nx = current->x + dx[i];
        int ny = current->y + dy[i];

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