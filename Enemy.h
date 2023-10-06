#ifndef ENEMY_H
#define ENEMY_H
#include "gameObject.h"
#include <iostream>

class Enemy : public gameObject {

private:

int speed;


public:

// basic constructor

Enemy(int id, int x, int y, int health, int damage, int speed) : Enemy::gameObject(id, x, y, health, damage), speed(speed) {}


// basic function that makes the object take the damage of a towers HP

void takeDamage(int damage) {

    health -= damage;

    if (health < 1) {

        std::cout << "Dead!" << std::endl;

    }

}


// getters and setters

int getSpeed() {

    return speed;

}

};
#endif