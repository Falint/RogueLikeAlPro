/**
 * @file serializer.h
 * @brief Serialisasi dan deserialisasi game state ke format teks.
 *
 * Format: Section-based text file dengan pola [SECTION] dan KEY=VALUE.
 * Dipilih karena ringan, mudah dibaca manusia, dan mudah diperluas.
 */

#pragma once

#include "../entity/player.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace roguelike {

/**
 * @brief Data yang perlu disimpan/dimuat.
 */
struct GameSaveData {
    /* Player */
    std::string playerName;
    int hp = 0;
    int maxHp = 0;
    int attack = 0;
    int defense = 0;
    int gold = 0;
    int experience = 0;
    int level = 0;
    int posX = 0;
    int posY = 0;

    /* Weapon yang di-equip */
    std::string weaponData; // serialized weapon string

    /* Stored weapons */
    std::vector<std::string> storedWeaponsData;

    /* Inventory items */
    std::vector<std::string> inventoryData;

    /* Checkpoint & Floor */
    int checkpointX = -1;
    int checkpointY = -1;
    int currentFloor = 0;

    /* Meta */
    int saveVersion = 0;
};

/**
 * @brief Serializer untuk konversi GameSaveData ↔ format teks.
 */
class Serializer {
public:
    /**
     * @brief Mengkonversi GameSaveData menjadi string untuk ditulis ke file.
     */
    static std::string serialize(const GameSaveData& data);

    /**
     * @brief Mengkonversi string dari file menjadi GameSaveData.
     * @return nullopt jika format tidak valid.
     */
    static std::optional<GameSaveData> deserialize(const std::string& content);

private:
    /** @brief Parse satu section dari content. */
    static std::unordered_map<std::string, std::string>
    parseSection(const std::string& content, const std::string& sectionName);

    /** @brief Mendapatkan semua line di bawah section dengan prefix tertentu. */
    static std::vector<std::string>
    parseSectionList(const std::string& content,
                     const std::string& sectionName,
                     const std::string& prefix);
};

} // namespace roguelike
