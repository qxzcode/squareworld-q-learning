#pragma once

#include <utility> // std::pair

#include "GameState.h"

namespace simulation {
    
    constexpr double SCREEN_WIDTH = 100;
    constexpr double SCREEN_HEIGHT = 100;
    constexpr double PLAYER_SPEED = 1;
    constexpr double BULLET_SPEED = 3;
    constexpr double PLAYER_SIZE = 4;
    constexpr unsigned int FIRE_RATE = 25;
    
    std::pair<double, bool> update(GameState& state, Action action);

    void reset(GameState& state);
    
}