#ifdef VISUALIZER
#include <SDL2/SDL.h> // for the SDL_main thingy
#endif

#include <thread>
#include <atomic>

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

static constexpr uint64_t DEFAULT_GENERATION_COUNT = UINT64_MAX;
static constexpr double DEFAULT_LEARN_RATE = 0.3;
static constexpr double DEFAULT_DISCOUNT_FACTOR = 0.99;
static constexpr double DEFAULT_RANDOM_RATE = 0.3;

std::atomic<bool> cinReady(false);
void flushCin() {
    cin.seekg(0, std::ios::end);
    cin.clear();
}

void train(bool fresh, uint64_t generationCount) {
    double learnRate = DEFAULT_LEARN_RATE;
    double discountFactor = DEFAULT_DISCOUNT_FACTOR;
    double randomRate = DEFAULT_RANDOM_RATE;

    cout << "> Running Q-Learning train" << endl;
    cout << "Learning rate: " << learnRate << endl;
    cout << "Discount factor: " << discountFactor << endl;
    cout << "Random action rate: " << randomRate << endl;
    cout << "NN topology: " << Learner::HIDDEN_LAYERS << "H" << Learner::HIDDEN_LAYER_NEURONS << endl;
    cout << "Replay memory: " << Learner::REPLAY_MEMORY_SIZE << endl;
    cout << "Train loops: " << Learner::TRAIN_LOOPS << endl;
    cout << "Training for " << generationCount << " generations (press ENTER to halt)" << endl;

    Learner learner(learnRate, discountFactor, randomRate); // learning rate, discount factor, random action rate
    if (!fresh) {
        learner.load();
        cout << "Loaded saved neural network" << endl;
    }
    GameState state;
    simulation::reset(state);
    
    // start helper thread
    std::thread helperThread;
    if (generationCount == UINT64_MAX) {
        helperThread = std::thread([]() {
            cin.peek();
            flushCin();
            cinReady = true;
        });
    }
    
    double lastProgress = util::getTime();
    double lastSave = util::getTime();
    for (uint64_t n = 0; !cinReady && n < generationCount; n++) {
        GameState lastState = state;
        Action action = learner.chooseAction(state);
        auto res = simulation::update(state, action);
        learner.observeReward(lastState, action, res.second? lastState : state, res.first);
        if (res.second) {
            simulation::reset(state);
        }
        
        double time = util::getTime();
        if (time - lastProgress > 1.0) {
            lastProgress = time;
            cout << "Generation: " << n << "         \r" << std::flush;
        }
        if (time - lastSave > 5*60.0) {
            lastSave = time;
            learner.save();
        }
    }

    cout << "Saving neural network" << endl;
    learner.save();
    cout << "Save completed successfully." << endl;

    flushCin();
    helperThread.join();
}

void visualize(int scale) {
    #ifdef VISUALIZER
    cout << "Loading saved model with default parameters." << endl;
    Learner learner(DEFAULT_LEARN_RATE, DEFAULT_DISCOUNT_FACTOR, 0);
    learner.load();
    GameState state;
    simulation::reset(state);
    Visualizer visualizer(learner, state);
    visualizer.scale = scale;
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
    uint64_t generationCount = DEFAULT_GENERATION_COUNT;
    int visualizerScale = 4;
    if (argc > 1) {
        std::string carg(argv[1]);
        if (carg.find("t") != std::string::npos) {
            command = Command::TRAIN;
            if (carg.find("r") != std::string::npos) {
                fresh = false;
            }
            if (argc > 2) {
                std::string gcarg(argv[2]);
                std::istringstream iss(gcarg);
                iss >> generationCount;
            }
        } else if (carg == "v") {
            command = Command::VISUALIZE;
            if (argc > 2) {
                std::string gcarg(argv[2]);
                std::istringstream iss(gcarg);
                iss >> visualizerScale;
            }
        } else {
            command = Command::UNKNOWN;
        }
    }

    switch (command) {
        case Command::TRAIN:
            train(fresh, generationCount);
            break;
        case Command::VISUALIZE:
            visualize(visualizerScale);
            break;
        case Command::UNKNOWN:
            cout << "Unknown command " << argv[1] << endl;
            cout << "Commands: t, v " << endl;
            return -1;
    }
    
    return 0;
}