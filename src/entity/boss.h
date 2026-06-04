/**
 * @file boss.h
 * @brief Kelas Boss — musuh kuat dengan AI agresif dan loot table khusus.
 *
 * Boss memiliki HP, attack, dan defense yang jauh lebih tinggi dari enemy biasa.
 * Stats dihitung dari base enemy stats × boss multiplier.
 */

#pragma once

#include "enemy.h"

namespace roguelike {

/**
 * @brief Boss enemy dengan stats tinggi dan loot table khusus.
 */
class Boss : public Enemy {
public:
    /**
     * @brief Membuat boss pada posisi dan floor tertentu.
     * @param pos Posisi boss.
     * @param floorIndex Floor index.
     */
    Boss(Position pos, int floorIndex);

    /** @brief Mengecek apakah entity ini adalah boss. */
    bool isBoss() const;
};

} // namespace roguelike
