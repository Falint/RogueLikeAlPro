/**
 * @file item.h
 * @brief Definisi item yang dapat disimpan dalam inventory.
 *
 * Item mencakup potion, gold bag, dan item lainnya (senjata
 * ditangani terpisah oleh modul weapon).
 */

#pragma once

#include <string>

namespace roguelike {

/**
 * @brief Jenis item dalam inventory.
 */
enum class ItemType {
    POTION,     ///< Mengembalikan HP pemain
    GOLD_BAG    ///< Kantong emas yang bisa di-pickup
};

/**
 * @brief Sebuah item dalam game.
 */
struct Item {
    std::string name;
    ItemType type;
    int value;  ///< Untuk POTION: jumlah heal. Untuk GOLD_BAG: jumlah gold.

    /**
     * @brief Membuat item dari string serialisasi.
     * Format: "TYPE:NAME:VALUE"
     */
    static Item fromString(const std::string& str);

    /**
     * @brief Mengkonversi item ke string untuk serialisasi.
     */
    std::string toString() const;

    /**
     * @brief Mendapatkan nama tipe item sebagai string.
     */
    static std::string typeToString(ItemType type);

    /**
     * @brief Mengkonversi string ke ItemType.
     */
    static ItemType stringToType(const std::string& str);
};

} // namespace roguelike
