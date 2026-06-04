/**
 * @file weapon.cpp
 * @brief Implementasi class Weapon.
 */

#include "weapon.h"
#include "../utility/constants.h"

#include <sstream>

namespace roguelike {

Weapon::Weapon()
    : name_("Fists")
    , baseDamage_(0)
    , upgradeLevel_(0)
    , rarity_(Rarity::COMMON)
{
}

Weapon::Weapon(const std::string& name, int baseDamage, Rarity rarity)
    : name_(name)
    , baseDamage_(baseDamage)
    , upgradeLevel_(0)
    , rarity_(rarity)
{
}

int Weapon::getTotalDamageBonus() const {
    int upgradeBonus = upgradeLevel_ * constants::WEAPON_UPGRADE_DAMAGE_GAIN;
    return baseDamage_ + getRarityBonus() + upgradeBonus;
}

const std::string& Weapon::getName() const {
    return name_;
}

int Weapon::getBaseDamage() const {
    return baseDamage_;
}

int Weapon::getUpgradeLevel() const {
    return upgradeLevel_;
}

Rarity Weapon::getRarity() const {
    return rarity_;
}

void Weapon::increaseUpgradeLevel() {
    ++upgradeLevel_;
}

int Weapon::getRarityBonus() const {
    switch (rarity_) {
        case Rarity::COMMON:    return constants::RARITY_COMMON_BONUS;
        case Rarity::RARE:      return constants::RARITY_RARE_BONUS;
        case Rarity::EPIC:      return constants::RARITY_EPIC_BONUS;
        case Rarity::LEGENDARY: return constants::RARITY_LEGENDARY_BONUS;
    }
    return constants::RARITY_COMMON_BONUS;
}

std::string Weapon::rarityToString(Rarity rarity) {
    switch (rarity) {
        case Rarity::COMMON:    return "COMMON";
        case Rarity::RARE:      return "RARE";
        case Rarity::EPIC:      return "EPIC";
        case Rarity::LEGENDARY: return "LEGENDARY";
    }
    return "COMMON";
}

Rarity Weapon::stringToRarity(const std::string& str) {
    if (str == "RARE")      return Rarity::RARE;
    if (str == "EPIC")      return Rarity::EPIC;
    if (str == "LEGENDARY") return Rarity::LEGENDARY;
    return Rarity::COMMON;
}

std::string Weapon::getDisplayName() const {
    std::ostringstream oss;
    oss << name_ << " [" << rarityToString(rarity_) << "]";
    if (upgradeLevel_ > 0) {
        oss << " +" << upgradeLevel_;
    }
    oss << " (DMG +" << getTotalDamageBonus() << ")";
    return oss.str();
}

std::string Weapon::serialize() const {
    return name_ + ":" + std::to_string(baseDamage_) + ":"
         + std::to_string(upgradeLevel_) + ":" + rarityToString(rarity_);
}

Weapon Weapon::deserialize(const std::string& str) {
    std::istringstream ss(str);
    std::string name, baseDmgStr, upgradeLvlStr, rarityStr;

    std::getline(ss, name, ':');
    std::getline(ss, baseDmgStr, ':');
    std::getline(ss, upgradeLvlStr, ':');
    std::getline(ss, rarityStr, ':');

    Weapon weapon(name, std::stoi(baseDmgStr), stringToRarity(rarityStr));
    int levels = std::stoi(upgradeLvlStr);
    for (int i = 0; i < levels; ++i) {
        weapon.increaseUpgradeLevel();
    }
    return weapon;
}

bool Weapon::isValid() const {
    return name_ != "Fists";
}

} // namespace roguelike
