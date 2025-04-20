#include <iostream>
#include <ctime>
#include "Game.h"

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    
    Game game;
    
    std::cout << "Welcome to Tower Defense!" << std::endl;
    std::cout << "How many towers do you want to place? ";
    
    int towerCount;
    std::cin >> towerCount;
    
    // Limit the number of towers
    if (towerCount < 1) towerCount = 1;
    if (towerCount > 10) towerCount = 10;
    
    game.initGame(towerCount);
    game.startRound();
    
    return 0;
}
