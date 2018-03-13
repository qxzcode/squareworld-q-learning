#include "simulation.h"

#include <cmath>

static void rotate(double& x, double& y, double sin, double cos) {
    double newX = x*cos+y*sin, newY = y*cos-x*sin;
    x = newX; y = newY;
}
static double calcAutoShoot(double dx, double dy, double vx, double vy, double v) {
    // rotate reference frame
    const double m = std::hypot(vx, vy);
    double sin = m==0? 0 : vy/m;
    double cos = m==0? 1 : vx/m;
    rotate(vx, vy, sin, cos);
    rotate(dx, dy, sin, cos);
    
    dy = -dy;
    const double adf2 = 2*vx*dx/dy;
    double df = 1 + (dx*dx)/(dy*dy);
    
    // calculate the result
    double disc = adf2*adf2 + 4*df*(v*v-vx*vx);
    // if (disc < 0) return; // no solution
    disc = std::sqrt(disc);
    df *= 2;
    sin = -sin; // invert rot
    
    double c1 = (adf2 + disc) / df;
    double b1 = vx - c1*dx/dy;
    const double t1 = dy/c1;
    rotate(b1, c1, sin, cos);
    const double res1 = std::atan2(c1, b1);
    double c2 = (adf2 - disc) / df;
    double b2 = vx - c2*dx/dy;
    const double t2 = dy/c2;
    rotate(b2, c2, sin, cos);
    const double res2 = std::atan2(c2, b2);
    
    if (t1 < 0) return res2;
    if (t2 < 0) return res1;
    return t1<t2? res1 : res2;
}

std::pair<double, bool> simulation::update(GameState& state, Action action) {
    // TODO
    short vx = 0, vy = 0;
    switch (action) {
        case Action::UP:
            vy = -PLAYER_SPEED;
            break;
        case Action::DOWN:
            vy = +PLAYER_SPEED;
            break;
        case Action::RIGHT:
            vx = +PLAYER_SPEED;
            break;
        case Action::LEFT:
            vx = -PLAYER_SPEED;
            break;
        default:
            // wuut (or NONE)
            break;
    }
    state.playerX() += vx;
    state.playerY() += vy;

    // enforce player bounding
    if (state.playerX() < PLAYER_SIZE / 2) {
        state.playerX() = PLAYER_SIZE / 2;
        vx = 0;
    }
    if (state.playerX() + (PLAYER_SIZE / 2) > SCREEN_WIDTH) {
        state.playerX() = SCREEN_WIDTH - (PLAYER_SIZE / 2);
        vx = 0;
    }
    if (state.playerY() < PLAYER_SIZE) {
        state.playerY() = PLAYER_SIZE / 2;
        vy = 0;
    }
    if (state.playerY() + (PLAYER_SIZE / 2) > SCREEN_HEIGHT) {
        state.playerY() = SCREEN_HEIGHT - (PLAYER_SIZE / 2);
        vy = 0;
    }

    // Fire a bullet (dark magick formula for aim)
    if (state.fireCycle >= FIRE_RATE) {
        state.fireCycle = 0;
        double bulletAngle = calcAutoShoot(
            (SCREEN_WIDTH / 2) - state.playerX(),
            (SCREEN_HEIGHT / 2) - state.playerY(),
            vx, vy,
            BULLET_SPEED
        );
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
        state.bulletX(i) += std::cos(state.bulletDirection(i)) * simulation::BULLET_SPEED;
        state.bulletY(i) += std::sin(state.bulletDirection(i)) * simulation::BULLET_SPEED;

        // check collision with player
        if ((state.bulletX(i) > state.playerX() - (simulation::PLAYER_SIZE / 2))
            && (state.bulletX(i) < state.playerX() + (simulation::PLAYER_SIZE / 2))
            && (state.bulletY(i) > state.playerY() - (simulation::PLAYER_SIZE / 2))
            && (state.bulletY(i) < state.playerY() + (simulation::PLAYER_SIZE / 2))) {
            // ouch.
            return {-10, true};
        }
    }

    // welp, player's still alive
    // take your cookie and leave
    return {0.1, false};
}

void simulation::reset(GameState& state) {
    state.playerX() = util::rand() * simulation::SCREEN_WIDTH;
    state.playerY() = util::rand() * simulation::SCREEN_HEIGHT;
}