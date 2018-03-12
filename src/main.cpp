#include "Learner.h"
#include "simulation.h"
#include "util.h"

#include "debug.h"

static GameState initState() {
    GameState state;
    state.playerX() = util::rand();
    state.playerY() = util::rand();
    return state;
}

int main() {
    cout << "== Squareworld Q-Learning ==" << endl;
    
    Learner learner(0.3, 0.9, 0.1);
    GameState state = initState();
    
    for (long n = 0; n < 1000; n++) {
        simulation::update(state);
    }
    
    return 0;
}