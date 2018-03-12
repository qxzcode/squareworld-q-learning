#pragma once

#include <vector>

#include "genann.h"
#include "GameState.h"
#include "Action.h"

class Learner {
public:
    static constexpr unsigned NUM_INPUTS = GameState::NUM_VALUES + 1;
    
    Learner(Learner&&) = default;
    Learner(const Learner&) = delete; // I'm lazy
    Learner(double learningRate, double discountFactor, double randomRate):
                learningRate(learningRate),discountFactor(discountFactor),randomRate(randomRate) {
        ann = genann_init(NUM_INPUTS, 5, 10, 1);
    }
    ~Learner() {
        genann_free(ann);
    }
    
    void observeReward(const GameState& curState, Action action, const GameState& nextState, double reward);
    
    Action chooseAction(const GameState& state) const;
    
    void load();
    void save() const;
    
    double learningRate, discountFactor, randomRate;
    
private:
    genann* ann;
    
    struct observation {
        double inputs[NUM_INPUTS];
        double q;
    };
    std::vector<observation> replayMemory;
    
    double getQ(const GameState& state, Action action) const;
    double getMaxQ(const GameState& state) const;
    
};