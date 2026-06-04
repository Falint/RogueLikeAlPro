/**
 * @file enemy.cpp
 * @brief Implementasi class Enemy.
 *
 * Stats di-scale: baseStat * pow(SCALE_PER_FLOOR, floorIndex).
 */

#include "enemy.h"
#include "../utility/constants.h"

#include <cmath>
#include <string>

namespace roguelike {

Enemy::Enemy(Position pos, int floorIndex)
    : Entity(
        "Goblin",
        constants::ENEMY_BASE_HP + (constants::ENEMY_HP_ADD_PER_FLOOR * floorIndex),
        constants::ENEMY_BASE_ATTACK + (constants::ENEMY_ATTACK_ADD_PER_FLOOR * floorIndex),
        constants::ENEMY_BASE_DEFENSE + (constants::ENEMY_DEFENSE_ADD_PER_FLOOR * floorIndex),
        pos,
        EntityType::ENEMY)
    , expReward_(constants::ENEMY_BASE_EXP + (constants::ENEMY_EXP_ADD_PER_FLOOR * floorIndex))
    , goldReward_(constants::ENEMY_BASE_GOLD + (constants::ENEMY_GOLD_ADD_PER_FLOOR * floorIndex))
    , floorIndex_(floorIndex)
{
    /* Beri nama berdasarkan floor untuk variasi */
    static const std::string names[] = {"Goblin", "Orc", "Dark Knight", "Troll", "Vampire"};
    if (floorIndex >= 0) {
        name_ = names[floorIndex % std::size(names)];
    }
}

int Enemy::getExpReward() const { return expReward_; }
int Enemy::getGoldReward() const { return goldReward_; }
int Enemy::getFloorIndex() const { return floorIndex_; }

} // namespace roguelike
