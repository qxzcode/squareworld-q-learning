#pragma once

#include "genann.h"
#include "GameState.h"
#include "Action.h"

class Learner {
public:
    static constexpr unsigned NUM_INPUTS = GameState::NUM_VALUES + 1;
    
    Learner(Learner&&) = default;
    Learner(const Learner&) = delete; // I'm lazy
    Learner(double learningRate, double discountFactor, double qRandomness):
            learningRate(learningRate),discountFactor(discountFactor),qRandomness(qRandomness) {
        ann = genann_init(NUM_INPUTS, 5, 10, 1);
    }
    ~Learner() {
        genann_free(ann);
    }
    
    void observeReward(const GameState& curState, Action action, const GameState& nextState, double reward);
    
    Action chooseAction(const GameState& state) const;
    
    void load();
    void save() const;
    
    double learningRate, discountFactor, qRandomness;
    
private:
    genann* ann;
    
    double getQ(const GameState& state, Action action) const;
    double getMaxQ(const GameState& state) const;
    
};