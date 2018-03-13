#include "Learner.h"

#include <limits>
#include <iterator>
#include <chrono> // to seed randomness
#include <random> // std::default_random_engine
#include <algorithm> // std::shuffle
#include <stdio.h>

#include "debug.h"
#include "util.h"

constexpr double MIN_DOUBLE = -std::numeric_limits<double>::max();

static std::default_random_engine randEngine(std::chrono::system_clock::now().time_since_epoch().count());

static double normX(double x) {
    return x / simulation::SCREEN_WIDTH;
}
static double normY(double y) {
    return y / simulation::SCREEN_HEIGHT;
}
static double normV(double v) {
    return (v + simulation::PLAYER_MAX_SPEED) / (simulation::PLAYER_MAX_SPEED * 2);
}
static double normAngle(double angle) {
    return angle / 3.1415926535897931;
}

using inputArr_t = double[Learner::NUM_INPUTS];
static void fillInputs(inputArr_t& inputs, const GameState& state, Action action) {
    inputs[0] = normX(state.playerX);
    inputs[1] = normY(state.playerY);
    inputs[2] = normV(state.playerVx);
    inputs[3] = normV(state.playerVy);
    for (unsigned i = 0; i < GameState::NUM_BULLETS; i++) {
        inputs[4+i*3] = normX(state.bullets[i].x - state.playerX);
        inputs[5+i*3] = normY(state.bullets[i].y - state.playerY);
        inputs[6+i*3] = normAngle(state.bullets[i].direction);
    }
    for (unsigned i = 0; i < Action::NUM_ACTIONS; i++) {
        inputs[GameState::NUM_VALUES + i] = action==i? 1.0 : 0.0;
    }
}

// static size_t counter = 0;
void Learner::observeReward(const GameState& curState, Action action, const GameState& nextState, double reward) {
    observation obs;
    obs.q = reward + discountFactor*getMaxQ(nextState);
    fillInputs(obs.inputs, curState, action);
    
    // for (double d : obs.inputs) {
    //     cout << d << endl;
    // }
    // cout << endl;
    // cout << obs.q << endl << endl;
    
    replayMemory.push_back(std::move(obs));
    if (replayMemory.size() >= Learner::REPLAY_MEMORY_SIZE) {
        for (unsigned long n = 0; n < Learner::TRAIN_LOOPS; n++) {
            std::shuffle(replayMemory.begin(), replayMemory.end(), randEngine);
            for (auto&& o : replayMemory) {
                genann_train(ann, o.inputs, &o.q, learningRate);
            }
        }
        replayMemory.clear();
    }
}

Action Learner::chooseAction(const GameState& state) const {
    if (util::rand() < randomRate) {
        return static_cast<Action>(util::rand() * NUM_ACTIONS);
    }
    
    Action bestAction = Action::NUM_ACTIONS;
    double bestQ = MIN_DOUBLE;
    forEachAction([&](Action a) {
        double Q = getQ(state, a);
        // cout << "q["<<a<<"] = "<<Q << endl;
        if (Q > bestQ) {
            bestQ = Q;
            bestAction = a;
        }
    });
    // cout << endl;
    return bestAction;
}


void Learner::load() {
    genann_free(ann);
    FILE* file = fopen("save.nnt", "r");
    ann = genann_read(file);
    fclose(file);
    cout << "Loaded learner" << endl;
}

void Learner::save() const {
    FILE* file = fopen("save.nnt", "w");
    genann_write(ann, file);
    fclose(file);
    cout << "Saved learner" << endl;
}


double Learner::getQ(const GameState& curState, Action action) const {
    double inputs[NUM_INPUTS];
    fillInputs(inputs, curState, action);
    return *genann_run(ann, inputs);
}

double Learner::getMaxQ(const GameState& state) const {
    double maxQ = MIN_DOUBLE;
    forEachAction([&](Action a) {
        maxQ = std::max(maxQ, getQ(state, a));
    });
    return maxQ;
}