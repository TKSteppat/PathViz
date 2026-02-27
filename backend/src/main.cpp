#include <SFML/Graphics.hpp>
#include "../include/grid.h"
#include "../include/BFS.h"
#include "../include/DFS.h"
// #include "../include/Dijkstra.h" // Uncomment when Dijkstra is added
#include <iostream>

Pathfinder* showMenuAndChoose(Grid& grid, Node* start, Node* goal, sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/sansation.ttf")) {
        // If font fails, we just exit menu
        return nullptr;
    }

    // Buttons as rectangles
    sf::RectangleShape bfsButton(sf::Vector2f(200, 50));
    bfsButton.setPosition(100, 100);
    bfsButton.setFillColor(sf::Color::White);

    sf::RectangleShape dfsButton(sf::Vector2f(200, 50));
    dfsButton.setPosition(100, 200);
    dfsButton.setFillColor(sf::Color::White);

    sf::RectangleShape dijkstraButton(sf::Vector2f(200, 50));
    dijkstraButton.setPosition(100, 300);
    dijkstraButton.setFillColor(sf::Color::White);

    // Button labels
    sf::Text bfsText("BFS", font, 24);
    bfsText.setFillColor(sf::Color::Black);
    bfsText.setPosition(150, 110);

    sf::Text dfsText("DFS", font, 24);
    dfsText.setFillColor(sf::Color::Black);
    dfsText.setPosition(150, 210);

    sf::Text dijkstraText("Dijkstra's", font, 24);
    dijkstraText.setFillColor(sf::Color::Black);
    dijkstraText.setPosition(130, 310);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (bfsButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    return new BFS(grid, start, goal);
                if (dfsButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    return new DFS(grid, start, goal);
                if (dijkstraButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    return nullptr; // placeholder
            }
        }

        window.clear(sf::Color::Black);

        // Draw buttons and labels
        window.draw(bfsButton);
        window.draw(dfsButton);
        window.draw(dijkstraButton);

        window.draw(bfsText);
        window.draw(dfsText);
        window.draw(dijkstraText);

        window.display();
    }

    return nullptr;
}

int main() {
    const int rows = 20;
    const int cols = 20;
    const int cellSize = 30;

    Grid grid(rows, cols);

    // Add sample walls
    for (int i = 5; i < 15; i++) grid.getNode(10, i).isWall = true;
    for (int i = 0; i < 8; i++) grid.getNode(i, 5).isWall = true;

    Node* start = &grid.getNode(0, 0);
    Node* goal = &grid.getNode(rows - 1, cols - 1);

    sf::RenderWindow window(sf::VideoMode(cols * cellSize, rows * cellSize), "PathViz");

    Pathfinder* algo = showMenuAndChoose(grid, start, goal, window);
    if (!algo) return 0; // user closed window or Dijkstra not implemented

    bool running = false;
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space)
                    running = true;
            }
        }

        // Run one step of the algorithm every 50ms
        if (running && !algo->isFinished()) {
            if (clock.getElapsedTime().asMilliseconds() > 50) {
                algo->step();
                clock.restart();
            }
        }

        // Draw grid
        window.clear(sf::Color::White);
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                Node& node = grid.getNode(r, c);
                sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));
                cell.setPosition(c * cellSize, r * cellSize);

                if (&node == start)
                    cell.setFillColor(sf::Color::Green);
                else if (&node == goal)
                    cell.setFillColor(sf::Color::Red);
                else if (node.isWall)
                    cell.setFillColor(sf::Color::Black);
                else if (node.visited)
                    cell.setFillColor(sf::Color::Blue);
                else
                    cell.setFillColor(sf::Color::White);

                window.draw(cell);
            }
        }
        window.display();
    }

    delete algo;
    return 0;
}