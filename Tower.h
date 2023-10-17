#ifndef TOWER_H
#define TOWER_H
#include "gameObject.h"
#include "Enemy.h"

class Tower : public gameObject {

    private:

    // intilizes attackrange, damage and attackSpeed using OOP principles. 

    int attackRange;

    float attackSpeed;

    public:

    // constructor

    Tower() {}

    Tower(int id, int x, int y, int health, int damage, int attackRange, float attackSpeed) 
    : Tower::gameObject(id, x, y, health, damage), attackRange(attackRange), attackSpeed(attackSpeed) {}

    // attacks the enemy, using pointers to reduce enemys hp by damage.

    void attack(Enemy* opponent) {
    if (opponent->getHealth() > 0) {
        opponent->takeDamage(damage);
    }
    }

    // Checks whether enemies in range or not

    bool inRange(Enemy* opponent) {
        return attackRange * attackRange >=
         (x - opponent->getX(opponent)) * (x - opponent->getX(opponent)) 
         + (y - opponent->getY(opponent)) * (y - opponent->getY(opponent)) ? true : false;
    }

};
#endif