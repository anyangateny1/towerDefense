#ifndef GAME_H
#define GAME_H
#include <ctime>
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Tower.h"
#include "Enemy.h"
#include "Grid.h"

class Game {
private:
    // Time tracking
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    std::chrono::time_point<std::chrono::steady_clock> prevTime;
    std::chrono::time_point<std::chrono::steady_clock> currTime;
    
    // Object tracking
    int idCount;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<std::unique_ptr<Enemy>> enemies;
    
    // Game state
    int enemiesEscaped;
    int enemiesKilled;
    int roundDuration;
    int maxEscaped;
    bool gameOver;
    
    // Enemy spawning and limits
    float enemySpawnTimer;
    float enemySpawnInterval;
    const int maxEnemies = 20;        // Maximum enemies allowed on screen
    int totalEnemiesSpawned;          // Track total enemies spawned
    const int maxTotalEnemies = 50;   // Maximum enemies per round
    
    // Grid system
    Grid gameMap;
    
    // SFML components
    sf::RenderWindow window;
    sf::Font font;
    sf::Texture towerTexture;
    sf::Texture enemyTexture;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    
    // UI elements
    sf::Text scoreText;
    sf::Text timeText;
    sf::Text gameStateText;
    sf::Text waveText;
    
    // Helper methods
    void loadResources();
    void drawObjects();
    void handleEvents();
    void updateUI();
    void spawnEnemy();
    void updateTowers(float deltaTime);
    void cleanupEnemies();
    
public:
    Game();
    ~Game();
    
    void initGame(int amountOfTowers);
    void startRound();
    void moveEnemies();
    void attackEnemies();
    void checkWin();
    void run();
};
#endif
