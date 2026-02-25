#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Grid.h"
#include "Node.h"

class Pathfinder {
public:
    Pathfinder(Grid& g, Node* s, Node* go)
        : grid(g), start(s), goal(go) {}

    virtual bool step() = 0;
    virtual bool isFinished() const = 0;

protected:
    Grid& grid;
    Node* start;
    Node* goal;
};

#endif