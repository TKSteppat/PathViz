#ifndef BFS_H
#define BFS_H

#include <queue>
#include "pathfinder.h"

class BFS : public Pathfinder {
public:
    BFS(Grid& g, Node* s, Node* go)
        : Pathfinder(g, s, go) {
        frontier.push(start);
        start->visited = true;
    }

    bool step() override;           // Executes one BFS iteration
    bool isFinished() const override { return finished; }

private:
    std::queue<Node*> frontier;
    bool finished = false;

    void addNeighbors(Node* current);
};

#endif