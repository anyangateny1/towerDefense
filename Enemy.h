#ifndef ENEMY_H
#define ENEMY_H
#include "GameObject.h"
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

class Enemy : public GameObject {
protected:
    int speed;
    sf::Clock animationClock;
    bool isMoving;
    const float maxHealth;
    
public:
    // Basic constructors - maintaining original functionality
    Enemy() : GameObject(), speed(0), isMoving(false), maxHealth(100.0f) {}
    
    Enemy(int id, int x, int y, int health, int damage, int speed) 
        : GameObject(id, x, y, health, damage), speed(speed), isMoving(false), maxHealth(health) {}
    
    // Move enemy
    virtual void move(int newX, int newY) {
        if (newX == x && newY == y) return;
        
        isMoving = true;
        x = newX;  // Already 1-indexed from GameObject
        y = newY;  // Already 1-indexed from GameObject
        updatePosition();
    }
    
    // Take damage with visual feedback
    virtual void takeDamage(int dmg) override {
        if (!isAlive()) return;
        
        health = std::max(0, health - dmg);
        
        // Visual feedback - flash red when hit
        if (texture) {
            sprite.setColor(sf::Color::Red);
        }
    }
    
    // Getters and setters
    virtual int getSpeed() const {
        return speed;
    }
    
    virtual void setSpeed(int newSpeed) {
        this->speed = newSpeed;
    }
    
    void updatePosition() override {
        if (!texture) return;
        
        const float cellSize = 80.0f;
        // Position sprite at center of grid cell with slight animation
        float offsetY = isMoving ? sin(animationClock.getElapsedTime().asSeconds() * 4) * 5 : 0;
        
        // Convert from 1-indexed to 0-indexed for display
        sprite.setPosition(
            toZeroIndex(y) * cellSize + cellSize/2 + 100, 
            toZeroIndex(x) * cellSize + cellSize/2 + 100 + offsetY
        );
    }
    
    void draw(sf::RenderWindow& window) override {
        if (!isAlive() || !texture) return;
        
        // Reset color gradually
        sf::Color currentColor = sprite.getColor();
        if (currentColor != sf::Color::White) {
            currentColor.r = std::min(255, currentColor.r + 5);
            currentColor.g = std::min(255, currentColor.g + 5);
            currentColor.b = std::min(255, currentColor.b + 5);
            sprite.setColor(currentColor);
        }
        
        // Draw the sprite
        window.draw(sprite);
        
        // Draw health bar only if enemy has taken damage
        if (health < maxHealth) {
            const float cellSize = 80.0f;
            sf::RectangleShape healthBar;
            
            // Calculate health bar size and position (using 0-indexed coordinates for display)
            float barWidth = cellSize * 0.8f;
            float barHeight = 5.0f;
            float barX = toZeroIndex(y) * cellSize + cellSize * 0.1f + 100;
            float barY = toZeroIndex(x) * cellSize + 80 + 100;
            
            // Draw background (gray)
            healthBar.setSize(sf::Vector2f(barWidth, barHeight));
            healthBar.setPosition(barX, barY);
            healthBar.setFillColor(sf::Color(100, 100, 100));
            window.draw(healthBar);
            
            // Draw health (green to red based on health percentage)
            float healthPercentage = std::max(0.0f, std::min(1.0f, static_cast<float>(health) / maxHealth));
            
            healthBar.setSize(sf::Vector2f(barWidth * healthPercentage, barHeight));
            
            // Color ranges from green (full health) to red (low health)
            int greenComponent = static_cast<int>(255 * healthPercentage);
            int redComponent = static_cast<int>(255 * (1.0f - healthPercentage));
            healthBar.setFillColor(sf::Color(redComponent, greenComponent, 0));
            
            window.draw(healthBar);
        }
    }
};
#endif
