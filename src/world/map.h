/**
 * @file map.h
 * @brief Representasi map dungeon 2D.
 *
 * Map menyimpan grid tile dan menyediakan query untuk pathfinding,
 * collision detection, dan rendering.
 */

#pragma once

#include "tile.h"
#include "../utility/types.h"
#include "../utility/constants.h"

#include <vector>
#include <string>

namespace roguelike {

/**
 * @brief Struct untuk merepresentasikan sebuah room dalam dungeon.
 */
struct Room {
    int x;      ///< Posisi kiri atas (kolom)
    int y;      ///< Posisi kiri atas (baris)
    int width;
    int height;

    /** Mendapatkan posisi tengah room. */
    Position center() const {
        return {x + width / 2, y + height / 2};
    }

    /** Mengecek apakah dua room saling overlap. */
    bool intersects(const Room& other) const {
        return (x <= other.x + other.width && x + width >= other.x &&
                y <= other.y + other.height && y + height >= other.y);
    }
};

/**
 * @brief Map dungeon berukuran MAP_WIDTH x MAP_HEIGHT.
 */
class Map {
public:
    Map();

    /** @brief Mengisi seluruh map dengan tile WALL. */
    void fill(TileType type);

    /** @brief Mendapatkan tile pada posisi tertentu. */
    TileType getTile(int x, int y) const;
    TileType getTile(const Position& pos) const;

    /** @brief Mengatur tile pada posisi tertentu. */
    void setTile(int x, int y, TileType type);
    void setTile(const Position& pos, TileType type);

    /** @brief Mengecek apakah posisi valid dan bisa dilewati. */
    bool isWalkableAt(int x, int y) const;
    bool isWalkableAt(const Position& pos) const;

    /** @brief Mengecek apakah posisi berada dalam batas map. */
    bool isInBounds(int x, int y) const;
    bool isInBounds(const Position& pos) const;

    /** @brief Mendapatkan karakter render untuk tile pada posisi tertentu. */
    char getRenderChar(int x, int y) const;

    /** @brief Mendapatkan daftar room yang telah digenerate. */
    const std::vector<Room>& getRooms() const;

    /** @brief Menambahkan room ke daftar (digunakan oleh MapGenerator). */
    void addRoom(const Room& room);

    /** @brief Mendapatkan posisi stairs (tangga ke lantai berikutnya). */
    Position getStairsPosition() const;

    int getWidth() const;
    int getHeight() const;

private:
    std::vector<std::vector<TileType>> grid_;
    std::vector<Room> rooms_;
    int width_;
    int height_;
};

} // namespace roguelike
