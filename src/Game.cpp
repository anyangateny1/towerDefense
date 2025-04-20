#include "Game.h"

Game::Game() : 
    idCount(0), 
    enemiesEscaped(0),
    enemiesKilled(0),
    roundDuration(60),
    maxEscaped(10),
    gameOver(false),
    enemySpawnTimer(0.0f),
    enemySpawnInterval(2.0f),
    totalEnemiesSpawned(0),
    window(sf::VideoMode(800, 600), "Tower Defense Game") {
    
    loadResources();
    startTime = std::chrono::steady_clock::now();
    prevTime = std::chrono::steady_clock::now();
}

Game::~Game() {
    // Resources are automatically cleaned up by vectors and SFML
}

void Game::loadResources() {
    // Load font
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }
    
    // Load textures
    if (!towerTexture.loadFromFile("assets/textures/tower.png")) {
        std::cerr << "Failed to load tower texture!" << std::endl;
    }
    
    if (!enemyTexture.loadFromFile("assets/textures/enemy.png")) {
        std::cerr << "Failed to load enemy texture!" << std::endl;
    }
    
    if (!backgroundTexture.loadFromFile("assets/textures/background.png")) {
        std::cerr << "Failed to load background texture!" << std::endl;
    }
    
    // Setup sprites and scale them
    backgroundSprite.setTexture(backgroundTexture);
    
    // Scale background to fit window
    float scaleX = 800.0f / backgroundTexture.getSize().x;
    float scaleY = 600.0f / backgroundTexture.getSize().y;
    backgroundSprite.setScale(scaleX, scaleY);
    
    // Scale game object textures
    const float cellSize = 80.0f;
    const float desiredSize = cellSize * 0.7f; // Make objects slightly smaller than cell
    
    // Scale tower texture
    float towerScale = desiredSize / towerTexture.getSize().x;
    towerTexture.setSmooth(true); // Enable smooth scaling
    
    // Scale enemy texture
    float enemyScale = desiredSize / enemyTexture.getSize().x;
    enemyTexture.setSmooth(true); // Enable smooth scaling
    
    // Setup text elements
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);
    
    timeText.setFont(font);
    timeText.setCharacterSize(24);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(10, 40);
    
    gameStateText.setFont(font);
    gameStateText.setCharacterSize(36);
    gameStateText.setFillColor(sf::Color::Red);
    // Center the game state text
    gameStateText.setPosition(400, 300);
    gameStateText.setOrigin(gameStateText.getLocalBounds().width / 2, 
                           gameStateText.getLocalBounds().height / 2);
    
    // Setup wave text
    waveText.setFont(font);
    waveText.setCharacterSize(24);
    waveText.setFillColor(sf::Color::White);
    waveText.setPosition(10, 70);
}

void Game::initGame(int amountOfTowers) {
    window.clear();
    window.draw(backgroundSprite);
    
    // Show the path
    gameMap.draw(window);
    window.display();
    
    std::cout << "Place your towers! Avoid placing them on the enemy path." << std::endl;
    std::cout << "Note: Grid positions are 1-indexed (1-" << gameMap.getRows() << " for rows, 1-" << gameMap.getColumns() << " for columns)" << std::endl;
    
    for (int i = 0; i < amountOfTowers; i++) {
        int x, y;
        bool validPosition = false;
        
        while (!validPosition) {
            std::cout << "Enter location for Tower " << i+1 << " (x y): ";
            std::cin >> x >> y;
            
            // Coordinates are now 1-indexed, so we adjust the bounds check
            if (x < 1 || x > gameMap.getRows() || y < 1 || y > gameMap.getColumns()) {
                std::cout << "Position out of bounds! Please enter values between 1-" 
                         << gameMap.getRows() << " for x and 1-" 
                         << gameMap.getColumns() << " for y." << std::endl;
                continue;
            }
            
            // Check if position is on the path
            auto& path = gameMap.getPath();
            if (std::find(path.begin(), path.end(), std::make_pair(x, y)) != path.end()) {
                std::cout << "Cannot place tower on the enemy path! Try again." << std::endl;
                continue;
            }
            
            // Check if position is already occupied
            if (gameMap.getObjectAt(x, y) != nullptr) {
                std::cout << "Position already occupied! Try again." << std::endl;
                continue;
            }
            
            validPosition = true;
        }
        
        // Create tower with balanced stats (coordinates are now 1-indexed)
        auto tower = std::make_unique<Tower>(
            idCount++,    // id
            x, y,        // position (1-indexed)
            100,         // health
            10,          // damage
            2,          // range
            0.8f        // attack cooldown
        );
        tower->setTexture(&towerTexture);
        
        gameMap.addObject(tower.get());
        towers.push_back(std::move(tower));
    }
}

void Game::startRound() {
    startTime = std::chrono::steady_clock::now();
    prevTime = std::chrono::steady_clock::now();
    run();
}

void Game::run() {
    sf::Clock deltaClock;
    float moveTimer = 0.0f;
    const float moveInterval = 1.0f; // Move every 1 second
    
    while (window.isOpen() && !gameOver) {
        float deltaTime = deltaClock.restart().asSeconds();
        
        handleEvents();
        
        currTime = std::chrono::steady_clock::now();
        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(currTime - startTime).count();
        
        // Update game state
        enemySpawnTimer += deltaTime;
        moveTimer += deltaTime;
        
        // Spawn enemies at regular intervals if we haven't reached the limit
        if (enemySpawnTimer >= enemySpawnInterval && totalEnemiesSpawned < maxTotalEnemies) {
            spawnEnemy();
            enemySpawnTimer = 0.0f;
        }
        
        // Move enemies at fixed intervals
        if (moveTimer >= moveInterval) {
            moveEnemies();
            moveTimer = 0.0f;
        }
        
        updateTowers(deltaTime);
        attackEnemies();
        checkWin();
        
        // Check for round end conditions
        if (elapsedSeconds >= roundDuration || 
            (totalEnemiesSpawned >= maxTotalEnemies && enemies.empty())) {
            
            if (enemiesEscaped < maxEscaped) {
                gameStateText.setString("Victory! Enemies Killed: " + std::to_string(enemiesKilled));
            } else {
                gameStateText.setString("Defeat! Too many enemies escaped!");
            }
            gameOver = true;
        }
        
        updateUI();
        drawObjects();
    }
    
    // Display final game state
    if (gameOver) {
        sf::Clock endTimer;
        while (endTimer.getElapsedTime().asSeconds() < 3.0f) {
            window.clear();
            window.draw(backgroundSprite);
            window.draw(gameStateText);
            window.display();
        }
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void Game::spawnEnemy() {
    // Check enemy limits
    if (enemies.size() >= maxEnemies || totalEnemiesSpawned >= maxTotalEnemies) {
        return;
    }
    
    // Calculate base health scaling with number of towers
    int baseHealth = 80 + (towers.size() * 30);
    
    // Add randomization to make enemies more varied
    int healthVariation = baseHealth * 0.2;  // 20% variation
    int finalHealth = baseHealth + (rand() % healthVariation) - (healthVariation / 2);
    
    // Scale health up as the game progresses
    float progressionScale = 1.0f + (static_cast<float>(totalEnemiesSpawned) / maxTotalEnemies * 0.3f);
    finalHealth = static_cast<int>(finalHealth * progressionScale);
    
    // Create enemy with scaled stats (using 1-indexed coordinates)
    auto enemy = std::make_unique<Enemy>(
        idCount++,    // id
        3, 1,        // starting position (top of path, now 1-indexed)
        finalHealth,  // scaled health
        10 + (rand() % 5),  // damage (10-14)
        1            // speed (fixed to 1 for better control)
    );
    
    if (enemy) {
        enemy->setTexture(&enemyTexture);
        gameMap.addObject(enemy.get());
        enemies.push_back(std::move(enemy));
        totalEnemiesSpawned++;
    }
}

void Game::cleanupEnemies() {
    // First, mark enemies for removal
    std::vector<std::unique_ptr<Enemy>> survivingEnemies;
    survivingEnemies.reserve(enemies.size());

    for (auto& enemy : enemies) {
        if (!enemy) continue;

        bool isDead = !enemy->isAlive();
        bool isEscaped = enemy->getY() >= gameMap.getColumns() - 1;

        if (!isDead && !isEscaped) {
            // Enemy is still alive and in bounds
            survivingEnemies.push_back(std::move(enemy));
        } else {
            // Enemy is dead or out of bounds
            gameMap.removeObject(enemy.get());
            if (isDead) {
                enemiesKilled++;
            } else if (isEscaped) {
                enemiesEscaped++;
            }
        }
    }

    // Replace old vector with surviving enemies
    enemies = std::move(survivingEnemies);
}

void Game::moveEnemies() {
    // First update grid positions
    gameMap.updateGrid();
    
    // Then clean up dead or escaped enemies
    cleanupEnemies();
}

void Game::updateTowers(float deltaTime) {
    for (auto& tower : towers) {
        tower->update(deltaTime);
    }
}

void Game::attackEnemies() {
    // First, find all tower targets and calculate total damage
    struct EnemyDamage {
        Enemy* enemy;
        int totalDamage;
        EnemyDamage(Enemy* e) : enemy(e), totalDamage(0) {}
    };
    std::vector<EnemyDamage> enemyDamages;

    // First pass: collect all damage that will be dealt
    for (const auto& tower : towers) {
        if (!tower || !tower->isAlive() || tower->isOnCooldown()) continue;

        // Find closest enemy in range
        Enemy* closestEnemy = nullptr;
        float closestDistance = std::numeric_limits<float>::max();
        
        for (const auto& enemy : enemies) {
            if (!enemy || !enemy->isAlive()) continue;
            
            int dx = enemy->getX() - tower->getX();
            int dy = enemy->getY() - tower->getY();
            float distance = std::sqrt(dx*dx + dy*dy);
            
            if (distance < closestDistance && tower->isInRange(enemy.get())) {
                closestDistance = distance;
                closestEnemy = enemy.get();
            }
        }
        
        if (closestEnemy) {
            // Find or create damage entry for this enemy
            auto it = std::find_if(enemyDamages.begin(), enemyDamages.end(),
                [closestEnemy](const EnemyDamage& ed) { return ed.enemy == closestEnemy; });
            
            if (it == enemyDamages.end()) {
                enemyDamages.emplace_back(closestEnemy);
                it = enemyDamages.end() - 1;
            }
            
            // Add tower's damage and start its cooldown
            it->totalDamage += tower->getDamage();
            tower->startCooldown();
        }
    }

    // Second pass: apply all damage at once
    for (const auto& ed : enemyDamages) {
        if (ed.enemy && ed.enemy->isAlive()) {
            ed.enemy->takeDamage(ed.totalDamage);
        }
    }
}

void Game::checkWin() {
    if (enemiesEscaped >= maxEscaped) {
        gameStateText.setString("Defeat! Too many enemies escaped!");
        gameOver = true;
    }
}

void Game::updateUI() {
    auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(currTime - startTime).count();
    int timeRemaining = roundDuration - elapsedSeconds;
    
    // Update score text
    std::string scoreString = "Enemies: Killed " + std::to_string(enemiesKilled) + 
        " / Escaped " + std::to_string(enemiesEscaped) + 
        " / Max " + std::to_string(maxEscaped);
    scoreText.setString(scoreString);
    
    // Update time text
    timeText.setString("Time: " + std::to_string(timeRemaining) + "s");
    
    // Update wave text with more info
    std::string waveString = "Wave Progress: " + std::to_string(totalEnemiesSpawned) + 
        "/" + std::to_string(maxTotalEnemies) + 
        " (Active: " + std::to_string(enemies.size()) + ")";
    waveText.setString(waveString);
    
    // Center the game state text when it changes
    if (gameOver) {
        gameStateText.setOrigin(gameStateText.getLocalBounds().width / 2, 
                               gameStateText.getLocalBounds().height / 2);
        gameStateText.setPosition(400, 300);
    }
}

void Game::drawObjects() {
    if (!window.isOpen()) return;
    
    window.clear();
    window.draw(backgroundSprite);
    
    // Draw grid cells
    gameMap.draw(window);
    
    // Draw towers
    for (const auto& tower : towers) {
        if (tower && tower->isAlive()) {
            tower->draw(window);
        }
    }
    
    // Draw enemies
    for (const auto& enemy : enemies) {
        if (enemy && enemy->isAlive()) {
            enemy->draw(window);
        }
    }
    
    // Draw UI elements
    window.draw(scoreText);
    window.draw(timeText);
    window.draw(waveText);
    
    if (gameOver) {
        // Add a semi-transparent background for better text visibility
        sf::RectangleShape overlay(sf::Vector2f(800, 600));
        overlay.setFillColor(sf::Color(0, 0, 0, 128));
        window.draw(overlay);
        window.draw(gameStateText);
    }
    
    window.display();
}
