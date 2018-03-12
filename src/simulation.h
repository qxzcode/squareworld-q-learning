#pragma once

#include "GameState.h"
#include <cmath>

namespace simulation {
    
    constexpr double SCREEN_WIDTH = 100;
    constexpr double SCREEN_HEIGHT = 100;
    constexpr double PLAYER_SPEED = 2;
    constexpr double BULLET_SPEED = 4;
    constexpr double PLAYER_SIZE = 10;
    constexpr unsigned int FIRE_RATE = 2;
    
    double update(GameState& state, Action action);
    
}