/**
 * @file item.cpp
 * @brief Implementasi serialisasi dan konversi Item.
 */

#include "item.h"
#include <sstream>
#include <stdexcept>

namespace roguelike {

Item Item::fromString(const std::string& str) {
    /* Format: "TYPE:NAME:VALUE" */
    std::istringstream ss(str);
    std::string typeStr, name, valueStr;

    std::getline(ss, typeStr, ':');
    std::getline(ss, name, ':');
    std::getline(ss, valueStr, ':');

    Item item;
    item.type = stringToType(typeStr);
    item.name = name;
    item.value = std::stoi(valueStr);
    return item;
}

std::string Item::toString() const {
    return typeToString(type) + ":" + name + ":" + std::to_string(value);
}

std::string Item::typeToString(ItemType type) {
    switch (type) {
        case ItemType::POTION:   return "POTION";
        case ItemType::GOLD_BAG: return "GOLD_BAG";
    }
    return "UNKNOWN";
}

ItemType Item::stringToType(const std::string& str) {
    if (str == "POTION")   return ItemType::POTION;
    if (str == "GOLD_BAG") return ItemType::GOLD_BAG;
    return ItemType::POTION; // fallback aman
}

} // namespace roguelike
