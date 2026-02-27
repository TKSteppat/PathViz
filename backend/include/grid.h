#ifndef GRID_H
#define GRID_H

#include <vector>
#include "node.h"

class Grid {
public:
    Grid(int width, int height);

    Node& getNode(int x, int y);
    bool isValid(int x, int y) const;

    int getWidth() const;
    int getHeight() const;

    void print(Node* start = nullptr, Node* goal = nullptr) const;

private:
    int width;
    int height;
    std::vector<std::vector<Node>> nodes;
};

#endif