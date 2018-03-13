#pragma once

#include <utility> // std::pair

#include "GameState.h"

namespace simulation {
    
    std::pair<double, bool> update(GameState& state, Action action);

    void reset(GameState& state);
    
}