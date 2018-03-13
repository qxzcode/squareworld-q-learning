#pragma once

#include <vector>

#include "genann.h"
#include "GameState.h"
#include "Action.h"
#include "Constants.h"

class Learner {
public:
    static constexpr unsigned NUM_INPUTS = GameState::NUM_VALUES + Action::NUM_ACTIONS;
    static constexpr uint64_t REPLAY_MEMORY_SIZE = 200;
    static constexpr uint64_t TRAIN_LOOPS = 300;
    static constexpr unsigned HIDDEN_LAYERS = 8;
    static constexpr unsigned HIDDEN_LAYER_NEURONS = 8;
    
    Learner(Learner&&) = default;
    Learner(const Learner&) = delete; // I'm lazy
    Learner(double learningRate, double discountFactor, double randomRate):
                learningRate(learningRate),discountFactor(discountFactor),randomRate(randomRate) {
        ann = genann_init(NUM_INPUTS, HIDDEN_LAYERS, HIDDEN_LAYER_NEURONS, 1);
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