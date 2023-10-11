#include <iostream>
#include "Tower.h"
#include "Enemy.h"
#include "Grid.h"

int main() {

Grid Map;

Enemy John(1, 2, 3, 1, 1, 5);

Tower Anyang(1,1,2,1,1,2,2);

Map.addObject(&John);

Map.addObject(&Anyang);

Anyang.setHealth(2);

Map.printGrid();

std::cout << std::endl;

std::cout << std::endl;

Anyang.attack(&John);

Map.printGrid();

return 0;

}