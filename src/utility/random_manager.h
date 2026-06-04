/**
 * @file random_manager.h
 * @brief Random number generator terpusat menggunakan <random>.
 *
 * Singleton pattern memastikan seluruh sistem (loot, combat, map generation)
 * menggunakan sumber random yang sama dan reproducible.
 * TIDAK menggunakan rand() atau srand().
 */

#pragma once

#include <random>

namespace roguelike {

class RandomManager {
public:
    /**
     * @brief Mendapatkan instance singleton.
     */
    static RandomManager& instance();

    /**
     * @brief Menghasilkan integer acak dalam rentang [min, max] (inklusif).
     */
    int getInt(int min, int max);

    /**
     * @brief Menghasilkan float acak dalam rentang [min, max].
     */
    double getDouble(double min, double max);

    /**
     * @brief Menguji probabilitas — return true jika roll berhasil.
     * @param chancePercent Nilai 0–100 yang merepresentasikan persentase keberhasilan.
     */
    bool rollChance(int chancePercent);

    /**
     * @brief Reset seed (untuk testing atau new game).
     */
    void reseed(unsigned int seed);

    /** Deleted copy/move untuk singleton */
    RandomManager(const RandomManager&) = delete;
    RandomManager& operator=(const RandomManager&) = delete;

private:
    RandomManager();

    std::mt19937 engine_;
};

} // namespace roguelike
