#include <SFML/Graphics.hpp>
#include "../include/Grid.h"
#include "../include/BFS.h"

int main() {
    const int rows = 20;
    const int cols = 20;
    const int cellSize = 30;

    Grid grid(rows, cols);

    Node* start = &grid.getNode(0, 0);
    Node* goal = &grid.getNode(rows - 1, cols - 1);

    BFS bfs(grid, start, goal);

    sf::RenderWindow window(
        sf::VideoMode(cols * cellSize, rows * cellSize),
        "PathViz - BFS"
    );

    bool running = false;

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

        static sf::Clock clock;

    if (running && !bfs.isFinished()) {
        if (clock.getElapsedTime().asMilliseconds() > 50) {
            bfs.step();
            clock.restart();
        }
    }

        window.clear(sf::Color::White);

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                Node& node = grid.getNode(r, c);

                sf::RectangleShape cell(
                    sf::Vector2f(cellSize - 1, cellSize - 1)
                );

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

    return 0;
}