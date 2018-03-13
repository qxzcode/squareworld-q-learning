#include "Learner.h"
#include "simulation.h"
#include "util.h"

#include "debug.h"

enum Command {
    TRAIN,
    VISUALIZE,
    UNKNOWN
};

void train() {
    Learner learner(0.3, 0.9, 0.1); // learning rate, discount factor, random action rate
    GameState state;
    simulation::reset(state);
    
    for (unsigned long n = 0; n < Learner::GENERATION_COUNT; n++) {
        GameState lastState = state;
        Action action = learner.chooseAction(state);
        auto res = simulation::update(state, action);
        learner.observeReward(lastState, action, res.second? lastState : state, res.first);
        if (res.second) {
            simulation::reset(state);
        }
    }
}

void visualize() {
    #ifdef VISUALIZER
    cout << "> Running SDL visualizer" << endl;
    Learner learner;
    GameState state;
    Visualizer visualizer(learner, state);
    visualizer.loop();
    #else
    cout << "Visualizer support not compiled in this binary." << endl;
    #endif
}

int main(int argc, char* argv[]) {
    cout << "== Squareworld Q-Learning ==" << endl;

    Command command = Command::TRAIN;
    if (argc > 1) {
        std::string carg(argv[1]);
        if (carg == "t") {
            command = Command::TRAIN;
        } else if (carg == "v") {
            command = Command::VISUALIZE;
        } else {
            command = Command::UNKNOWN;
        }
    }

    switch (command) {
        case Command::TRAIN:
            train();
            break;
        case Command::VISUALIZE:
            visualize();
            break;
        case Command::UNKNOWN:
            cout << "Unknown command " << argv[1] << endl;
            return -1;
    }
    
    return 0;
}