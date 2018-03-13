#pragma once

#include "Action.h"
#include "util.h"

class GameState {
public:
    static constexpr unsigned NUM_BULLETS = 6;
    static constexpr unsigned NUM_PLAYER_VALUES = 4;
    static constexpr unsigned NUM_VALUES = NUM_PLAYER_VALUES + 3*NUM_BULLETS;
    
    struct bullet {
        double x, y, direction;
    };
    
    double playerX, playerY;
    double playerVx, playerVy;
    bullet bullets[NUM_BULLETS];

    unsigned short currentBullet = 0;
    unsigned short fireCycle = 0;
};