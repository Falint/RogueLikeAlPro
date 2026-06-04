/**
 * @file weapon.h
 * @brief Sistem senjata dengan rarity tier dan upgrade level.
 *
 * Setiap senjata memiliki nama, damage bonus, upgrade level, dan rarity.
 * Senjata dapat di-equip, di-swap, dan di-upgrade.
 */

#pragma once

#include <string>

namespace roguelike {

/**
 * @brief Tingkat kelangkaan senjata.
 */
enum class Rarity {
    COMMON,
    RARE,
    EPIC,
    LEGENDARY
};

/**
 * @brief Sebuah senjata dalam game.
 */
class Weapon {
public:
    Weapon();
    Weapon(const std::string& name, int baseDamage, Rarity rarity);

    /** @brief Mendapatkan total damage bonus (base + upgrade + rarity). */
    int getTotalDamageBonus() const;

    /** @brief Mendapatkan nama senjata. */
    const std::string& getName() const;

    /** @brief Mendapatkan base damage. */
    int getBaseDamage() const;

    /** @brief Mendapatkan upgrade level saat ini. */
    int getUpgradeLevel() const;

    /** @brief Mendapatkan rarity. */
    Rarity getRarity() const;

    /** @brief Menaikkan upgrade level (dipanggil oleh WeaponUpgrade). */
    void increaseUpgradeLevel();

    /** @brief Mendapatkan bonus damage dari rarity. */
    int getRarityBonus() const;

    /** @brief Konversi rarity ke string. */
    static std::string rarityToString(Rarity rarity);

    /** @brief Konversi string ke rarity. */
    static Rarity stringToRarity(const std::string& str);

    /** @brief Mendapatkan nama tampilan lengkap (nama + level + rarity). */
    std::string getDisplayName() const;

    /** @brief Serialisasi ke string. Format: "NAME:BASE_DMG:UPGRADE_LVL:RARITY" */
    std::string serialize() const;

    /** @brief Deserialisasi dari string. */
    static Weapon deserialize(const std::string& str);

    /** @brief Mengecek apakah senjata valid (bukan senjata kosong). */
    bool isValid() const;

private:
    std::string name_;
    int baseDamage_;
    int upgradeLevel_;
    Rarity rarity_;
};

} // namespace roguelike
