#ifndef TOWER_H
#define TOWER_H
#include "Enemy.h"

class Tower {

    private:

    // intilizes attackrange, damage and attackSpeed using OOP principles. 

    int attackRange;

    int damage;

    float attackSpeed;
    
    public:

    // constructor

    Tower();
    
    // constructor

    Tower(int attackrange, int damage, float attackSpeed);

    // attacks the enemy, using pointers to reduce enemys hp by damage.

    void attack(Enemy* opponent);





    





};
#endif