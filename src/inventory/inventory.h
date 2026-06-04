/**
 * @file inventory.h
 * @brief Sistem inventory pemain dengan kapasitas terbatas.
 *
 * Menggunakan std::vector<Item> sebagai container utama.
 * Mendukung add, remove, use, display, dan serialisasi.
 */

#pragma once

#include "item.h"
#include "../utility/constants.h"

#include <vector>
#include <string>
#include <optional>

namespace roguelike {

/**
 * @brief Inventory pemain untuk menyimpan item.
 */
class Inventory {
public:
    Inventory();

    /**
     * @brief Menambahkan item ke inventory.
     * @return true jika berhasil, false jika penuh.
     */
    bool addItem(const Item& item);

    /**
     * @brief Menghapus item pada indeks tertentu.
     * @return Item yang dihapus, atau nullopt jika indeks tidak valid.
     */
    std::optional<Item> removeItem(size_t index);

    /**
     * @brief Menggunakan item pada indeks tertentu.
     * @return Nilai efek item (misalnya jumlah heal), atau 0 jika gagal.
     */
    int useItem(size_t index);

    /** @brief Mendapatkan jumlah item saat ini. */
    size_t getItemCount() const;

    /** @brief Mengecek apakah inventory penuh. */
    bool isFull() const;

    /** @brief Mengecek apakah inventory kosong. */
    bool isEmpty() const;

    /** @brief Mendapatkan item pada indeks tertentu (read-only). */
    const Item& getItem(size_t index) const;

    /** @brief Mendapatkan seluruh item (read-only). */
    const std::vector<Item>& getItems() const;

    /** @brief Menghapus semua item. */
    void clear();

    /** @brief Mendapatkan representasi string dari seluruh inventory. */
    std::string getDisplayString() const;

private:
    std::vector<Item> items_;
    size_t maxCapacity_;
};

} // namespace roguelike
