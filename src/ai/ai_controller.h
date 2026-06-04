/**
 * @file ai_controller.h
 * @brief Sistem AI untuk pergerakan enemy dan boss.
 *
 * Seluruh logika AI dipisahkan ke modul ini — entity sendiri
 * tidak mengandung logika pergerakan.
 *
 * Enemy biasa: bergerak menuju pemain jika dalam jarak deteksi.
 * Boss: selalu agresif, mengejar pemain dari jarak jauh.
 */

#pragma once

#include "../entity/enemy.h"
#include "../world/map.h"

#include <vector>
#include <memory>

namespace roguelike {

/**
 * @brief Controller AI untuk pergerakan enemy.
 */
class AIController {
public:
    /**
     * @brief Menggerakkan satu enemy menuju pemain.
     * @param enemy Enemy yang akan digerakkan.
     * @param playerPos Posisi pemain saat ini.
     * @param map Map dungeon untuk collision check.
     * @param allEnemies Semua enemy untuk menghindari tumpang tindih posisi.
     *
     * Enemy biasa hanya bergerak jika pemain dalam ENEMY_DETECTION_RANGE.
     * Boss selalu bergerak menuju pemain (BOSS_DETECTION_RANGE lebih besar).
     */
    static void moveTowardsPlayer(Enemy& enemy,
                                   const Position& playerPos,
                                   const Map& map,
                                   const std::vector<std::unique_ptr<Enemy>>& allEnemies);

    /**
     * @brief Mengecek apakah enemy berada di samping pemain (bisa menyerang).
     * @return true jika jarak Manhattan == 1.
     */
    static bool isAdjacentToPlayer(const Enemy& enemy, const Position& playerPos);

private:
    /**
     * @brief Menghitung jarak Manhattan antara dua posisi.
     */
    static int manhattanDistance(const Position& a, const Position& b);

    /**
     * @brief Mengecek apakah posisi sudah ditempati enemy lain.
     */
    static bool isOccupiedByEnemy(const Position& pos,
                                   const Enemy& excludeEnemy,
                                   const std::vector<std::unique_ptr<Enemy>>& allEnemies);
};

} // namespace roguelike
