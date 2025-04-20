#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <SFML/Graphics.hpp>

class GameObject {
protected:
    int id;
    int x;  // Now 1-indexed
    int y;  // Now 1-indexed
    int health;
    int damage;
    sf::Sprite sprite;
    sf::Texture* texture;
    const float cellSize = 80.0f;
    const float spriteScale = 0.7f; // Objects will be 70% of cell size

    // Convert from 1-indexed to 0-indexed for sprite positioning
    int toZeroIndex(int oneIndexed) const {
        return oneIndexed - 1;
    }

public:
    GameObject() : id(-1), x(1), y(1), health(0), damage(0), texture(nullptr) {}
    
    GameObject(int id, int x, int y, int health, int damage) 
        : id(id), x(x), y(y), health(health), damage(damage), texture(nullptr) {}
    
    virtual ~GameObject() {}
    
    int getId() const { return id; }
    virtual int getX() const { return x; }  // Returns 1-indexed x
    virtual int getY() const { return y; }  // Returns 1-indexed y
    virtual int getHealth() const { return health; }
    virtual int getDamage() const { return damage; }
    
    virtual bool isAlive() const { return health > 0; }
    
    virtual void takeDamage(int dmg) { health -= dmg; }
    virtual void setHealth(int newHealth) { health = newHealth; }
    virtual void setX(int newX) { x = newX; }  // Expects 1-indexed x
    virtual void setY(int newY) { 
        y = newY;  // Expects 1-indexed y
        updatePosition();
    }
    
    void setTexture(sf::Texture* newTexture) {
        texture = newTexture;
        if (texture) {
            sprite.setTexture(*texture);
            sprite.setOrigin(texture->getSize().x / 2.0f, texture->getSize().y / 2.0f);
            
            // Scale sprite to fit within cell
            float desiredSize = cellSize * spriteScale;
            float scale = desiredSize / texture->getSize().x;
            sprite.setScale(scale, scale);
            
            updatePosition();
        }
    }
    
    virtual void updatePosition() {
        // Position sprite at center of grid cell (convert from 1-indexed to 0-indexed for display)
        if (texture) {
            sprite.setPosition(
                toZeroIndex(y) * cellSize + cellSize/2 + 100,
                toZeroIndex(x) * cellSize + cellSize/2 + 100
            );
        }
    }
    
    virtual void draw(sf::RenderWindow& window) {
        if (isAlive() && texture) {
            window.draw(sprite);
        }
    }
};
#endif
