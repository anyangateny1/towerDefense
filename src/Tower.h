#ifndef TOWER_H
#define TOWER_H
#include "Enemy.h"
#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <cmath>

class Tower : public GameObject {
  private:
    int range;
    float attackCooldown;
    float attackTimer;
    sf::CircleShape rangeIndicator;
    sf::RectangleShape attackLine;
    bool isAttacking;
    GameObject* currentTarget;

  public:
    Tower()
        : GameObject(), range(2), attackCooldown(1.0f), attackTimer(0.0f), isAttacking(false),
          currentTarget(nullptr) {
        initializeVisuals();
    }

    Tower(int id, int x, int y, int health, int damage, int range, float attackCooldown)
        : GameObject(id, x, y, health, damage), range(range), attackCooldown(attackCooldown),
          attackTimer(0.0f), isAttacking(false), currentTarget(nullptr) {
        initializeVisuals();
    }

    void initializeVisuals() {
        const float cellSize = 80.0f;

        // Setup range indicator (red zone)
        rangeIndicator.setRadius(range * cellSize);
        rangeIndicator.setFillColor(sf::Color(255, 0, 0, 30));     // translucent red fill
        rangeIndicator.setOutlineColor(sf::Color(255, 0, 0, 120)); // stronger red outline
        rangeIndicator.setOutlineThickness(1.5f);

        // Setup attack line
        attackLine.setSize(sf::Vector2f(2.0f, cellSize));
        attackLine.setFillColor(sf::Color::Red);
    }

    void updatePosition() override {
        const float cellSize = 80.0f;
        if (texture) {
            // Convert from 1-indexed to 0-indexed for display
            sprite.setPosition(toZeroIndex(y) * cellSize + cellSize / 2 + 100,
                               toZeroIndex(x) * cellSize + cellSize / 2 + 100);

            // Update range indicator position
            rangeIndicator.setPosition(
                toZeroIndex(y) * cellSize + cellSize / 2 + 100 - range * cellSize,
                toZeroIndex(x) * cellSize + cellSize / 2 + 100 - range * cellSize);
        }
    }

    bool isInRange(const GameObject* target) const {
        if (!target)
            return false;

        // Calculate distance using 1-indexed coordinates
        int dx = target->getX() - x;
        int dy = target->getY() - y;
        return std::sqrt(dx * dx + dy * dy) <= range;
    }

    void attack(GameObject* target) {
        if (!target || !target->isAlive())
            return;

        if (attackTimer <= 0.0f && isInRange(target)) {
            target->takeDamage(damage);
            attackTimer = attackCooldown;
            isAttacking = true;
            currentTarget = target;
        }
    }

    bool isOnCooldown() const { return attackTimer > 0.0f; }

    void startCooldown() {
        attackTimer = attackCooldown;
        isAttacking = true;
    }

    void update(float deltaTime) {
        if (attackTimer > 0.0f) {
            attackTimer -= deltaTime;
            if (attackTimer <= 0.0f) {
                isAttacking = false;
                currentTarget = nullptr;
            }
        }
    }

    void draw(sf::RenderWindow& window) override {
        // Draw range indicator
        window.draw(rangeIndicator);

        // Draw the tower sprite
        if (texture) {
            window.draw(sprite);
        }

        if (isAttacking && currentTarget) {
            const float cellSize = 80.0f;

            sf::Vector2f start(toZeroIndex(y) * cellSize + cellSize / 2 + 100,
                               toZeroIndex(x) * cellSize + cellSize / 2 + 100);

            sf::Vector2f end(toZeroIndex(currentTarget->getY()) * cellSize + cellSize / 2 + 100,
                             toZeroIndex(currentTarget->getX()) * cellSize + cellSize / 2 + 100);

            float dx = end.x - start.x;
            float dy = end.y - start.y;
            float length = std::sqrt(dx * dx + dy * dy);
            float angle = std::atan2(dy, dx) * 180 / M_PI;

            // Update attack line
            attackLine.setSize(sf::Vector2f(length, 2.0f));
            attackLine.setPosition(start);
            attackLine.setRotation(angle);

            window.draw(attackLine);
        }
    }

    int getRange() const { return range; }
    void setRange(int newRange) {
        range = newRange;
        initializeVisuals();
    }
};
#endif
