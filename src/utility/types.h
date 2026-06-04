/**
 * @file types.h
 * @brief Tipe data umum yang digunakan di seluruh codebase.
 *
 * Menyediakan struct Position, enum Direction, dan alias tipe lainnya
 * agar konsisten di semua modul.
 */

#pragma once

#include <string>

namespace roguelike {

/**
 * @brief Posisi 2D pada map (kolom x, baris y).
 */
struct Position {
    int x = 0;
    int y = 0;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

/**
 * @brief Arah pergerakan pemain.
 */
enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

/**
 * @brief State utama game loop.
 */
enum class GameState {
    PLAYING,
    INVENTORY_MENU,
    WEAPON_MENU,
    UPGRADE_MENU,
    GAME_OVER,
    VICTORY,
    QUIT
};

/**
 * @brief Mengembalikan offset Position berdasarkan Direction.
 */
inline Position directionToOffset(Direction dir) {
    switch (dir) {
        case Direction::UP:    return {0, -1};
        case Direction::DOWN:  return {0,  1};
        case Direction::LEFT:  return {-1, 0};
        case Direction::RIGHT: return {1,  0};
        case Direction::NONE:  return {0,  0};
    }
    return {0, 0};
}

} // namespace roguelike
