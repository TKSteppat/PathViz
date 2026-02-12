#ifndef NODE_H
#define NODE_H

struct Node {
    int x;
    int y;

    bool isWall = false;
    bool visited = false;

    Node* parent = nullptr;

    Node(int xPos = 0, int yPos = 0)
        : x(xPos), y(yPos) {}
};

#endif