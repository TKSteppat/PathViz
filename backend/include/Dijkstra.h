#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <queue>
#include "Pathfinder.h"

class Dijkstra : public Pathfinder {
public:
    Dijkstra(Grid& grid, Node* start, Node* goal);

    bool step() override;
    bool isFinished() const override;

private:
    struct Compare {
        bool operator()(Node* a, Node* b) {
            return a->distance > b->distance;
        }
    };

    std::priority_queue<Node*, std::vector<Node*>, Compare> frontier;
    bool finished = false;

    void addNeighbors(Node* current);
};

#endif