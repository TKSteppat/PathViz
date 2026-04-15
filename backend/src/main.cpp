#include <SFML/Graphics.hpp>
#include "../include/grid.h"
#include "../include/BFS.h"
#include "../include/DFS.h"
#include "../include/Dijkstra.h"
#include <iostream>
#include <limits>
#include <vector>

// =========================
// RESET ONLY ALGORITHM STATE
// =========================
void resetGridState(Grid& grid, int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Node& n = grid.getNode(r, c);
            n.visited = false;
            n.parent = nullptr;
            n.distance = std::numeric_limits<float>::infinity();
        }
    }
}

// =========================
// FULL RESET BOARD
// =========================
void clearBoard(Grid& grid, int rows, int cols, Node* start, Node* goal) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Node& n = grid.getNode(r, c);
            n.visited = false;
            n.parent = nullptr;
            n.distance = std::numeric_limits<float>::infinity();
            n.isWall = false;
        }
    }

    start->isWall = false;
    goal->isWall = false;
}

// =========================
// MAZE GENERATOR (NO BORDERS, FULL GRID USABLE)
// =========================
void generateMaze(Grid& grid, int rows, int cols, Node* start, Node* goal) {
    clearBoard(grid, rows, cols, start, goal);

    // STEP 1: fill everything with walls
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            grid.getNode(r, c).isWall = true;

    std::vector<Node*> stack;

    // Start anywhere (NO BORDER RESTRICTIONS)
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

            // FULL GRID ACCESS (NO BORDERS)
            if (nx >= 0 && nx < rows &&
                ny >= 0 && ny < cols) {

                if (grid.getNode(nx, ny).isWall)
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

    // Ensure endpoints are always open
    start->isWall = false;
    goal->isWall = false;
}

// =========================
// BUTTON STRUCT
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
};

// =========================
// MAIN
// =========================
int main() {
    const int rows = 20;
    const int cols = 20;
    const int cellSize = 30;

    Grid grid(rows, cols);

    Node* start = &grid.getNode(0, 0);
    Node* goal  = &grid.getNode(rows - 1, cols - 1);

    sf::RenderWindow window(
        sf::VideoMode(cols * cellSize + 200, rows * cellSize),
        "PathViz Toolbar"
    );

    sf::Font font;
    font.loadFromFile("assets/fonts/Sansation.ttf");

    // =========================
    // TOOLBAR
    // =========================
    Button bfsBtn, dfsBtn, dijBtn, mazeBtn, resetBtn;

    Button* buttons[] = {&bfsBtn, &dfsBtn, &dijBtn, &mazeBtn, &resetBtn};
    std::string labels[] = {"BFS", "DFS", "Dijkstra", "Maze", "Reset"};

    for (int i = 0; i < 5; i++) {
        buttons[i]->shape.setSize({180, 40});
        buttons[i]->shape.setPosition(10, 10 + i * 50);
        buttons[i]->shape.setFillColor(sf::Color(200, 200, 200));

        buttons[i]->text.setFont(font);
        buttons[i]->text.setString(labels[i]);
        buttons[i]->text.setCharacterSize(20);
        buttons[i]->text.setFillColor(sf::Color::Black);
        buttons[i]->text.setPosition(30, 15 + i * 50);
    }

    Pathfinder* algo = nullptr;
    bool running = false;
    sf::Clock clock;
    int delay = 50;

    // =========================
    // LOOP
    // =========================
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                auto mouse = sf::Mouse::getPosition(window);

                if (bfsBtn.isClicked(mouse))
                    algo = new BFS(grid, start, goal);

                if (dfsBtn.isClicked(mouse))
                    algo = new DFS(grid, start, goal);

                if (dijBtn.isClicked(mouse))
                    algo = new Dijkstra(grid, start, goal);

                if (mazeBtn.isClicked(mouse))
                    generateMaze(grid, rows, cols, start, goal);

                if (resetBtn.isClicked(mouse)) {
                    running = false;
                    delete algo;
                    algo = nullptr;
                    clearBoard(grid, rows, cols, start, goal);
                }
            }
        }

        // START
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            running = true;

        // WALL PLACEMENT
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            auto mouse = sf::Mouse::getPosition(window);

            int c = (mouse.x - 200) / cellSize;
            int r = mouse.y / cellSize;

            if (r >= 0 && r < rows && c >= 0 && c < cols) {
                Node& n = grid.getNode(r, c);

                if (&n != start && &n != goal)
                    n.isWall = true;
            }
        }

        // RUN ALGO
        if (running && algo && !algo->isFinished()) {
            if (clock.getElapsedTime().asMilliseconds() > delay) {
                algo->step();
                clock.restart();
            }
        }

        // =========================
        // RENDER
        // =========================
        window.clear(sf::Color::White);

        for (auto b : buttons)
            b->draw(window);

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                Node& n = grid.getNode(r, c);

                sf::RectangleShape cell({cellSize - 1, cellSize - 1});
                cell.setPosition(c * cellSize + 200, r * cellSize);

                if (&n == start) cell.setFillColor(sf::Color::Green);
                else if (&n == goal) cell.setFillColor(sf::Color::Red);
                else if (n.isWall) cell.setFillColor(sf::Color::Black);
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