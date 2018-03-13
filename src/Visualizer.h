#pragma once
#ifdef VISUALIZER

#include "Learner.h"
#include "simulation.h"
#include <SDL2/SDL.h>

class Visualizer {
public:
    static constexpr unsigned short SCALE = 4;

    Visualizer(Learner& learner, GameState& state):
        learner(learner), state(state) { }

    void init();
    void loop();

    Learner& learner;
    GameState& state;
private:
    void update();
    void draw() const;

    SDL_Surface* surface;
    SDL_Window* window;
};
#endif