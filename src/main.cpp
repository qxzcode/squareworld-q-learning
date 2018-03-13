#ifdef VISUALIZER
#include <SDL2/SDL.h> // for the SDL_main thingy
#endif

#include "Learner.h"
#include "Visualizer.h"
#include "simulation.h"
#include "util.h"

#include "debug.h"

enum Command {
    TRAIN,
    VISUALIZE,
    UNKNOWN
};

static constexpr double DEFAULT_LEARN_RATE = 0.3;
static constexpr double DEFAULT_DISCOUNT_FACTOR = 0.9;
static constexpr double DEFAULT_RANDOM_RATE = 0.1;

void train(bool fresh) {
    double learnRate = DEFAULT_LEARN_RATE;
    double discountFactor = DEFAULT_DISCOUNT_FACTOR;
    double randomRate = DEFAULT_RANDOM_RATE;

    cout << "> Running Q-Learning train" << endl;
    cout << "Learning rate: " << learnRate << endl;
    cout << "Discount factor: " << discountFactor << endl;
    cout << "Random action rate: " << randomRate << endl;
    cout << "Replay memory: " << Learner::REPLAY_MEMORY_SIZE << endl;
    cout << "Train loops: " << Learner::TRAIN_LOOPS << endl;
    cout << "Training for " << Learner::GENERATION_COUNT << " generations" << endl;

    Learner learner(learnRate, discountFactor, randomRate); // learning rate, discount factor, random action rate
    if (!fresh) {
        learner.load();
        cout << "Loaded saved neural network" << endl;
    }
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

    cout << "Saving neural network" << endl;
    learner.save();
    cout << "Save completed successfully." << endl;
}

void visualize() {
    #ifdef VISUALIZER
    cout << "Loading saved model with default parameters." << endl;
    Learner learner(DEFAULT_LEARN_RATE, DEFAULT_DISCOUNT_FACTOR, DEFAULT_RANDOM_RATE);
    learner.load();
    GameState state;
    Visualizer visualizer(learner, state);
    cout << "> Running SDL visualizer" << endl;
    visualizer.init();
    visualizer.loop();
    #else
    cout << "Visualizer support not compiled in this binary." << endl;
    #endif
}

int main(int argc, char* argv[]) {
    cout << "== Squareworld Q-Learning ==" << endl;

    Command command = Command::TRAIN;
    bool fresh = true;
    if (argc > 1) {
        std::string carg(argv[1]);
        if (carg == "t") {
            command = Command::TRAIN;
        } else if (carg == "r") {
            command = Command::TRAIN;
            fresh = false;
        } else if (carg == "v") {
            command = Command::VISUALIZE;
        } else {
            command = Command::UNKNOWN;
        }
    }

    switch (command) {
        case Command::TRAIN:
            train(fresh);
            break;
        case Command::VISUALIZE:
            visualize();
            break;
        case Command::UNKNOWN:
            cout << "Unknown command " << argv[1] << endl;
            cout << "Commands: t, v " << endl;
            return -1;
    }
    
    return 0;
}