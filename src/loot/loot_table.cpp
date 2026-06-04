/**
 * @file loot_table.cpp
 * @brief Implementasi loot table.
 *
 * Setiap roll drop menggunakan RandomManager.rollChance()
 * dengan probabilitas dari LootConfig.
 */

#include "loot_table.h"
#include "loot_config.h"
#include "../utility/random_manager.h"
#include "../utility/constants.h"

namespace roguelike {

LootDrop LootTable::generateEnemyLoot() {
    LootDrop drop;
    auto& rng = RandomManager::instance();

    /* Roll gold drop */
    if (rng.rollChance(LootConfig::getEnemyGoldChance())) {
        int goldAmount = rng.getInt(constants::GOLD_DROP_MIN, constants::GOLD_DROP_MAX);
        drop.bonusGold = goldAmount;
    }

    /* Roll potion drop */
    if (rng.rollChance(LootConfig::getEnemyPotionChance())) {
        Item potion;
        potion.name = "Health Potion";
        potion.type = ItemType::POTION;
        potion.value = constants::POTION_HEAL_AMOUNT;
        drop.items.push_back(potion);
    }

    /* Roll common weapon */
    if (rng.rollChance(LootConfig::getEnemyCommonWeaponChance())) {
        drop.weapons.push_back(generateCommonWeapon());
    }

    /* Roll rare weapon */
    if (rng.rollChance(LootConfig::getEnemyRareWeaponChance())) {
        drop.weapons.push_back(generateRareWeapon());
    }

    return drop;
}

LootDrop LootTable::generateBossLoot() {
    LootDrop drop;
    auto& rng = RandomManager::instance();

    /* Boss selalu drop gold besar */
    if (rng.rollChance(LootConfig::getBossGoldChance())) {
        drop.bonusGold = rng.getInt(constants::BOSS_GOLD_DROP_MIN,
                                     constants::BOSS_GOLD_DROP_MAX);
    }

    /* Roll potion */
    if (rng.rollChance(LootConfig::getBossPotionChance())) {
        Item potion;
        potion.name = "Greater Health Potion";
        potion.type = ItemType::POTION;
        potion.value = constants::POTION_HEAL_AMOUNT * 2;
        drop.items.push_back(potion);
    }

    /* Roll epic weapon */
    if (rng.rollChance(LootConfig::getBossEpicWeaponChance())) {
        drop.weapons.push_back(generateEpicWeapon());
    }

    /* Roll legendary weapon */
    if (rng.rollChance(LootConfig::getBossLegendaryWeaponChance())) {
        drop.weapons.push_back(generateLegendaryWeapon());
    }

    return drop;
}

Weapon LootTable::generateCommonWeapon() {
    auto& rng = RandomManager::instance();
    static const std::string names[] = {
        "Iron Sword", "Wooden Club", "Bronze Dagger", "Stone Axe"
    };
    int idx = rng.getInt(0, static_cast<int>(std::size(names)) - 1);
    int baseDmg = rng.getInt(3, 7);
    return Weapon(names[static_cast<size_t>(idx)], baseDmg, Rarity::COMMON);
}

Weapon LootTable::generateRareWeapon() {
    auto& rng = RandomManager::instance();
    static const std::string names[] = {
        "Steel Longsword", "Shadow Blade", "Flame Dagger", "Thunder Mace"
    };
    int idx = rng.getInt(0, static_cast<int>(std::size(names)) - 1);
    int baseDmg = rng.getInt(6, 12);
    return Weapon(names[static_cast<size_t>(idx)], baseDmg, Rarity::RARE);
}

Weapon LootTable::generateEpicWeapon() {
    auto& rng = RandomManager::instance();
    static const std::string names[] = {
        "Void Reaper", "Phoenix Blade", "Frost Bringer", "Storm Caller"
    };
    int idx = rng.getInt(0, static_cast<int>(std::size(names)) - 1);
    int baseDmg = rng.getInt(10, 18);
    return Weapon(names[static_cast<size_t>(idx)], baseDmg, Rarity::EPIC);
}

Weapon LootTable::generateLegendaryWeapon() {
    auto& rng = RandomManager::instance();
    static const std::string names[] = {
        "Excalibur", "Ragnarok", "Divine Wrath", "Eternal Edge"
    };
    int idx = rng.getInt(0, static_cast<int>(std::size(names)) - 1);
    int baseDmg = rng.getInt(18, 30);
    return Weapon(names[static_cast<size_t>(idx)], baseDmg, Rarity::LEGENDARY);
}

} // namespace roguelike
