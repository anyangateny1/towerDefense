#ifndef GRID_H
#define GRID_H
#include "Enemy.h"
#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

class Grid {
  private:
    const int rows;
    const int columns;
    std::vector<int> objectIds;
    std::vector<std::vector<GameObject*>> grid;
    sf::RectangleShape cellOutline;
    const float cellSize;
    std::vector<std::pair<int, int>> path; // Path for enemies to follow

    int toZeroIndex(int oneIndexed) const { return oneIndexed - 1; }

    // Convert from 0-indexed to 1-indexed coordinates
    int toOneIndex(int zeroIndexed) const { return zeroIndexed + 1; }

    bool isValidPosition(int x, int y) const {
        // Convert to 0-indexed for internal check
        int x0 = toZeroIndex(x);
        int y0 = toZeroIndex(y);
        return x0 >= 0 && x0 < rows && y0 >= 0 && y0 < columns;
    }

  public:
    Grid() : rows(5), columns(7), cellSize(80.0f) {
        // Initialize grid with nullptr
        grid.resize(rows, std::vector<GameObject*>(columns, nullptr));

        cellOutline.setSize(sf::Vector2f(cellSize, cellSize));
        cellOutline.setFillColor(sf::Color::Transparent);
        cellOutline.setOutlineColor(sf::Color(100, 100, 100, 150));
        cellOutline.setOutlineThickness(1.0f);

        path = {{3, 1}, {3, 2}, {2, 2}, {2, 3}, {3, 3}, {3, 4},
                {4, 4}, {4, 5}, {3, 5}, {3, 6}, {2, 6}, {2, 7}};
    }

    int getRows() const { return rows; }
    int getColumns() const { return columns; }

    void printGrid() const {
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= columns; j++) {
                if (grid[toZeroIndex(i)][toZeroIndex(j)] &&
                    grid[toZeroIndex(i)][toZeroIndex(j)]->isAlive()) {
                    std::cout << grid[toZeroIndex(i)][toZeroIndex(j)]->getHealth();
                } else {
                    std::cout << "0";
                }
            }
            std::cout << std::endl;
        }
    }

    void addObject(GameObject* object) {
        if (!object)
            return;

        // Get 1-indexed coordinates from object
        int x = object->getX();
        int y = object->getY();

        // Check if coordinates are valid
        if (!isValidPosition(x, y)) {
            std::cerr << "Invalid position (" << x << "," << y << ")!" << std::endl;
            return;
        }

        int x0 = toZeroIndex(x);
        int y0 = toZeroIndex(y);

        // Check if id is already used
        int id = object->getId();
        if (std::find(objectIds.begin(), objectIds.end(), id) != objectIds.end()) {
            std::cerr << "ID " << id << " already in use!" << std::endl;
            return;
        }

        // Remove any existing object at this position
        if (grid[x0][y0]) {
            removeObject(grid[x0][y0]);
        }

        grid[x0][y0] = object;
        objectIds.push_back(id);
    }

    GameObject* getObjectAt(int x, int y) const {
        if (!isValidPosition(x, y))
            return nullptr;
        return grid[toZeroIndex(x)][toZeroIndex(y)];
    }

    void updateGrid() {
        std::vector<std::vector<GameObject*>> newGrid(rows,
                                                      std::vector<GameObject*>(columns, nullptr));

        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= columns; j++) {
                int i0 = toZeroIndex(i);
                int j0 = toZeroIndex(j);
                if (grid[i0][j0] && !dynamic_cast<Enemy*>(grid[i0][j0])) {
                    newGrid[i0][j0] = grid[i0][j0];
                }
            }
        }

        // Then move each enemy along the path
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= columns; j++) {
                int i0 = toZeroIndex(i);
                int j0 = toZeroIndex(j);
                if (!grid[i0][j0])
                    continue;

                Enemy* enemy = dynamic_cast<Enemy*>(grid[i0][j0]);
                if (!enemy || !enemy->isAlive())
                    continue;

                // Find current position in path
                auto currentPos = std::make_pair(i, j);
                auto it = std::find(path.begin(), path.end(), currentPos);

                if (it != path.end() && std::next(it) != path.end()) {
                    // Get next position in path
                    auto nextPos = *std::next(it);

                    // Move enemy to next position (using 1-indexed coordinates)
                    enemy->move(nextPos.first, nextPos.second);
                    newGrid[toZeroIndex(nextPos.first)][toZeroIndex(nextPos.second)] = enemy;
                }
            }
        }

        // Update the grid
        grid = std::move(newGrid);
    }

    const std::vector<std::pair<int, int>>& getPath() const { return path; }

    void draw(sf::RenderWindow& window) {
        if (!window.isOpen())
            return;

        // Draw grid cells
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= columns; j++) {
                int i0 = toZeroIndex(i);
                int j0 = toZeroIndex(j);
                cellOutline.setPosition(j0 * cellSize + 100, i0 * cellSize + 100);

                if (std::find(path.begin(), path.end(), std::make_pair(i, j)) != path.end()) {
                    cellOutline.setFillColor(sf::Color(0, 0, 0, 160));
                } else {
                    cellOutline.setFillColor(sf::Color(0, 0, 0, 110));
                }

                window.draw(cellOutline);
            }
        }
    }

    void removeObject(GameObject* object) {
        if (!object)
            return;

        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= columns; j++) {
                int i0 = toZeroIndex(i);
                int j0 = toZeroIndex(j);
                if (grid[i0][j0] == object) {
                    grid[i0][j0] = nullptr;

                    // Remove the object's ID from objectIds
                    auto it = std::find(objectIds.begin(), objectIds.end(), object->getId());
                    if (it != objectIds.end()) {
                        objectIds.erase(it);
                    }
                    return;
                }
            }
        }
    }
};
#endif
