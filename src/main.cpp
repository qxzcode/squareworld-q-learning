#include "Learner.h"
#include "simulation.h"
#include "util.h"

#include "debug.h"

static void resetPlayer(GameState& state) {
    state.playerX() = util::rand() * simulation::SCREEN_WIDTH;
    state.playerY() = util::rand() * simulation::SCREEN_HEIGHT;
}

static GameState initState() {
    GameState state;
    resetPlayer(state);
    return state;
}

int main() {
    cout << "== Squareworld Q-Learning ==" << endl;
    
    Learner learner(0.3, 0.9, 0.1); // learning rate, discount factor, random action rate
    GameState state = initState();
    
    for (unsigned long n = 0; n < Learner::GENERATION_COUNT; n++) {
        GameState lastState = state;
        Action action = learner.chooseAction(state);
        auto res = simulation::update(state, action);
        learner.observeReward(lastState, action, res.second? lastState : state, res.first);
        if (res.second) {
            // reset the player pos
            resetPlayer(state);
        }
    }
    
    return 0;
}