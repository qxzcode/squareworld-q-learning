#pragma once
#ifdef VISUALIZER

#include "Learner.h"
#include "simulation.h"
#include <SDL2/SDL.h>

struct Color {
    int r, g, b;
};

class Visualizer {
public:
    static constexpr double FPS = 30;

    Visualizer(Learner& learner, GameState& state):
        learner(learner), state(state) { }

    void init();
    void loop();
    void quit();

    Learner& learner;
    GameState& state;
    bool running = true;
    int scale = 4;
private:
    void update();
    void draw() const;

    void fillRect(SDL_Surface* surface, int x, int y, int w, int h, Color col) const;

    SDL_Surface* surface;
    SDL_Window* window;
};
#endif