#include "Visualizer.h"

void Visualizer::loop() {
    // TODO
}

void Visualizer::update() {
    Action action = learner.chooseAction(state);
    auto res = simulation::update(state, action);
    if (res.second) {
        simulation::reset(state);
    }
}

void Visualizer::draw() const {
    // TODO
}