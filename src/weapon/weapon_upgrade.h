/**
 * @file weapon_upgrade.h
 * @brief Sistem upgrade senjata menggunakan gold.
 *
 * Biaya upgrade meningkat seiring level.
 * Semua parameter berasal dari constants.h — tidak ada magic number.
 */

#pragma once

#include "weapon.h"

namespace roguelike {

/**
 * @brief Mengelola proses upgrade senjata.
 */
class WeaponUpgrade {
public:
    /**
     * @brief Menghitung biaya upgrade untuk level senjata saat ini.
     * @param currentLevel Level upgrade senjata saat ini.
     * @return Biaya dalam gold.
     *
     * Rumus: BASE_COST + currentLevel * COST_FACTOR
     */
    static int getUpgradeCost(int currentLevel);

    /**
     * @brief Mencoba meng-upgrade senjata.
     * @param weapon Senjata yang akan di-upgrade (modified in-place).
     * @param playerGold Gold pemain saat ini (modified in-place jika berhasil).
     * @return true jika upgrade berhasil, false jika gold tidak cukup atau max level.
     */
    static bool tryUpgrade(Weapon& weapon, int& playerGold);

    /**
     * @brief Mengecek apakah senjata sudah mencapai max upgrade level.
     */
    static bool isMaxLevel(const Weapon& weapon);

    /**
     * @brief Mendapatkan info upgrade sebagai string untuk display.
     */
    static std::string getUpgradeInfo(const Weapon& weapon, int playerGold);
};

} // namespace roguelike
