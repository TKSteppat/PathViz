#include "../include/Dijkstra.h"

Dijkstra::Dijkstra(Grid& g, Node* s, Node* go)
    : Pathfinder(g, s, go)
{
    start->distance = 0;
    frontier.push(start);
}

bool Dijkstra::isFinished() const {
    return finished;
}

bool Dijkstra::step() {
    if (finished || frontier.empty()) {
        finished = true;
        return false;
    }

    Node* current = frontier.top();
    frontier.pop();

    // Skip if already processed
    if (current->visited)
        return true;

    current->visited = true;

    if (current == goal) {
        finished = true;
        return false;
    }

    addNeighbors(current);
    return true;
}

void Dijkstra::addNeighbors(Node* current) {
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    for (int i = 0; i < 4; i++) {
        int nx = current->x + dx[i];
        int ny = current->y + dy[i];

        if (grid.isValid(nx, ny)) {
            Node& neighbor = grid.getNode(nx, ny);

            if (neighbor.isWall)
                continue;

            float newDist = current->distance + neighbor.weight;

            if (newDist < neighbor.distance) {
                neighbor.distance = newDist;
                neighbor.parent = current;
                frontier.push(&neighbor);
            }
        }
    }
}