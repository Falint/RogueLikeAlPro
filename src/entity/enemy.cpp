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
        static_cast<int>(constants::ENEMY_BASE_HP * std::pow(constants::ENEMY_HP_SCALE_PER_FLOOR, floorIndex)),
        static_cast<int>(constants::ENEMY_BASE_ATTACK * std::pow(constants::ENEMY_ATTACK_SCALE_PER_FLOOR, floorIndex)),
        static_cast<int>(constants::ENEMY_BASE_DEFENSE * std::pow(constants::ENEMY_DEFENSE_SCALE_PER_FLOOR, floorIndex)),
        pos,
        EntityType::ENEMY)
    , expReward_(static_cast<int>(constants::ENEMY_BASE_EXP * std::pow(constants::ENEMY_EXP_SCALE_PER_FLOOR, floorIndex)))
    , goldReward_(static_cast<int>(constants::ENEMY_BASE_GOLD * std::pow(constants::ENEMY_GOLD_SCALE_PER_FLOOR, floorIndex)))
    , floorIndex_(floorIndex)
{
    /* Beri nama berdasarkan floor untuk variasi */
    static const std::string names[] = {"Goblin", "Orc", "Dark Knight"};
    if (floorIndex >= 0 && floorIndex < static_cast<int>(std::size(names))) {
        name_ = names[static_cast<size_t>(floorIndex)];
    }
}

int Enemy::getExpReward() const { return expReward_; }
int Enemy::getGoldReward() const { return goldReward_; }
int Enemy::getFloorIndex() const { return floorIndex_; }

} // namespace roguelike
