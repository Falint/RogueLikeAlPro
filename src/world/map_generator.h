/**
 * @file map_generator.h
 * @brief Prosedural dungeon generator menggunakan algoritma room placement.
 *
 * Menghasilkan dungeon dengan room-room yang terhubung oleh koridor.
 * Menempatkan checkpoint, stairs, dan spawn point.
 */

#pragma once

#include "map.h"

namespace roguelike {

/**
 * @brief Generator map prosedural untuk dungeon multi-lantai.
 */
class MapGenerator {
public:
    /**
     * @brief Menghasilkan map untuk floor tertentu.
     * @param floorIndex Indeks lantai (0 = easy, 1 = medium, 2 = boss).
     * @return Map yang telah digenerate.
     */
    static Map generate(int floorIndex);

private:
    /** @brief Membuat sebuah room di dalam map. */
    static void carveRoom(Map& map, const Room& room);

    /** @brief Membuat koridor horizontal antara dua titik. */
    static void carveHorizontalCorridor(Map& map, int x1, int x2, int y);

    /** @brief Membuat koridor vertikal antara dua titik. */
    static void carveVerticalCorridor(Map& map, int y1, int y2, int x);

    /** @brief Menghubungkan dua room dengan koridor berbentuk L. */
    static void connectRooms(Map& map, const Room& a, const Room& b);

    /** @brief Menempatkan checkpoint di room acak. */
    static void placeCheckpoint(Map& map);

    /** @brief Menempatkan stairs di room terjauh dari room pertama. */
    static void placeStairs(Map& map);
};

} // namespace roguelike
