/**
 * @file tile.h
 * @brief Definisi tipe tile pada map dungeon.
 *
 * Menggunakan enum class untuk type-safety.
 * Setiap TileType memiliki representasi karakter ASCII di constants.h.
 */

#pragma once

namespace roguelike {

/**
 * @brief Jenis tile yang tersedia pada map.
 */
enum class TileType {
    WALL,
    FLOOR,
    CHECKPOINT,
    STAIRS_DOWN,  // tangga ke lantai berikutnya
    STAIRS_UP     // tangga ke lantai sebelumnya
};

/**
 * @brief Mengecek apakah tile bisa dilewati oleh entity.
 */
inline bool isWalkable(TileType tile) {
    switch (tile) {
        case TileType::FLOOR:
        case TileType::CHECKPOINT:
        case TileType::STAIRS_DOWN:
        case TileType::STAIRS_UP:
            return true;
        case TileType::WALL:
            return false;
    }
    return false;
}

} // namespace roguelike
