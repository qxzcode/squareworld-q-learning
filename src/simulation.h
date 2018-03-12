#pragma once

#include "GameState.h"

namespace simulation {
    
    constexpr double SCREEN_WIDTH = 800;
    constexpr double SCREEN_HEIGHT = 600;
    constexpr double PLAYER_SPEED = 2;
    constexpr double BULLET_SPEED = 4;
    constexpr unsigned int FIRE_RATE = 2;
    
    double update(GameState& state, Action action);
    
}