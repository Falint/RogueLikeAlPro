/**
 * @file map_generator.cpp
 * @brief Implementasi prosedural dungeon generator.
 *
 * Algoritma: random room placement → connect rooms via L-shaped corridors
 * → place checkpoint → place stairs (kecuali lantai boss).
 */

#include "map_generator.h"
#include "../utility/random_manager.h"
#include "../utility/constants.h"

#include <cmath>
#include <algorithm>

namespace roguelike {

Map MapGenerator::generate(int floorIndex) {
    Map map;
    map.fill(TileType::WALL);

    auto& rng = RandomManager::instance();

    /* Jumlah room sedikit bertambah per lantai agar lebih kompleks */
    int roomCount = constants::MAX_ROOMS_PER_MAP + floorIndex;

    for (int i = 0; i < roomCount; ++i) {
        int w = rng.getInt(constants::MIN_ROOM_SIZE, constants::MAX_ROOM_SIZE);
        int h = rng.getInt(constants::MIN_ROOM_SIZE, constants::MAX_ROOM_SIZE);
        int x = rng.getInt(1, constants::MAP_WIDTH - w - 1);
        int y = rng.getInt(1, constants::MAP_HEIGHT - h - 1);

        Room newRoom{x, y, w, h};

        /* Cek overlap dengan room yang sudah ada */
        bool overlaps = false;
        for (const auto& existing : map.getRooms()) {
            if (newRoom.intersects(existing)) {
                overlaps = true;
                break;
            }
        }

        if (overlaps) {
            continue; // skip, coba room berikutnya
        }

        carveRoom(map, newRoom);

        /* Hubungkan dengan room sebelumnya */
        if (!map.getRooms().empty()) {
            connectRooms(map, map.getRooms().back(), newRoom);
        }

        map.addRoom(newRoom);
    }

    /* Pastikan minimal ada 2 room */
    if (map.getRooms().size() < 2) {
        Room fallbackA{2, 2, 6, 5};
        Room fallbackB{20, 10, 6, 5};
        carveRoom(map, fallbackA);
        map.addRoom(fallbackA);
        carveRoom(map, fallbackB);
        connectRooms(map, fallbackA, fallbackB);
        map.addRoom(fallbackB);
    }

    placeCheckpoint(map);

    /* Stairs hanya di lantai non-terakhir */
    if (floorIndex < constants::TOTAL_FLOORS - 1) {
        placeStairs(map);
    }

    return map;
}

void MapGenerator::carveRoom(Map& map, const Room& room) {
    for (int y = room.y; y < room.y + room.height; ++y) {
        for (int x = room.x; x < room.x + room.width; ++x) {
            map.setTile(x, y, TileType::FLOOR);
        }
    }
}

void MapGenerator::carveHorizontalCorridor(Map& map, int x1, int x2, int y) {
    int start = std::min(x1, x2);
    int end = std::max(x1, x2);
    for (int x = start; x <= end; ++x) {
        map.setTile(x, y, TileType::FLOOR);
    }
}

void MapGenerator::carveVerticalCorridor(Map& map, int y1, int y2, int x) {
    int start = std::min(y1, y2);
    int end = std::max(y1, y2);
    for (int y = start; y <= end; ++y) {
        map.setTile(x, y, TileType::FLOOR);
    }
}

void MapGenerator::connectRooms(Map& map, const Room& a, const Room& b) {
    Position ca = a.center();
    Position cb = b.center();

    /* L-shaped corridor: horizontal lalu vertikal, atau sebaliknya */
    if (RandomManager::instance().rollChance(50)) {
        carveHorizontalCorridor(map, ca.x, cb.x, ca.y);
        carveVerticalCorridor(map, ca.y, cb.y, cb.x);
    } else {
        carveVerticalCorridor(map, ca.y, cb.y, ca.x);
        carveHorizontalCorridor(map, ca.x, cb.x, cb.y);
    }
}

void MapGenerator::placeCheckpoint(Map& map) {
    const auto& rooms = map.getRooms();
    if (rooms.size() < 2) return;

    /* Tempatkan checkpoint di room tengah (bukan pertama atau terakhir) */
    size_t midIndex = rooms.size() / 2;
    Position pos = rooms[midIndex].center();
    map.setTile(pos, TileType::CHECKPOINT);
}

void MapGenerator::placeStairs(Map& map) {
    const auto& rooms = map.getRooms();
    if (rooms.empty()) return;

    /* Tempatkan stairs di room terakhir (terjauh dari spawn room pertama) */
    Position pos = rooms.back().center();
    map.setTile(pos, TileType::STAIRS);
}

} // namespace roguelike
