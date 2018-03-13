#pragma once

enum Action {
    LEFT, NONE, UP, DOWN, RIGHT,
    NUM_ACTIONS
};

template<class Func>
static void forEachAction(const Func& func) {
    for (int ai = 0; ai < Action::NUM_ACTIONS; ai++) {
        func(static_cast<Action>(ai));
    }
}