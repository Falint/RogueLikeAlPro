/**
 * @file combat_system.cpp
 * @brief Implementasi sistem combat.
 *
 * Damage formula: max(MIN_DAMAGE, attackerAttack - defenderDefense)
 * Player attack menggunakan getTotalAttack() yang mencakup weapon bonus.
 */

#include "combat_system.h"
#include "../utility/constants.h"

#include <algorithm>
#include <sstream>

namespace roguelike {

int CombatSystem::calculateDamage(int attackerAttack, int defenderDefense) {
    return std::max(constants::MIN_DAMAGE, attackerAttack - defenderDefense);
}

CombatResult CombatSystem::playerAttackEnemy(Player& player, Enemy& enemy) {
    CombatResult result;
    result.damageDealt = calculateDamage(player.getTotalAttack(), enemy.getDefense());

    enemy.takeDamage(result.damageDealt);
    result.targetDied = !enemy.isAlive();

    std::ostringstream oss;
    oss << "You hit " << enemy.getName() << " for " << result.damageDealt << " damage!";
    if (result.targetDied) {
        oss << " " << enemy.getName() << " defeated!";
    }
    result.message = oss.str();

    return result;
}

CombatResult CombatSystem::enemyAttackPlayer(Enemy& enemy, Player& player) {
    CombatResult result;
    result.damageDealt = calculateDamage(enemy.getAttack(), player.getDefense());

    player.takeDamage(result.damageDealt);
    result.targetDied = !player.isAlive();

    std::ostringstream oss;
    oss << enemy.getName() << " hits you for " << result.damageDealt << " damage!";
    if (result.targetDied) {
        oss << " You have been slain!";
    }
    result.message = oss.str();

    return result;
}

std::string CombatSystem::grantRewards(Player& player, const Enemy& enemy) {
    int goldReward = enemy.getGoldReward();
    int expReward = enemy.getExpReward();

    player.addGold(goldReward);
    player.addExperience(expReward);

    std::ostringstream oss;
    oss << "Gained " << goldReward << " gold and " << expReward << " EXP!";
    return oss.str();
}

} // namespace roguelike
