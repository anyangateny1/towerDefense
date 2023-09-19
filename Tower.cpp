#include "Tower.h"

// constructor that does nothing

Tower::Tower() {}

// init variables and tower

Tower::Tower(int attackrange, int damage, float attackSpeed) : attackRange(attackRange), damage(damage), attackSpeed(attackSpeed) {}

// attacks the enemy

void Tower::attack(Enemy* opponent) {
    
    opponent->takeDamage(damage);

}
