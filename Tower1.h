#ifndef TOWER1_H
#define TOWER1_H
#include <cstdlib> // rand
#include "Tower.h"

class Tower1 : public Tower {
    private:
        bool splash;
        bool x1_5;

    public:
        Tower1(int id, int x, int y, int health, int damage, int attackRange, float attackSpeed):
        Tower(id, x, y, health, damage, attackRange, attackSpeed) {
            splash = false;
            x1_5 = false;
        }

        bool isSplash() {
            return splash;
        }

        bool isX1_5() {
            return x1_5;
        }
    
    //......























    
}
#endif