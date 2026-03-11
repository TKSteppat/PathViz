#include <SFML/Graphics.hpp>
#include "../include/grid.h"
#include "../include/BFS.h"
#include "../include/DFS.h"
#include <iostream>

enum AlgoType {
    BFS_ALGO,
    DFS_ALGO
};

AlgoType showMenuAndChoose(sf::RenderWindow& window) {

    sf::Font font;
    font.loadFromFile("assets/fonts/sansation.ttf");

    sf::RectangleShape bfsButton(sf::Vector2f(200,50));
    bfsButton.setPosition(100,100);
    bfsButton.setFillColor(sf::Color::White);

    sf::RectangleShape dfsButton(sf::Vector2f(200,50));
    dfsButton.setPosition(100,200);
    dfsButton.setFillColor(sf::Color::White);

    sf::Text bfsText("BFS",font,24);
    bfsText.setFillColor(sf::Color::Black);
    bfsText.setPosition(160,110);

    sf::Text dfsText("DFS",font,24);
    dfsText.setFillColor(sf::Color::Black);
    dfsText.setPosition(160,210);

    while(window.isOpen()) {

        sf::Event event;

        while(window.pollEvent(event)) {

            if(event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::MouseButtonPressed) {

                sf::Vector2i mouse = sf::Mouse::getPosition(window);

                if(bfsButton.getGlobalBounds().contains(mouse.x,mouse.y))
                    return BFS_ALGO;

                if(dfsButton.getGlobalBounds().contains(mouse.x,mouse.y))
                    return DFS_ALGO;
            }
        }

        window.clear(sf::Color::Black);

        window.draw(bfsButton);
        window.draw(dfsButton);
        window.draw(bfsText);
        window.draw(dfsText);

        window.display();
    }

    return BFS_ALGO;
}

int main() {

    const int rows = 20;
    const int cols = 20;
    const int cellSize = 30;

    Grid grid(rows,cols);

    Node* start = &grid.getNode(0,0);
    Node* goal = &grid.getNode(rows-1,cols-1);

    sf::RenderWindow window(sf::VideoMode(cols*cellSize,rows*cellSize),"PathViz");

    AlgoType selectedAlgo = showMenuAndChoose(window);

    Pathfinder* algo = nullptr;

    bool running = false;
    bool draggingStart = false;
    bool draggingGoal = false;
    bool drawingWalls = false;

    sf::Clock clock;

    while(window.isOpen()) {

        sf::Event event;

        while(window.pollEvent(event)) {

            if(event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::KeyPressed) {

                if(event.key.code == sf::Keyboard::Space && !running) {

                    for(int r=0;r<rows;r++)
                        for(int c=0;c<cols;c++){
                            Node& node = grid.getNode(r,c);
                            node.visited = false;
                            node.parent = nullptr;
                        }

                    if(algo) delete algo;

                    if(selectedAlgo == BFS_ALGO)
                        algo = new BFS(grid,start,goal);

                    if(selectedAlgo == DFS_ALGO)
                        algo = new DFS(grid,start,goal);

                    running = true;
                }

                if(event.key.code == sf::Keyboard::R) {

                    running = false;

                    for(int r=0;r<rows;r++)
                        for(int c=0;c<cols;c++){
                            Node& node = grid.getNode(r,c);
                            node.visited = false;
                            node.parent = nullptr;
                            node.isWall = false;
                        }
                }
            }

            if(!running && event.type == sf::Event::MouseButtonPressed) {

                int col = event.mouseButton.x / cellSize;
                int row = event.mouseButton.y / cellSize;

                if(grid.isValid(row,col)) {

                    Node* clicked = &grid.getNode(row,col);

                    if(clicked == start)
                        draggingStart = true;

                    else if(clicked == goal)
                        draggingGoal = true;

                    else {

                        if(clicked != start && clicked != goal) {
                            clicked->isWall = !clicked->isWall;
                            drawingWalls = true;
                        }
                    }
                }
            }

            if(event.type == sf::Event::MouseButtonReleased) {

                draggingStart = false;
                draggingGoal = false;
                drawingWalls = false;
            }
        }

        if(!running && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

            sf::Vector2i mouse = sf::Mouse::getPosition(window);

            int col = mouse.x / cellSize;
            int row = mouse.y / cellSize;

            if(grid.isValid(row,col)) {

                Node* node = &grid.getNode(row,col);

                if(draggingStart) {

                    if(!node->isWall && node != goal)
                        start = node;
                }

                else if(draggingGoal) {

                    if(!node->isWall && node != start)
                        goal = node;
                }

                else if(drawingWalls) {

                    if(node != start && node != goal)
                        node->isWall = true;
                }
            }
        }

        if(running && algo && !algo->isFinished()) {

            if(clock.getElapsedTime().asMilliseconds() > 50) {
                algo->step();
                clock.restart();
            }
        }

        window.clear(sf::Color::White);

        for(int r=0;r<rows;r++)
            for(int c=0;c<cols;c++){

                Node& node = grid.getNode(r,c);

                sf::RectangleShape cell(sf::Vector2f(cellSize-1,cellSize-1));
                cell.setPosition(c*cellSize,r*cellSize);

                if(&node == start)
                    cell.setFillColor(sf::Color::Green);

                else if(&node == goal)
                    cell.setFillColor(sf::Color::Red);

                else if(node.isWall)
                    cell.setFillColor(sf::Color::Black);

                else if(node.visited)
                    cell.setFillColor(sf::Color::Blue);

                else
                    cell.setFillColor(sf::Color::White);

                window.draw(cell);
            }

        if(goal->parent != nullptr) {

            Node* pathNode = goal;

            while(pathNode && pathNode->parent) {

                pathNode = pathNode->parent;

                if(pathNode != start && !pathNode->isWall) {

                    sf::RectangleShape pathCell(sf::Vector2f(cellSize-1,cellSize-1));
                    pathCell.setPosition(pathNode->x*cellSize,pathNode->y*cellSize);
                    pathCell.setFillColor(sf::Color::Yellow);

                    window.draw(pathCell);
                }
            }
        }

        window.display();
    }

    delete algo;

    return 0;
}