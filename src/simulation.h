#pragma once

#include <utility> // std::pair

#include "GameState.h"
#include "Constants.h"

namespace simulation {
    
    std::pair<double, bool> update(GameState& state, Action action);

    void reset(GameState& state);
    
}