#include "simulation.h"

double simulation::update(GameState& state, Action action) {
    // TODO
    switch (action) {
        case Action::UP:
            state.playerY() -= simulation::PLAYER_SPEED;
            break;
        case Action::DOWN:
            state.playerY() += simulation::PLAYER_SPEED;
            break;
        case Action::RIGHT:
            state.playerX() += simulation::PLAYER_SPEED;
            break;
        case Action::LEFT:
            state.playerX() -= simulation::PLAYER_SPEED;
            break;
        default:
            // wuut
            break;
    }
    // Fire a bullet (dark magick formula for aim)
    if (state.fireCycle >= simulation::FIRE_RATE) {
        state.fireCycle = 0;
        double bulletAngle = 0; // MAGICK HERE
        // bullet starts in center
        state.bulletX(state.currentBullet) = simulation::SCREEN_WIDTH / 2;
        state.bulletY(state.currentBullet) = simulation::SCREEN_HEIGHT / 2;
        state.bulletDirection(state.currentBullet) = bulletAngle;
        state.currentBullet++;
        state.currentBullet %= GameState::NUM_BULLETS;
    }
    state.fireCycle++;
    // step the bullets
    for (unsigned int i = 0; i < GameState::NUM_BULLETS; i++) {
        state.bulletX(i) += std::cos(state.bulletDirection(i));
        state.bulletY(i) += std::sin(state.bulletDirection(i));

        // check collision with player
        if ((state.bulletX(i) > state.playerX())
            && (state.bulletX(i) < state.playerX() + simulation::PLAYER_SIZE)
            && (state.bulletY(i) > state.playerY())
            && (state.bulletY(i) < state.playerY() + simulation::PLAYER_SIZE)) {
            // ouch.
            return -10;
        }
    }

    // welp, player's still alive
    // take your cookie and leave
    return 0.1;
}