/**
 * @file weapon_upgrade.cpp
 * @brief Implementasi sistem upgrade senjata.
 *
 * Rumus biaya: BASE_COST + currentLevel * COST_FACTOR
 * Damage gain per level: WEAPON_UPGRADE_DAMAGE_GAIN (dari constants)
 */

#include "weapon_upgrade.h"
#include "../utility/constants.h"

#include <sstream>

namespace roguelike {

int WeaponUpgrade::getUpgradeCost(int currentLevel) {
    return constants::WEAPON_UPGRADE_BASE_COST
         + currentLevel * constants::WEAPON_UPGRADE_COST_FACTOR;
}

bool WeaponUpgrade::tryUpgrade(Weapon& weapon, int& playerGold) {
    if (isMaxLevel(weapon)) {
        return false;
    }

    int cost = getUpgradeCost(weapon.getUpgradeLevel());
    if (playerGold < cost) {
        return false;
    }

    playerGold -= cost;
    weapon.increaseUpgradeLevel();
    return true;
}

bool WeaponUpgrade::isMaxLevel(const Weapon& weapon) {
    return weapon.getUpgradeLevel() >= constants::WEAPON_MAX_UPGRADE_LEVEL;
}

std::string WeaponUpgrade::getUpgradeInfo(const Weapon& weapon, int playerGold) {
    std::ostringstream oss;
    oss << "  Weapon: " << weapon.getDisplayName() << "\n";

    if (isMaxLevel(weapon)) {
        oss << "  Status: MAX LEVEL\n";
    } else {
        int cost = getUpgradeCost(weapon.getUpgradeLevel());
        oss << "  Upgrade Cost: " << cost << " gold\n";
        oss << "  Your Gold: " << playerGold << "\n";
        oss << "  Damage after upgrade: +"
            << (weapon.getTotalDamageBonus() + constants::WEAPON_UPGRADE_DAMAGE_GAIN) << "\n";

        if (playerGold >= cost) {
            oss << "  Press [U] to upgrade\n";
        } else {
            oss << "  Not enough gold!\n";
        }
    }

    return oss.str();
}

} // namespace roguelike
