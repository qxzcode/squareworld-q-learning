#pragma once

#include "Learner.h"

class Visualizer {
public:
    Visualizer(Learner& learner, GameState& state):
        learner(learner), state(state) { }

    void loop();

    Learner& learner;
    GameState& state;
private:
    void update();
    void draw() const;
};