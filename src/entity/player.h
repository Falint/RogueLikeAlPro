/**
 * @file player.h
 * @brief Kelas Player — entity yang dikendalikan pemain.
 *
 * Player memiliki gold, EXP, level, inventory, dan equipped weapon.
 * Level-up otomatis saat EXP mencukupi.
 */

#pragma once

#include "entity.h"
#include "../inventory/inventory.h"
#include "../weapon/weapon.h"

namespace roguelike {

/**
 * @brief Entity pemain dengan sistem leveling dan inventory.
 */
class Player : public Entity {
public:
    Player();
    Player(const std::string& name, Position pos);

    // ─── Gold ────────────────────────────────────────────────────────────
    void addGold(int amount);
    bool spendGold(int amount);
    int getGold() const;
    void setGold(int gold);

    // ─── Experience & Level ──────────────────────────────────────────────
    void addExperience(int amount);
    int getExperience() const;
    int getLevel() const;
    int getExpToNextLevel() const;
    void setExperience(int exp);
    void setLevel(int level);

    // ─── Inventory ───────────────────────────────────────────────────────
    Inventory& getInventory();
    const Inventory& getInventory() const;

    // ─── Weapon ──────────────────────────────────────────────────────────
    Weapon& getEquippedWeapon();
    const Weapon& getEquippedWeapon() const;
    void equipWeapon(const Weapon& weapon);

    /** @brief Total attack = base attack + weapon damage bonus. */
    int getTotalAttack() const;

    // ─── Stored Weapons (inventory senjata terpisah) ─────────────────────
    void storeWeapon(const Weapon& weapon);
    const std::vector<Weapon>& getStoredWeapons() const;
    std::vector<Weapon>& getStoredWeapons();

private:
    /** @brief Proses level-up jika EXP cukup. Bisa naik beberapa level sekaligus. */
    void checkLevelUp();

    int gold_;
    int experience_;
    int level_;
    Inventory inventory_;
    Weapon equippedWeapon_;
    std::vector<Weapon> storedWeapons_;
};

} // namespace roguelike
