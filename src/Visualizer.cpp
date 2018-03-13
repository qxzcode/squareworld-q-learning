#ifdef VISUALIZER
#include "Visualizer.h"

struct Color {
    int r, g, b;
};

static void fillRect(SDL_Surface* surface, int x, int y, int w, int h, Color col) {
    SDL_Rect r = {x * Visualizer::SCALE, y * Visualizer::SCALE, w * Visualizer::SCALE, h * Visualizer::SCALE};
    SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, col.r, col.g, col.b));
}

void Visualizer::init() {
    // initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Squareworld Q-Learning", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        simulation::SCREEN_WIDTH * Visualizer::SCALE, simulation::SCREEN_HEIGHT * Visualizer::SCALE, SDL_WINDOW_ALLOW_HIGHDPI);
    surface = SDL_GetWindowSurface(window);
}

void Visualizer::loop() {
    // TODO
    draw();
    SDL_UpdateWindowSurface(window);
    SDL_Delay(10000);
}

void Visualizer::update() {
    Action action = learner.chooseAction(state);
    auto res = simulation::update(state, action);
    if (res.second) {
        simulation::reset(state);
    }
}

void Visualizer::draw() const {
    // clear
    fillRect(surface, 0, 0, simulation::SCREEN_WIDTH * Visualizer::SCALE,
        simulation::SCREEN_HEIGHT * Visualizer::SCALE, {0xFF, 0xFF, 0xFF});
    // render
    fillRect(surface, state.playerX(), state.playerY(), simulation::PLAYER_SIZE, simulation::PLAYER_SIZE, {0x00, 0x00, 0xFF});
}
#endif