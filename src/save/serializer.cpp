/**
 * @file serializer.cpp
 * @brief Implementasi Serializer untuk format save teks.
 *
 * Format file:
 *   [HEADER]
 *   VERSION=1
 *   [PLAYER]
 *   NAME=Hero
 *   HP=80
 *   ...
 *   [WEAPON]
 *   DATA=SwordName:10:2:RARE
 *   [STORED_WEAPONS]
 *   WEAPON_0=...
 *   WEAPON_1=...
 *   [INVENTORY]
 *   ITEM_0=POTION:Health Potion:30
 *   ITEM_1=...
 *   [CHECKPOINT]
 *   POS_X=10
 *   POS_Y=7
 *   FLOOR=2
 */

#include "serializer.h"
#include "../utility/constants.h"

#include <sstream>

namespace roguelike {

std::string Serializer::serialize(const GameSaveData& data) {
    std::ostringstream oss;

    oss << "[HEADER]\n";
    oss << "VERSION=" << constants::SAVE_VERSION << "\n";

    oss << "[PLAYER]\n";
    oss << "NAME=" << data.playerName << "\n";
    oss << "HP=" << data.hp << "\n";
    oss << "MAX_HP=" << data.maxHp << "\n";
    oss << "ATTACK=" << data.attack << "\n";
    oss << "DEFENSE=" << data.defense << "\n";
    oss << "GOLD=" << data.gold << "\n";
    oss << "EXP=" << data.experience << "\n";
    oss << "LEVEL=" << data.level << "\n";
    oss << "POS_X=" << data.posX << "\n";
    oss << "POS_Y=" << data.posY << "\n";

    oss << "[WEAPON]\n";
    oss << "DATA=" << data.weaponData << "\n";

    oss << "[STORED_WEAPONS]\n";
    oss << "COUNT=" << data.storedWeaponsData.size() << "\n";
    for (size_t i = 0; i < data.storedWeaponsData.size(); ++i) {
        oss << "WEAPON_" << i << "=" << data.storedWeaponsData[i] << "\n";
    }

    oss << "[INVENTORY]\n";
    oss << "COUNT=" << data.inventoryData.size() << "\n";
    for (size_t i = 0; i < data.inventoryData.size(); ++i) {
        oss << "ITEM_" << i << "=" << data.inventoryData[i] << "\n";
    }

    oss << "[CHECKPOINT]\n";
    oss << "POS_X=" << data.checkpointX << "\n";
    oss << "POS_Y=" << data.checkpointY << "\n";
    oss << "FLOOR=" << data.currentFloor << "\n";

    return oss.str();
}

std::optional<GameSaveData> Serializer::deserialize(const std::string& content) {
    if (content.empty()) {
        return std::nullopt;
    }

    /* Validasi header */
    auto headerFields = parseSection(content, "HEADER");
    if (headerFields.find("VERSION") == headerFields.end()) {
        return std::nullopt;
    }

    try {
        int version = std::stoi(headerFields.at("VERSION"));
        if (version != constants::SAVE_VERSION) {
            return std::nullopt;
        }
    } catch (...) {
        return std::nullopt;
    }

    GameSaveData data;
    data.saveVersion = constants::SAVE_VERSION;

    /* Parse player section */
    auto playerFields = parseSection(content, "PLAYER");
    try {
        data.playerName = playerFields.count("NAME") ? playerFields.at("NAME") : "Hero";
        data.hp         = playerFields.count("HP")      ? std::stoi(playerFields.at("HP"))      : 0;
        data.maxHp      = playerFields.count("MAX_HP")  ? std::stoi(playerFields.at("MAX_HP"))  : 0;
        data.attack     = playerFields.count("ATTACK")  ? std::stoi(playerFields.at("ATTACK"))  : 0;
        data.defense    = playerFields.count("DEFENSE") ? std::stoi(playerFields.at("DEFENSE")) : 0;
        data.gold       = playerFields.count("GOLD")    ? std::stoi(playerFields.at("GOLD"))    : 0;
        data.experience = playerFields.count("EXP")     ? std::stoi(playerFields.at("EXP"))     : 0;
        data.level      = playerFields.count("LEVEL")   ? std::stoi(playerFields.at("LEVEL"))   : 1;
        data.posX       = playerFields.count("POS_X")   ? std::stoi(playerFields.at("POS_X"))   : 0;
        data.posY       = playerFields.count("POS_Y")   ? std::stoi(playerFields.at("POS_Y"))   : 0;
    } catch (...) {
        return std::nullopt;
    }

    /* Parse weapon */
    auto weaponFields = parseSection(content, "WEAPON");
    data.weaponData = weaponFields.count("DATA") ? weaponFields.at("DATA") : "";

    /* Parse stored weapons */
    auto storedFields = parseSection(content, "STORED_WEAPONS");
    int storedCount = 0;
    try {
        storedCount = storedFields.count("COUNT") ? std::stoi(storedFields.at("COUNT")) : 0;
    } catch (...) {
        storedCount = 0;
    }
    for (int i = 0; i < storedCount; ++i) {
        std::string key = "WEAPON_" + std::to_string(i);
        if (storedFields.count(key)) {
            data.storedWeaponsData.push_back(storedFields.at(key));
        }
    }

    /* Parse inventory */
    auto invFields = parseSection(content, "INVENTORY");
    int invCount = 0;
    try {
        invCount = invFields.count("COUNT") ? std::stoi(invFields.at("COUNT")) : 0;
    } catch (...) {
        invCount = 0;
    }
    for (int i = 0; i < invCount; ++i) {
        std::string key = "ITEM_" + std::to_string(i);
        if (invFields.count(key)) {
            data.inventoryData.push_back(invFields.at(key));
        }
    }

    /* Parse checkpoint */
    auto checkFields = parseSection(content, "CHECKPOINT");
    try {
        data.checkpointX  = checkFields.count("POS_X") ? std::stoi(checkFields.at("POS_X")) : -1;
        data.checkpointY  = checkFields.count("POS_Y") ? std::stoi(checkFields.at("POS_Y")) : -1;
        data.currentFloor = checkFields.count("FLOOR")  ? std::stoi(checkFields.at("FLOOR"))  : 0;
    } catch (...) {
        data.checkpointX = -1;
        data.checkpointY = -1;
        data.currentFloor = 0;
    }

    return data;
}

std::unordered_map<std::string, std::string>
Serializer::parseSection(const std::string& content, const std::string& sectionName) {
    std::unordered_map<std::string, std::string> fields;
    std::istringstream stream(content);
    std::string line;
    bool inSection = false;
    std::string sectionTag = "[" + sectionName + "]";

    while (std::getline(stream, line)) {
        /* Trim whitespace */
        while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) {
            line.pop_back();
        }

        if (line == sectionTag) {
            inSection = true;
            continue;
        }

        /* Jika bertemu section lain, keluar */
        if (!line.empty() && line.front() == '[' && line.back() == ']') {
            if (inSection) break;
            continue;
        }

        if (inSection) {
            auto eqPos = line.find('=');
            if (eqPos != std::string::npos) {
                std::string key = line.substr(0, eqPos);
                std::string value = line.substr(eqPos + 1);
                fields[key] = value;
            }
        }
    }

    return fields;
}

std::vector<std::string>
Serializer::parseSectionList(const std::string& content,
                              const std::string& sectionName,
                              const std::string& prefix) {
    std::vector<std::string> result;
    auto fields = parseSection(content, sectionName);
    for (int i = 0; ; ++i) {
        std::string key = prefix + std::to_string(i);
        auto it = fields.find(key);
        if (it == fields.end()) break;
        result.push_back(it->second);
    }
    return result;
}

} // namespace roguelike
