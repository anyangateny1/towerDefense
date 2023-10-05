#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class gameObject {


    protected: 

    // coordinates for the object

    int x;
    int y;

    // health and damage

    int health;
    int damage;
    int id;

    public:
    
    gameObject() {}
    
    gameObject(int id, int x, int y, int health, int damage) : id(id), x(x), y(y), health(health), damage(damage) {}

    int getX() { return x; }
    
    int getY() { return y; }

    int getHealth() { return health; }

    int getDamage() { return damage; }

    void setHealth(int health) {

        this->health = health;

    }
};
#endif