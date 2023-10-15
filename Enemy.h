#ifndef ENEMY_H
#define ENEMY_H
#include "gameObject.h"
#include <iostream>

class Enemy : public gameObject {

protected:
    int speed;

public:
    // basic constructor

    Enemy() {}
    
    Enemy(int id, int x, int y, int health, int damage, int speed) : Enemy::gameObject(id, x, y, health, damage), speed(speed) {}

    //move enemy
    virtual void move(int newX, int newY){
        x = newX;
        y = newY;
    }

    //check if alive
     bool isAlive(){
        return (health > 0);
    }

    // basic function that makes the object take the damage of a towers HP

    virtual void takeDamage(int damage) {

        health -= damage;

    }


    // getters and setters

    virtual int getSpeed() {
        return speed;
    }

    //get coords
    virtual int getX(){
        return x;
    }

    virtual int getY(){
        return y;
    }

    virtual int getID(){
        return id;
    }

    virtual void setHealth(int hp){
        health = hp;
    }

    virtual void setSpeed(int speed){
        this->speed = speed;
    }

    virtual void setDamage(int dmg){
        damage = dmg;
    }


};
#endif