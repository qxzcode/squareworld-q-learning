#include "Learner.h"
#include "simulation.h"
#include "util.h"

#include "debug.h"

static GameState initState() {
    GameState state;
    state.playerX() = util::rand() * simulation::SCREEN_WIDTH;
    state.playerY() = util::rand() * simulation::SCREEN_HEIGHT;
    return state;
}

int main() {
    cout << "== Squareworld Q-Learning ==" << endl;
    
    Learner learner(0.3, 0.9, 0.1); // learning rate, discount factor, random action rate
    GameState state = initState();
    
    for (unsigned long n = 0; n < Learner::REPLAY_MEMORY_SIZE; n++) {
        GameState lastState = state;
        Action action = learner.chooseAction(state);
        double reward = simulation::update(state, action);
        learner.observeReward(lastState, action, state, reward);
    }
    
    return 0;
}