#pragma once

#include <utility> // std::pair

#include "GameState.h"

namespace simulation {
    
    constexpr double SCREEN_WIDTH = 240;
    constexpr double SCREEN_HEIGHT = 240;
    constexpr double PLAYER_SPEED = 2;
    constexpr double BULLET_SPEED = 4;
    constexpr double PLAYER_SIZE = 6;
    constexpr unsigned int FIRE_RATE = 8;
    
    std::pair<double, bool> update(GameState& state, Action action);

    void reset(GameState& state);
    
}