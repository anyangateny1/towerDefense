#ifndef GRID_H
#define GRID_H
#include "gameObject.h"
#include <algorithm>

class Grid {

    protected:
    // constant integer for the colums and rows

    const int columns;
    const int rows;

    // vector to keep track of IDs inputted.
    
    std::vector<int> idVect;
    
    // vector of pointers that will host the gameObjects

    
    // count

    int count;


    public:   

    std::vector<std::vector<gameObject*> > gameMap;


    // this code block makes the vector 2d, and adds more object pointers relevant to the columns and rows

    Grid() : columns(7), rows(5), count(0){

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

    // this also keeps tracks of IDs and pushbacks into previously named vector

    if (idVect.empty() || !(!idVect.empty() && std::find(idVect.begin(), idVect.end(), object->checkID()) != idVect.end())){

        this->gameMap[x][y] = object;

        std::cout << "Object added!" << std::endl;

        idVect.push_back(object->checkID());

    } else {

        std::cout << "ID already in use! Please try again with a different ID." << std::endl;
    }

    };

void setHealth(int x, int y, int health){

    gameMap[x][y]->setHealth(health);

}

void updateGrid(){

    // shifts all the enemy 1 place to the right.
    
    for (int i = 6; i > 0; i--) {
        
        gameMap[2][i] = gameMap[2][i-1];
        gameMap[2][i]->setY(i-1);

    }  
}

};
#endif