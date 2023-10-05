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

    Tower(int id, int x, int y, int health, int damage, int attackRange, float attackSpeed);

    // attacks the enemy, using pointers to reduce enemys hp by damage.

    void attack(Enemy* opponent);
    





    





};
#endif