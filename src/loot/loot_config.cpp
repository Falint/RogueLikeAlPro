/**
 * @file loot_config.cpp
 * @brief Implementasi LootConfig — semua nilai dari constants.h.
 */

#include "loot_config.h"
#include "../utility/constants.h"

namespace roguelike {

int LootConfig::getEnemyGoldChance()         { return constants::LOOT_ENEMY_GOLD_CHANCE; }
int LootConfig::getEnemyPotionChance()       { return constants::LOOT_ENEMY_POTION_CHANCE; }
int LootConfig::getEnemyCommonWeaponChance() { return constants::LOOT_ENEMY_COMMON_WPN_CHANCE; }
int LootConfig::getEnemyRareWeaponChance()   { return constants::LOOT_ENEMY_RARE_WPN_CHANCE; }

int LootConfig::getBossGoldChance()              { return constants::LOOT_BOSS_GOLD_CHANCE; }
int LootConfig::getBossEpicWeaponChance()        { return constants::LOOT_BOSS_EPIC_WPN_CHANCE; }
int LootConfig::getBossLegendaryWeaponChance()   { return constants::LOOT_BOSS_LEGENDARY_WPN_CHANCE; }
int LootConfig::getBossPotionChance()            { return constants::LOOT_BOSS_POTION_CHANCE; }

} // namespace roguelike
