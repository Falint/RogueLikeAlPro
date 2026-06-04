/**
 * @file boss.cpp
 * @brief Implementasi class Boss.
 *
 * Boss stats = enemy stats (sudah di-scale per floor) × boss multiplier.
 */

#include "boss.h"
#include "../utility/constants.h"

namespace roguelike {

Boss::Boss(Position pos, int floorIndex)
    : Enemy(pos, floorIndex)
{
    name_ = "Dragon Lord";
    type_ = EntityType::BOSS;

    /* Kalikan stats dengan boss multiplier */
    maxHp_   = static_cast<int>(maxHp_ * constants::BOSS_HP_MULTIPLIER);
    hp_      = maxHp_;
    attack_  = static_cast<int>(attack_ * constants::BOSS_ATTACK_MULTIPLIER);
    defense_ = static_cast<int>(defense_ * constants::BOSS_DEFENSE_MULTIPLIER);
    expReward_ = static_cast<int>(expReward_ * constants::BOSS_EXP_MULTIPLIER);
    goldReward_ = static_cast<int>(goldReward_ * constants::BOSS_GOLD_MULTIPLIER);
}

bool Boss::isBoss() const {
    return true;
}

} // namespace roguelike
