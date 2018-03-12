#pragma once

#include "Action.h"

class GameState {
public:
    static constexpr unsigned NUM_BULLETS = 5;
    static constexpr unsigned NUM_VALUES = 2 + 2*NUM_BULLETS;
    
    double values[NUM_VALUES];
    
};