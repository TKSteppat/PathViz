#include "../include/grid.h"
#include <iostream>

Grid::Grid(int w, int h) : width(w), height(h) {
    nodes.resize(height);
    for (int y = 0; y < height; y++) {
        nodes[y].reserve(width);
        for (int x = 0; x < width; x++) {
            nodes[y].emplace_back(x, y);
        }
    }
}

Node& Grid::getNode(int x, int y) {
    return nodes[y][x];
}

bool Grid::isValid(int x, int y) const {
    return x >= 0 && x < width &&
           y >= 0 && y < height;
}

int Grid::getWidth() const {
    return width;
}

int Grid::getHeight() const {
    return height;
}

void Grid::print(Node* start, Node* goal) const {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const Node& n = nodes[y][x];

            if (&n == start) {
                std::cout << "S ";
            }
            else if (&n == goal) {
                std::cout << "G ";
            }
            else if (n.isWall) {
                std::cout << "# ";
            }
            else if (n.visited) {
                std::cout << ". ";
            }
            else {
                std::cout << "- ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}