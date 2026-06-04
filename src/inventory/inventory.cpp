/**
 * @file inventory.cpp
 * @brief Implementasi sistem inventory.
 */

#include "inventory.h"
#include <sstream>

namespace roguelike {

Inventory::Inventory()
    : maxCapacity_(static_cast<size_t>(constants::INVENTORY_MAX_CAPACITY))
{
}

bool Inventory::addItem(const Item& item) {
    if (isFull()) {
        return false;
    }
    items_.push_back(item);
    return true;
}

std::optional<Item> Inventory::removeItem(size_t index) {
    if (index >= items_.size()) {
        return std::nullopt;
    }
    Item removed = items_[index];
    items_.erase(items_.begin() + static_cast<std::ptrdiff_t>(index));
    return removed;
}

int Inventory::useItem(size_t index) {
    if (index >= items_.size()) {
        return 0;
    }

    Item item = items_[index];
    int effectValue = 0;

    switch (item.type) {
        case ItemType::POTION:
            effectValue = item.value;
            break;
        case ItemType::GOLD_BAG:
            effectValue = item.value;
            break;
    }

    /* Hapus item setelah digunakan */
    items_.erase(items_.begin() + static_cast<std::ptrdiff_t>(index));
    return effectValue;
}

size_t Inventory::getItemCount() const {
    return items_.size();
}

bool Inventory::isFull() const {
    return items_.size() >= maxCapacity_;
}

bool Inventory::isEmpty() const {
    return items_.empty();
}

const Item& Inventory::getItem(size_t index) const {
    return items_.at(index);
}

const std::vector<Item>& Inventory::getItems() const {
    return items_;
}

void Inventory::clear() {
    items_.clear();
}

std::string Inventory::getDisplayString() const {
    if (items_.empty()) {
        return "  (empty)";
    }

    std::ostringstream oss;
    for (size_t i = 0; i < items_.size(); ++i) {
        oss << "  [" << i << "] " << items_[i].name;
        switch (items_[i].type) {
            case ItemType::POTION:
                oss << " (Heal +" << items_[i].value << ")";
                break;
            case ItemType::GOLD_BAG:
                oss << " (Gold +" << items_[i].value << ")";
                break;
        }
        oss << "\n";
    }
    return oss.str();
}

} // namespace roguelike
