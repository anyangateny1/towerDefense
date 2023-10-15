#ifndef GAME_H
#define GAME_H
#include <ctime>
#include <iostream>
#include "Tower.h"
#include "Enemy.h"
#include "Grid.h"

class Game {

private:
// data memebers to keep track of time.

time_t startTimeSeconds;

std::chrono::time_point<std::chrono::steady_clock> prevTime;

std::chrono::time_point<std::chrono::steady_clock> currTime;

// data memers to keep track of counts, and appropiately hand an ID to an object.

int idCount;

int towerCount;

int enemyCount;

// initialization of dynamic array of enemy and tower objects

Enemy* enemyObjects;

Tower* towerObjects;

// win condition

int winCondition;

// adds an object of Grid.

Grid Map;

public:

// init game instance with everything set to 0

Game() : idCount(0), towerCount(0), enemyCount(0), winCondition(0){   

    // adjusted to 100 to prevent seg faults as the game does not recognise when enemyObjets is filled up.

    // ADD A WAY FOR THE GAME OT RECOGNISE WHEN 100 IS BEING APPROACHED!

    enemyObjects = new Enemy[100];
    towerObjects = new Tower[100];

}

void initGame(int amountOfTowers){  
    
    // function that repeats based on how many towers the user wants in the first round.

    for (int i = 0; i < amountOfTowers; i++) {
        
        int x,y;

        std::cout << "Please enter location of Tower " << i+1 << ": " << std::endl;

        std::cin >> x;

        std::cin >> y;

        towerObjects[towerCount] = Tower(idCount, x, y, 1, 1, 1, 1);

        Map.addObject(&towerObjects[towerCount]);

        towerCount++;

        idCount++;

    }  

    Map.printGrid(); 

}

// function that starts the round

void startRound() {

// changes time variables.

// start time is time the function was called.

startTimeSeconds = time(NULL);

// previous time is alos the time the function was called.

prevTime = std::chrono::steady_clock::now();

// adds first enemy.

enemyObjects[enemyCount] = Enemy(idCount, 2, 0, (rand() % 9)+4, 1, 1);

Map.addObject(&enemyObjects[enemyCount]);

enemyCount++;

idCount++;

// makes the round last 20 seconds, or until the winConditioned is satisfied.

while (time(NULL) - startTimeSeconds < 20 && winCondition < 5) {

    currTime = std::chrono::steady_clock::now();

    // this function allows the game to update every 1 second.

    if (std::chrono::duration_cast<std::chrono::seconds>(currTime - prevTime).count() >= 1) {

        checkWin();

        this->moveEnemies();

        this->attackEnemies();


    }

}

// print script depending on if the win condition is satisifed or not.

if (winCondition >= 5) {

           std::cout << "Enemies win. " << winCondition << " Enemies managed to make it through!" << std::endl;

       } else {

           std::cout << "Towers win. Only " << winCondition << " Enemies managed to make it through!" << std::endl;

}

}

// deletes and frees the enemy and tower objects.

void destroy() {

    delete[] enemyObjects;

    delete[] towerObjects;

}

// uses functions form grid.h to move the enemies.

int moveEnemies(){

    Map.printGrid();

    Map.updateGrid();

    int i = 0;
    
    // adds another enemy at 2,0 when the other enemies move to the right.

    enemyObjects[enemyCount] = Enemy(idCount, 2, i, (rand() % 4)+1, 1, 1);

    Map.addObject(&enemyObjects[enemyCount]);

    enemyCount++;

    i++;

    idCount++;

    prevTime = currTime;

    return 0;

}

void attackEnemies(){

    // if the enemy passes the direct path of a tower, the tower will attack the enemy.

    for (int i = 0; i < towerCount; i++){

        for (int j = 0; j < enemyCount; j++) {

            // checks if the tower and the enemy are directly in front of each other.

            if ((towerObjects[i].getX() - enemyObjects[j].getX() == 1 || -1) && (towerObjects[i].getY() - enemyObjects[j].getY() == 0)) {

                towerObjects[i].attack(&enemyObjects[j]);

                std::cout << "Tower: " << i+1 << " attacked Enemy: " << j+1 << std::endl;

                if (!(enemyObjects[j].isAlive())){
                   
                    // if the enmy dies, it will be replaced with an enemy object that does nothing.

                    enemyObjects[j] = Enemy(0,2,j,0,0,0);

                }

            }

        }

    }
}

// simple function to count how many times an enemy is able to get through the maze.

void checkWin(){
            
    if (Map.gameMap[2][6]->getHealth() > 0){
            
        winCondition++;

    }

}



};
#endif