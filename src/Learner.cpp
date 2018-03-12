#include "Learner.h"

#include <limits>
#include <iterator>
#include <stdio.h>

#include "debug.h"
#include "util.h"

constexpr double MIN_DOUBLE = -std::numeric_limits<double>::max();

// static size_t counter = 0;
void Learner::observeReward(const GameState& curState, Action action, const GameState& nextState, double reward) {
    double newQ = reward + discountFactor*getMaxQ(nextState);
    
    double inputs[NUM_INPUTS];
    std::copy(std::begin(curState.values), std::end(curState.values), inputs);
    inputs[NUM_INPUTS-1] = action;
    
    genann_train(ann, inputs, &newQ, learningRate);
}

Action Learner::chooseAction(const GameState& state) const {
    Action bestAction = Action::NUM_ACTIONS;
    double bestQ = MIN_DOUBLE;
    forEachAction([&](Action a) {
        double Q = getQ(state, a) + util::rand()*qRandomness;
        if (Q > bestQ) {
            bestQ = Q;
            bestAction = a;
        }
    });
    return bestAction;
}


void Learner::load() {
    genann_free(ann);
    FILE* file = fopen("save.txt", "r");
    ann = genann_read(file);
    fclose(file);
    cout << "Loaded learner" << endl;
}

void Learner::save() const {
    FILE* file = fopen("save.txt", "w");
    genann_write(ann, file);
    fclose(file);
    cout << "Saved learner" << endl;
}


double Learner::getQ(const GameState& curState, Action action) const {
    double inputs[NUM_INPUTS];
    std::copy(std::begin(curState.values), std::end(curState.values), inputs);
    inputs[NUM_INPUTS-1] = action;
    return *genann_run(ann, inputs);
}

double Learner::getMaxQ(const GameState& state) const {
    double maxQ = MIN_DOUBLE;
    forEachAction([&](Action a) {
        maxQ = std::max(maxQ, getQ(state, a));
    });
    return maxQ;
}