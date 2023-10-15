#include <iostream>
#include "Tower.h"
#include "Enemy.h"
#include "Grid.h"
#include <ctime>
#include "Game.h"

int main() {

Game Game;

Game.initGame(7);

Game.startRound();

Game.destroy();

}