/**
 * @file map.cpp
 * @brief Implementasi Map — grid 2D tile dungeon.
 */

#include "map.h"

namespace roguelike {

Map::Map()
    : width_(constants::MAP_WIDTH)
    , height_(constants::MAP_HEIGHT)
{
    grid_.resize(static_cast<size_t>(height_),
                 std::vector<TileType>(static_cast<size_t>(width_), TileType::WALL));
}

void Map::fill(TileType type) {
    for (auto& row : grid_) {
        for (auto& tile : row) {
            tile = type;
        }
    }
}

TileType Map::getTile(int x, int y) const {
    if (!isInBounds(x, y)) {
        return TileType::WALL;
    }
    return grid_[static_cast<size_t>(y)][static_cast<size_t>(x)];
}

TileType Map::getTile(const Position& pos) const {
    return getTile(pos.x, pos.y);
}

void Map::setTile(int x, int y, TileType type) {
    if (isInBounds(x, y)) {
        grid_[static_cast<size_t>(y)][static_cast<size_t>(x)] = type;
    }
}

void Map::setTile(const Position& pos, TileType type) {
    setTile(pos.x, pos.y, type);
}

bool Map::isWalkableAt(int x, int y) const {
    return isInBounds(x, y) && isWalkable(getTile(x, y));
}

bool Map::isWalkableAt(const Position& pos) const {
    return isWalkableAt(pos.x, pos.y);
}

bool Map::isInBounds(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

bool Map::isInBounds(const Position& pos) const {
    return isInBounds(pos.x, pos.y);
}

char Map::getRenderChar(int x, int y) const {
    TileType tile = getTile(x, y);
    switch (tile) {
        case TileType::WALL:       return constants::TILE_WALL;
        case TileType::FLOOR:      return constants::TILE_FLOOR;
        case TileType::CHECKPOINT: return constants::TILE_CHECKPOINT;
        case TileType::STAIRS_DOWN: return constants::TILE_STAIRS_DOWN;
        case TileType::STAIRS_UP:   return constants::TILE_STAIRS_UP;
    }
    return constants::TILE_FLOOR;
}

const std::vector<Room>& Map::getRooms() const {
    return rooms_;
}

void Map::addRoom(const Room& room) {
    rooms_.push_back(room);
}

Position Map::getStairsUpPosition() const {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            if (grid_[static_cast<size_t>(y)][static_cast<size_t>(x)] == TileType::STAIRS_UP) {
                return {x, y};
            }
        }
    }
    return {-1, -1};
}

Position Map::getStairsDownPosition() const {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            if (grid_[static_cast<size_t>(y)][static_cast<size_t>(x)] == TileType::STAIRS_DOWN) {
                return {x, y};
            }
        }
    }
    return {-1, -1};
}

int Map::getWidth() const {
    return width_;
}

int Map::getHeight() const {
    return height_;
}

} // namespace roguelike
