#ifndef NODE_H
#define NODE_H

#include <limits>

struct Node {
    int x;
    int y;

    bool isWall = false;
    bool visited = false;

    float distance = std::numeric_limits<float>::infinity();
    float weight = 1.0f;

    Node* parent = nullptr;

    Node(int xPos = 0, int yPos = 0)
        : x(xPos), y(yPos) {}
};

#endif