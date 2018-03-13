#pragma once

#include "Action.h"
#include "util.h"

class GameState {
public:
    static constexpr unsigned NUM_BULLETS = 2;
    static constexpr unsigned NUM_PLAYER_VALUES = 4;
    static constexpr unsigned NUM_VALUES = NUM_PLAYER_VALUES + 3*NUM_BULLETS;
    
    struct bullet {
        double x=0, y=0, direction=0;
    };
    
    double playerX = 0, playerY = 0;
    double playerVx = 0, playerVy = 0;
    bullet bullets[NUM_BULLETS];

    unsigned short currentBullet = 0;
    unsigned short fireCycle = 0;
};