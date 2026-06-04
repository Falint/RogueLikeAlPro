/**
 * @file random_manager.cpp
 * @brief Implementasi RandomManager — singleton RNG menggunakan mt19937.
 */

#include "random_manager.h"

namespace roguelike {

RandomManager::RandomManager()
    : engine_(std::random_device{}())
{
}

RandomManager& RandomManager::instance() {
    static RandomManager inst;
    return inst;
}

int RandomManager::getInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine_);
}

double RandomManager::getDouble(double min, double max) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(engine_);
}

bool RandomManager::rollChance(int chancePercent) {
    return getInt(1, 100) <= chancePercent;
}

void RandomManager::reseed(unsigned int seed) {
    engine_.seed(seed);
}

} // namespace roguelike
