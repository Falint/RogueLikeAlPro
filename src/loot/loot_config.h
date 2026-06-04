/**
 * @file loot_config.h
 * @brief Konfigurasi probabilitas loot terpusat.
 *
 * Semua angka drop rate berasal dari constants.h.
 * File ini menyediakan API yang memudahkan query drop rate
 * tanpa perlu mengakses constants secara langsung.
 */

#pragma once

#include "../entity/enemy.h"

namespace roguelike {

/**
 * @brief Menyediakan akses terpusat ke konfigurasi drop rate.
 */
class LootConfig {
public:
    /** @brief Drop rate gold untuk enemy biasa (persen). */
    static int getEnemyGoldChance();

    /** @brief Drop rate potion untuk enemy biasa (persen). */
    static int getEnemyPotionChance();

    /** @brief Drop rate senjata common untuk enemy biasa (persen). */
    static int getEnemyCommonWeaponChance();

    /** @brief Drop rate senjata rare untuk enemy biasa (persen). */
    static int getEnemyRareWeaponChance();

    /** @brief Drop rate gold untuk boss (persen). */
    static int getBossGoldChance();

    /** @brief Drop rate senjata epic untuk boss (persen). */
    static int getBossEpicWeaponChance();

    /** @brief Drop rate senjata legendary untuk boss (persen). */
    static int getBossLegendaryWeaponChance();

    /** @brief Drop rate potion untuk boss (persen). */
    static int getBossPotionChance();
};

} // namespace roguelike
