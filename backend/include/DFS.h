#ifndef DFS_H
#define DFS_H

#include <stack>
#include "pathfinder.h"

class DFS : public Pathfinder {
public:
    DFS(Grid& g, Node* s, Node* go)
        : Pathfinder(g, s, go) {
        frontier.push(start);
        start->visited = true;
    }

    bool step() override;
    bool isFinished() const override { return finished; }

private:
    std::stack<Node*> frontier;
    bool finished = false;

    void addNeighbors(Node* current);
};

#endif