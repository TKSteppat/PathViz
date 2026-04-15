#include <SFML/Graphics.hpp>
#include "../include/grid.h"
#include "../include/BFS.h"
#include "../include/DFS.h"
#include "../include/Dijkstra.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <vector>

// =====================================================
// RESET ALGORITHM STATE (NO MENU INTERRUPTION)
// =====================================================
void resetAlgorithm(Grid& grid, Pathfinder*& algo, Node*& start, Node*& goal, int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Node& n = grid.getNode(r, c);
            n.visited = false;
            n.parent = nullptr;
            n.distance = std::numeric_limits<float>::infinity();
        }
    }

    delete algo;
    algo = nullptr;

    start = &grid.getNode(0, 0);
    goal  = &grid.getNode(rows - 1, cols - 1);
}

// =====================================================
// SAFE + SOLVABLE MAZE GENERATOR
// =====================================================
void generatePerfectMaze(Grid& grid, Node* start, Node* goal, int rows, int cols) {
    // Fill everything with walls first
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            grid.getNode(r, c).isWall = true;
        }
    }

    std::vector<Node*> stack;

    Node& startNode = grid.getNode(0, 0);
    startNode.isWall = false;
    stack.push_back(&startNode);

    int dx[4] = {2, -2, 0, 0};
    int dy[4] = {0, 0, 2, -2};

    while (!stack.empty()) {
        Node* current = stack.back();

        std::vector<Node*> neighbors;

        for (int i = 0; i < 4; i++) {
            int nx = current->x + dx[i];
            int ny = current->y + dy[i];

            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                if (grid.getNode(nx, ny).isWall)
                    neighbors.push_back(&grid.getNode(nx, ny));
            }
        }

        if (!neighbors.empty()) {
            Node* next = neighbors[rand() % neighbors.size()];

            int mx = (current->x + next->x) / 2;
            int my = (current->y + next->y) / 2;

            grid.getNode(mx, my).isWall = false;
            next->isWall = false;

            stack.push_back(next);
        } else {
            stack.pop_back();
        }
    }

    // Guarantee start and end are open
    start->isWall = false;
    goal->isWall = false;
}

// =====================================================
// MENU
// =====================================================
Pathfinder* showMenuAndChoose(Grid& grid, Node*& start, Node*& goal, sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/Sansation.ttf"))
        return nullptr;

    sf::RectangleShape bfsButton({200, 50});
    bfsButton.setPosition(100, 100);

    sf::RectangleShape dfsButton({200, 50});
    dfsButton.setPosition(100, 200);

    sf::RectangleShape dijButton({200, 50});
    dijButton.setPosition(100, 300);

    sf::RectangleShape mazeButton({200, 50});
    mazeButton.setPosition(100, 400);

    sf::Text bfsText("BFS", font, 24);
    bfsText.setPosition(150, 110);
    bfsText.setFillColor(sf::Color::Black);

    sf::Text dfsText("DFS", font, 24);
    dfsText.setPosition(150, 210);
    dfsText.setFillColor(sf::Color::Black);

    sf::Text dijText("Dijkstra", font, 24);
    dijText.setPosition(120, 310);
    dijText.setFillColor(sf::Color::Black);

    sf::Text mazeText("Maze", font, 24);
    mazeText.setPosition(140, 410);
    mazeText.setFillColor(sf::Color::Black);

    const int rows = 20;
    const int cols = 20;

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                auto mouse = sf::Mouse::getPosition(window);

                if (bfsButton.getGlobalBounds().contains(mouse.x, mouse.y))
                    return new BFS(grid, start, goal);

                if (dfsButton.getGlobalBounds().contains(mouse.x, mouse.y))
                    return new DFS(grid, start, goal);

                if (dijButton.getGlobalBounds().contains(mouse.x, mouse.y))
                    return new Dijkstra(grid, start, goal);

                if (mazeButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
                    generatePerfectMaze(grid, start, goal, rows, cols);
                }
            }
        }

        window.clear(sf::Color::Black);

        window.draw(bfsButton);
        window.draw(bfsText);

        window.draw(dfsButton);
        window.draw(dfsText);

        window.draw(dijButton);
        window.draw(dijText);

        window.draw(mazeButton);
        window.draw(mazeText);

        window.display();
    }

    return nullptr;
}

// =====================================================
// MAIN LOOP
// =====================================================
int main() {
    const int rows = 20, cols = 20, cellSize = 30;

    Grid grid(rows, cols);

    Node* start = &grid.getNode(0, 0);
    Node* goal  = &grid.getNode(rows - 1, cols - 1);

    sf::RenderWindow window(sf::VideoMode(cols * cellSize, rows * cellSize), "PathViz");

    Pathfinder* algo = showMenuAndChoose(grid, start, goal, window);
    if (!algo) return 0;

    bool running = false;
    int delay = 50;
    bool weightMode = false;

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {

                if (event.key.code == sf::Keyboard::Space)
                    running = true;

                if (event.key.code == sf::Keyboard::Up)
                    delay = std::max(5, delay - 5);

                if (event.key.code == sf::Keyboard::Down)
                    delay += 5;

                if (event.key.code == sf::Keyboard::W)
                    weightMode = !weightMode;

                // CLEAR ONLY
                if (event.key.code == sf::Keyboard::C) {
                    running = false;
                    resetAlgorithm(grid, algo, start, goal, rows, cols);
                }

                // FULL RESTART
                if (event.key.code == sf::Keyboard::R) {
                    running = false;
                    resetAlgorithm(grid, algo, start, goal, rows, cols);

                    algo = showMenuAndChoose(grid, start, goal, window);
                    if (!algo) return 0;
                }
            }
        }

        // DRAW WALLS / WEIGHTS
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            auto mouse = sf::Mouse::getPosition(window);
            int c = mouse.x / cellSize;
            int r = mouse.y / cellSize;

            if (r >= 0 && r < rows && c >= 0 && c < cols) {
                Node& node = grid.getNode(r, c);

                if (&node != start && &node != goal) {
                    if (weightMode) {
                        node.weight = 5.0f;
                        node.isWall = false;
                    } else {
                        node.isWall = true;
                        node.weight = 1.0f;
                    }
                }
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            auto mouse = sf::Mouse::getPosition(window);
            int c = mouse.x / cellSize;
            int r = mouse.y / cellSize;

            if (r >= 0 && r < rows && c >= 0 && c < cols) {
                Node& node = grid.getNode(r, c);

                if (&node != start && &node != goal) {
                    node.isWall = false;
                    node.weight = 1.0f;
                }
            }
        }

        // STEP ALGORITHM
        if (running && !algo->isFinished() &&
            clock.getElapsedTime().asMilliseconds() > delay) {
            algo->step();
            clock.restart();
        }

        // RENDER
        window.clear(sf::Color::White);

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                Node& node = grid.getNode(r, c);

                sf::RectangleShape cell({cellSize - 1, cellSize - 1});
                cell.setPosition(c * cellSize, r * cellSize);

                if (&node == start) cell.setFillColor(sf::Color::Green);
                else if (&node == goal) cell.setFillColor(sf::Color::Red);
                else if (node.isWall) cell.setFillColor(sf::Color::Black);
                else if (node.weight > 1) cell.setFillColor(sf::Color(128, 0, 128));
                else if (node.visited) cell.setFillColor(sf::Color::Blue);
                else cell.setFillColor(sf::Color::White);

                window.draw(cell);
            }
        }

        window.display();
    }

    delete algo;
    return 0;
}