#ifndef ENEMY1_H
#define ENEMY1_H
#include "Enemy.h"
#include <iostream>

class Enemy1 : public Enemy {
private:
    bool camo;

public:
    Enemy1(int id, int x, int y, int health, int damage, int speed, bool camoValue): Enemy(id, x, y, health, damage, speed), camo(camoValue);

    bool isCamo(){
        return camo;
    }

}














































#endif