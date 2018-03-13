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
    while (running) {
        update();
        if (!running) return;
        draw();
        SDL_Delay(1000 / FPS);
    }
}

void Visualizer::update() {
    // SDL stuff
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quit();
                return;
        }
    }
    // run the model
    Action action = learner.chooseAction(state);
    auto res = simulation::update(state, action);
    if (res.second) {
        simulation::reset(state);
    }
}

void Visualizer::draw() const {
    // --clear
    fillRect(surface, 0, 0, simulation::SCREEN_WIDTH * Visualizer::SCALE,
        simulation::SCREEN_HEIGHT * Visualizer::SCALE, {0xFF, 0xFF, 0xFF});
    // --render
    // player
    fillRect(surface, state.playerX(), state.playerY(), simulation::PLAYER_SIZE, simulation::PLAYER_SIZE, {0x00, 0x00, 0xFF});
    // bullets [arbitrarily size 2]
    for (unsigned int i = 0; i < GameState::NUM_BULLETS; i++) {
        fillRect(surface, state.bulletX(i) - 1, state.bulletY(i) - 1, 2, 2, {0xFF, 0x00, 0x00});
    }
    SDL_UpdateWindowSurface(window);
}

void Visualizer::quit() {
    running = false;
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
#endif