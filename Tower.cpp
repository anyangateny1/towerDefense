#include "Tower.h"

// init variables and tower

Tower::Tower(int id, int x, int y, int health, int damage, int attackRange, float attackSpeed) : Tower::gameObject(id, x, y, health, damage), attackRange(attackRange), attackSpeed(attackSpeed) {}

// attacks the enemy

void Tower::attack(Enemy* opponent) {
  if (attackRange > 1) {
    opponent->takeDamage(damage);
    }
}
