#include "simulation.h"

#include <cmath>

#include "Constants.h"

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
    double passiveReward = 0.004;
    short ax = 0, ay = 0;
    switch (action) {
        case Action::UP:
            ay = -PLAYER_ACCEL;
            break;
        case Action::DOWN:
            ay = +PLAYER_ACCEL;
            break;
        case Action::RIGHT:
            ax = +PLAYER_ACCEL;
            break;
        case Action::LEFT:
            ax = -PLAYER_ACCEL;
            break;
        default:
            // wuut (or NONE)
            break;
    }
    // apply acceleration
    state.playerVx += ax;
    state.playerVy += ay;

    // enforce max velocity
    if (state.playerVx > PLAYER_MAX_SPEED) {
        state.playerVx = PLAYER_MAX_SPEED;
    }
    if (state.playerVx < -PLAYER_MAX_SPEED) {
        state.playerVx = -PLAYER_MAX_SPEED;
    }
    if (state.playerVy > PLAYER_MAX_SPEED) {
        state.playerVy = PLAYER_MAX_SPEED;
    }
    if (state.playerVy < -PLAYER_MAX_SPEED) {
        state.playerVy = -PLAYER_MAX_SPEED;
    }
    
    // integrate velocity
    state.playerX += state.playerVx;
    state.playerY += state.playerVy;

    // enforce player bounding
    if (state.playerX < PLAYER_SIZE / 2) {
        state.playerX = PLAYER_SIZE / 2;
        state.playerVx = 0;
    }
    if (state.playerX + (PLAYER_SIZE / 2) > SCREEN_WIDTH) {
        state.playerX = SCREEN_WIDTH - (PLAYER_SIZE / 2);
        state.playerVx = 0;
    }
    if (state.playerY < PLAYER_SIZE) {
        state.playerY = PLAYER_SIZE / 2;
        state.playerVy = 0;
    }
    if (state.playerY + (PLAYER_SIZE / 2) > SCREEN_HEIGHT) {
        state.playerY = SCREEN_HEIGHT - (PLAYER_SIZE / 2);
        state.playerVy = 0;
    }

    // slightly penalize not moving
    // if (util::compareDouble(std::abs(state.playerVx), 0) && util::compareDouble(std::abs(state.playerVy), 0)) {
    //     passiveReward = 0;
    // }

    // Fire a bullet (dark magick formula for aim)
    if (state.fireCycle >= FIRE_RATE) {
        state.fireCycle = 0;
        double bulletAngle = calcAutoShoot(
            (SCREEN_WIDTH / 2) - state.playerX,
            (SCREEN_HEIGHT / 2) - state.playerY,
            state.playerVx, state.playerVy,
            BULLET_SPEED
        );
        // bullet starts in center
        state.bullets[state.currentBullet].x = simulation::SCREEN_WIDTH / 2;
        state.bullets[state.currentBullet].y = simulation::SCREEN_HEIGHT / 2;
        state.bullets[state.currentBullet].direction = bulletAngle;
        state.currentBullet++;
        state.currentBullet %= GameState::NUM_BULLETS;
    }
    state.fireCycle++;
    // step the bullets
    for (unsigned int i = 0; i < GameState::NUM_BULLETS; i++) {
        state.bullets[i].x += std::cos(state.bullets[i].direction) * simulation::BULLET_SPEED;
        state.bullets[i].y += std::sin(state.bullets[i].direction) * simulation::BULLET_SPEED;

        // check collision with player
        if ((state.bullets[i].x > state.playerX - (simulation::PLAYER_SIZE / 2))
            && (state.bullets[i].x < state.playerX + (simulation::PLAYER_SIZE / 2))
            && (state.bullets[i].y > state.playerY - (simulation::PLAYER_SIZE / 2))
            && (state.bullets[i].y < state.playerY + (simulation::PLAYER_SIZE / 2))) {
            // ouch.
            return {-1.0, true};
        }
    }

    // welp, player's still alive
    // take your cookie and leave
    return {passiveReward, false};
}

void simulation::reset(GameState& state) {
    state.playerX = util::rand() * simulation::SCREEN_WIDTH;
    state.playerY = util::rand() * simulation::SCREEN_HEIGHT;
    state.playerVx = state.playerVy = 0.0;
    for (auto&& b : state.bullets) {
        b.x = simulation::SCREEN_WIDTH/2;
        b.y = simulation::SCREEN_HEIGHT/2;
        b.direction = 0.0;
    }
}