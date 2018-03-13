#pragma once

#include "Action.h"
#include "util.h"

class GameState {
public:
    static constexpr unsigned NUM_BULLETS = 6;
    static constexpr unsigned NUM_VALUES = 2 + 3*NUM_BULLETS;
    
    double values[NUM_VALUES] = {0};
    
    double& playerX() { return values[0]; }
    double& playerY() { return values[1]; }
    double& bulletX(int n) { return values[2+n*3]; }
    double& bulletY(int n) { return values[3+n*3]; }
    double& bulletDirection(int n) { return values[4+n*3]; }

    unsigned short currentBullet = 0;
    unsigned short fireCycle = 0;
};