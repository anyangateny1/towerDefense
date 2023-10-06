#ifndef GRID_H
#define GRID_H
#include "gameObject.h"

class Grid {

    protected:
    // constant integer for the colums and rows

    const int columns;
    const int rows;

    std::vector<std::vector<gameObject*> > gameMap;

    public:

    // vector of pointers that will host the gameObjects


    // this code block makes the vector 2d, and adds more object pointers relevant to the columns and rows

    Grid() : columns(7), rows(5){

        for (int i = 0; i < rows; i++){

        gameMap.push_back(std::vector<gameObject*>());
        
            for (int j = 0; j < columns; j++){

                gameMap[i].push_back(new gameObject());

            }

        }
    }

// simple function to print the current map.

void printGrid() {

        for (int i = 0; i < rows; i++){

            for (int j = 0; j < columns; j++){

                std::cout << gameMap[i][j]->getHealth();
            
            }

         std::cout << std::endl;

        }       

    }

// allows the object to be added.

void addObject(gameObject* object) {

    int x = object->getX();
    int y = object->getY();

    this->gameMap[x][y] = object;

    std::cout << "Object added!" << std::endl;

    };

void setHealth(int x, int y, int health){

    gameMap[x][y]->setHealth(health);

}


};
#endif