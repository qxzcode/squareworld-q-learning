#include "util.h"

#include <thread>
#include <chrono>
#include <random>

static std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
static std::uniform_real_distribution<double> dist(0.0, 1.0);
double util::rand() {
    return dist(gen);
}

double util::getTime() {
    return std::chrono::high_resolution_clock::now().time_since_epoch().count() / 1000000000.0;
}

void util::sleep(long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

const float ROUNDING_ERROR = 0.000001f;

bool util::compareDouble(const double a, const double b) {
    return (a + ROUNDING_ERROR >= b) && (a - ROUNDING_ERROR <= b);
}