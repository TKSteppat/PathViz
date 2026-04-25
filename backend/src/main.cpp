#include <SFML/Graphics.hpp>
#include "../include/grid.h"
#include "../include/BFS.h"
#include "../include/DFS.h"
#include "../include/Dijkstra.h"
#include <iostream>
#include <limits>
#include <vector>

// =========================
// STATE
// =========================
enum AlgoType { NONE, BFS_T, DFS_T, DIJKSTRA_T };

AlgoType selectedAlgo = NONE;
bool weightMode = false;
int delay = 50;

bool draggingStart = false;
bool draggingGoal = false;

// =========================
// HELPERS
// =========================
void clearBoard(Grid& grid, int rows, int cols, Node* start, Node* goal) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Node& n = grid.getNode(r, c);
            n.visited = false;
            n.parent = nullptr;
            n.distance = std::numeric_limits<float>::infinity();
            n.isWall = false;
            n.weight = 1.0f;
        }
    }

    start->isWall = false;
    goal->isWall = false;
}

void generateMaze(Grid& grid, int rows, int cols, Node* start, Node* goal) {
    clearBoard(grid, rows, cols, start, goal);

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            grid.getNode(r, c).isWall = true;

    std::vector<Node*> stack;

    Node& startNode = grid.getNode(0, 0);
    startNode.isWall = false;
    stack.push_back(&startNode);

    int dx[4] = {2, -2, 0, 0};
    int dy[4] = {0, 0, 2, -2};

    while (!stack.empty()) {
        Node* cur = stack.back();

        std::vector<Node*> neighbors;

        for (int i = 0; i < 4; i++) {
            int nx = cur->x + dx[i];
            int ny = cur->y + dy[i];

            if (nx >= 0 && nx < rows &&
                ny >= 0 && ny < cols &&
                grid.getNode(nx, ny).isWall) {
                neighbors.push_back(&grid.getNode(nx, ny));
            }
        }

        if (!neighbors.empty()) {
            Node* next = neighbors[rand() % neighbors.size()];

            int mx = (cur->x + next->x) / 2;
            int my = (cur->y + next->y) / 2;

            grid.getNode(mx, my).isWall = false;
            next->isWall = false;

            stack.push_back(next);
        } else {
            stack.pop_back();
        }
    }

    start->isWall = false;
    goal->isWall = false;
}

// =========================
// BUTTON
// =========================
struct Button {
    sf::RectangleShape shape;
    sf::Text text;

    bool isClicked(sf::Vector2i mouse) {
        return shape.getGlobalBounds().contains(mouse.x, mouse.y);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }

    void setColor(sf::Color c) {
        shape.setFillColor(c);
    }
};

void updateButtonColors(Button& bfs, Button& dfs, Button& dij, Button& weightBtn) {
    bfs.setColor(selectedAlgo == BFS_T ? sf::Color(120,255,120) : sf::Color(200,200,200));
    dfs.setColor(selectedAlgo == DFS_T ? sf::Color(120,255,120) : sf::Color(200,200,200));
    dij.setColor(selectedAlgo == DIJKSTRA_T ? sf::Color(120,255,120) : sf::Color(200,200,200));

    weightBtn.setColor(weightMode ? sf::Color(180,120,255) : sf::Color(200,200,200));
}

// =========================
// STATS
// =========================
int countVisited(Grid& grid, int rows, int cols) {
    int count = 0;
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            if (grid.getNode(r,c).visited) count++;
    return count;
}

int countWalls(Grid& grid, int rows, int cols) {
    int count = 0;
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            if (grid.getNode(r,c).isWall) count++;
    return count;
}

int countWeights(Grid& grid, int rows, int cols) {
    int count = 0;
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            if (grid.getNode(r,c).weight > 1.0f) count++;
    return count;
}

// =========================
// MAIN
// =========================
int main() {
    const int rows = 20;
    const int cols = 20;
    const int cellSize = 30;
    const int toolbarWidth = 240;

    Grid grid(rows, cols);

    Node* start = &grid.getNode(0,0);
    Node* goal = &grid.getNode(rows-1, cols-1);

    sf::RenderWindow window(
        sf::VideoMode(cols * cellSize + toolbarWidth, rows * cellSize),
        "PathViz"
    );

    sf::Font font;
    font.loadFromFile("assets/fonts/Sansation.ttf");

    Button bfsBtn, dfsBtn, dijBtn, mazeBtn, resetBtn;
    Button weightBtn, speedUpBtn, speedDownBtn;

    Button* buttons[] = {
        &bfsBtn, &dfsBtn, &dijBtn,
        &mazeBtn, &resetBtn,
        &weightBtn, &speedUpBtn, &speedDownBtn
    };

    std::string labels[] = {
        "BFS", "DFS", "Dijkstra",
        "Maze", "Reset",
        "Weight", "Speed +", "Speed -"
    };

    for (int i = 0; i < 8; i++) {
        buttons[i]->shape.setSize({200, 35});
        buttons[i]->shape.setPosition(10, 10 + i * 45);
        buttons[i]->shape.setFillColor(sf::Color(200,200,200));

        buttons[i]->text.setFont(font);
        buttons[i]->text.setString(labels[i]);
        buttons[i]->text.setCharacterSize(18);
        buttons[i]->text.setFillColor(sf::Color::Black);
        buttons[i]->text.setPosition(25, 15 + i * 45);
    }

    sf::Text statsText;
    statsText.setFont(font);
    statsText.setCharacterSize(20);
    statsText.setFillColor(sf::Color::Black);
    statsText.setPosition(10, 400);

    Pathfinder* algo = nullptr;
    bool running = false;
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                auto mouse = sf::Mouse::getPosition(window);

                int c = (mouse.x - toolbarWidth) / cellSize;
                int r = mouse.y / cellSize;

                if (r >= 0 && r < rows && c >= 0 && c < cols) {
                    Node* clicked = &grid.getNode(r,c);

                    if (clicked == start) draggingStart = true;
                    else if (clicked == goal) draggingGoal = true;
                }

                if (bfsBtn.isClicked(mouse)) {
                    selectedAlgo = BFS_T;
                    algo = new BFS(grid, start, goal);
                }

                if (dfsBtn.isClicked(mouse)) {
                    selectedAlgo = DFS_T;
                    algo = new DFS(grid, start, goal);
                }

                if (dijBtn.isClicked(mouse)) {
                    selectedAlgo = DIJKSTRA_T;
                    algo = new Dijkstra(grid, start, goal);
                }

                if (mazeBtn.isClicked(mouse))
                    generateMaze(grid, rows, cols, start, goal);

                if (resetBtn.isClicked(mouse)) {
                    running = false;
                    delete algo;
                    algo = nullptr;
                    clearBoard(grid, rows, cols, start, goal);
                }

                if (weightBtn.isClicked(mouse))
                    weightMode = !weightMode;

                if (speedUpBtn.isClicked(mouse))
                    delay = std::max(5, delay - 10);

                if (speedDownBtn.isClicked(mouse))
                    delay = std::min(300, delay + 10);
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                draggingStart = false;
                draggingGoal = false;
            }
        }

        // LEFT CLICK (draw OR drag)
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            auto mouse = sf::Mouse::getPosition(window);

            int c = (mouse.x - toolbarWidth) / cellSize;
            int r = mouse.y / cellSize;

            if (r >= 0 && r < rows && c >= 0 && c < cols) {
                Node* target = &grid.getNode(r, c);

                if (draggingStart && target != goal && !target->isWall) {
                    start = target;
                }
                else if (draggingGoal && target != start && !target->isWall) {
                    goal = target;
                }
                else {
                    if (target != start && target != goal) {
                        if (weightMode) {
                            target->weight = 5.0f;
                            target->isWall = false;
                        } else {
                            target->isWall = true;
                            target->weight = 1.0f;
                        }
                    }
                }
            }
        }

        // RIGHT CLICK (erase)
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            auto mouse = sf::Mouse::getPosition(window);

            int c = (mouse.x - toolbarWidth) / cellSize;
            int r = mouse.y / cellSize;

            if (r >= 0 && r < rows && c >= 0 && c < cols) {
                Node& n = grid.getNode(r,c);
                if (&n != start && &n != goal) {
                    n.isWall = false;
                    n.weight = 1.0f;
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            running = true;

        updateButtonColors(bfsBtn, dfsBtn, dijBtn, weightBtn);

        if (running && algo && !algo->isFinished()) {
            if (clock.getElapsedTime().asMilliseconds() > delay) {
                algo->step();
                clock.restart();
            }
        }

        statsText.setString(
            "Visited: " + std::to_string(countVisited(grid, rows, cols)) +
            "\nWalls: " + std::to_string(countWalls(grid, rows, cols)) +
            "\nWeights: " + std::to_string(countWeights(grid, rows, cols)) +
            "\nSpeed: " + std::to_string(delay) + " ms"
        );

        window.clear(sf::Color::White);

        for (auto b : buttons)
            b->draw(window);

        window.draw(statsText);

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                Node& n = grid.getNode(r,c);

                sf::RectangleShape cell({cellSize - 1, cellSize - 1});
                cell.setPosition(c * cellSize + toolbarWidth, r * cellSize);

                if (&n == start) cell.setFillColor(sf::Color::Green);
                else if (&n == goal) cell.setFillColor(sf::Color::Red);
                else if (n.isWall) cell.setFillColor(sf::Color::Black);
                else if (n.weight > 1.0f) cell.setFillColor(sf::Color(160,80,200));
                else if (n.visited) cell.setFillColor(sf::Color::Blue);
                else cell.setFillColor(sf::Color::White);

                window.draw(cell);
            }
        }

        window.display();
    }

    delete algo;
    return 0;
}