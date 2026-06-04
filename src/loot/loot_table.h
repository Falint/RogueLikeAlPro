/**
 * @file loot_table.h
 * @brief Sistem loot table modular untuk enemy biasa dan boss.
 *
 * Menggunakan probabilitas dari LootConfig dan RandomManager.
 * Menghasilkan daftar item dan senjata yang di-drop.
 */

#pragma once

#include "../inventory/item.h"
#include "../weapon/weapon.h"
#include "../entity/enemy.h"

#include <vector>
#include <string>

namespace roguelike {

/**
 * @brief Hasil loot dari satu enemy yang mati.
 */
struct LootDrop {
    std::vector<Item> items;
    std::vector<Weapon> weapons;
    int bonusGold = 0; ///< Gold tambahan di luar reward dasar enemy
};

/**
 * @brief Generator loot berdasarkan tipe musuh.
 */
class LootTable {
public:
    /**
     * @brief Menghasilkan loot dari enemy biasa.
     */
    static LootDrop generateEnemyLoot();

    /**
     * @brief Menghasilkan loot dari boss.
     */
    static LootDrop generateBossLoot();

private:
    /** @brief Membuat senjata common acak. */
    static Weapon generateCommonWeapon();

    /** @brief Membuat senjata rare acak. */
    static Weapon generateRareWeapon();

    /** @brief Membuat senjata epic acak. */
    static Weapon generateEpicWeapon();

    /** @brief Membuat senjata legendary acak. */
    static Weapon generateLegendaryWeapon();
};

} // namespace roguelike
