/**
 * @file player.cpp
 * @brief Implementasi class Player.
 *
 * Sistem leveling: EXP yang dibutuhkan = level * EXP_PER_LEVEL.
 * Saat level up, stats meningkat berdasarkan konstanta.
 */

#include "player.h"
#include "../utility/constants.h"

namespace roguelike {

Player::Player()
    : Entity("Hero",
             constants::PLAYER_BASE_HP,
             constants::PLAYER_BASE_ATTACK,
             constants::PLAYER_BASE_DEFENSE,
             {0, 0},
             EntityType::PLAYER)
    , gold_(constants::PLAYER_START_GOLD)
    , experience_(constants::PLAYER_START_EXP)
    , level_(constants::PLAYER_START_LEVEL)
{
}

Player::Player(const std::string& name, Position pos)
    : Entity(name,
             constants::PLAYER_BASE_HP,
             constants::PLAYER_BASE_ATTACK,
             constants::PLAYER_BASE_DEFENSE,
             pos,
             EntityType::PLAYER)
    , gold_(constants::PLAYER_START_GOLD)
    , experience_(constants::PLAYER_START_EXP)
    , level_(constants::PLAYER_START_LEVEL)
{
}

void Player::addGold(int amount) {
    gold_ += amount;
}

bool Player::spendGold(int amount) {
    if (gold_ < amount) {
        return false;
    }
    gold_ -= amount;
    return true;
}

int Player::getGold() const { return gold_; }
void Player::setGold(int gold) { gold_ = gold; }

void Player::addExperience(int amount) {
    experience_ += amount;
    checkLevelUp();
}

int Player::getExperience() const { return experience_; }
int Player::getLevel() const { return level_; }

int Player::getExpToNextLevel() const {
    return level_ * constants::EXP_PER_LEVEL;
}

void Player::setExperience(int exp) { experience_ = exp; }
void Player::setLevel(int level) { level_ = level; }

Inventory& Player::getInventory() { return inventory_; }
const Inventory& Player::getInventory() const { return inventory_; }

Weapon& Player::getEquippedWeapon() { return equippedWeapon_; }
const Weapon& Player::getEquippedWeapon() const { return equippedWeapon_; }

void Player::equipWeapon(const Weapon& weapon) {
    /* Simpan senjata lama ke stored weapons jika valid */
    if (equippedWeapon_.isValid()) {
        storedWeapons_.push_back(equippedWeapon_);
    }
    equippedWeapon_ = weapon;
}

int Player::getTotalAttack() const {
    return attack_ + equippedWeapon_.getTotalDamageBonus();
}

void Player::storeWeapon(const Weapon& weapon) {
    storedWeapons_.push_back(weapon);
}

const std::vector<Weapon>& Player::getStoredWeapons() const {
    return storedWeapons_;
}

std::vector<Weapon>& Player::getStoredWeapons() {
    return storedWeapons_;
}

void Player::checkLevelUp() {
    while (experience_ >= getExpToNextLevel()) {
        experience_ -= getExpToNextLevel();
        ++level_;

        /* Tingkatkan stats berdasarkan konstanta */
        maxHp_    += constants::HP_GAIN_PER_LEVEL;
        hp_        = maxHp_; // full heal saat level up
        attack_   += constants::ATTACK_GAIN_PER_LEVEL;
        defense_  += constants::DEFENSE_GAIN_PER_LEVEL;
    }
}

} // namespace roguelike
