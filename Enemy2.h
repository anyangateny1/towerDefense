#ifndef ENEMY2_H
#define ENEMY2_H
#include "Enemy.h"
// put timer value
//if statement if timer > int then health++
//original health value
class Enemy2 : public Enemy {
private:
    bool regrowth;
    int originalHP;
    int timer = 0;

public:
    Enemy2(int id, int x, int y, int health, int damage, int speed, bool regrowth): 
    Enemy(id, x, y, health, damage, speed), regrowth(regrowth), originalHP(health);

    void activateRegrowth(){
        if (regrowth && health < originalHP && timer > 5){
            health++;
        }

    }
}










#endif