/**
 * @file enemy.h
 * @brief Kelas Enemy — musuh biasa yang mengejar dan menyerang pemain.
 *
 * Stats di-scale berdasarkan floor index menggunakan konstanta.
 */

#pragma once

#include "entity.h"

namespace roguelike {

/**
 * @brief Musuh biasa dalam dungeon.
 */
class Enemy : public Entity {
public:
    /**
     * @brief Membuat enemy dengan stats yang di-scale berdasarkan floor.
     * @param pos Posisi awal enemy.
     * @param floorIndex Indeks lantai (0 = easy, 1 = medium, 2 = boss).
     */
    Enemy(Position pos, int floorIndex);

    /** @brief Mendapatkan jumlah EXP yang diberikan saat mati. */
    int getExpReward() const;

    /** @brief Mendapatkan jumlah gold yang di-drop saat mati. */
    int getGoldReward() const;

    /** @brief Mendapatkan floor index enemy ini. */
    int getFloorIndex() const;

private:
    int expReward_;
    int goldReward_;
    int floorIndex_;
};

} // namespace roguelike
