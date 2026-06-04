/**
 * @file checkpoint_system.cpp
 * @brief Implementasi CheckpointSystem.
 *
 * Saat pemain menginjak tile checkpoint:
 * 1. Simpan posisi sebagai respawn point
 * 2. Serialisasi seluruh state pemain
 * 3. Tulis ke file save
 */

#include "checkpoint_system.h"
#include "../utility/constants.h"

namespace roguelike {

CheckpointSystem::CheckpointSystem()
    : lastCheckpoint_{-1, -1}
    , checkpointFloor_(0)
    , hasCheckpoint_(false)
{
}

bool CheckpointSystem::checkAndSave(const Player& player, int currentFloor) {
    const Position& pos = player.getPosition();

    /* Simpan checkpoint */
    lastCheckpoint_ = pos;
    checkpointFloor_ = currentFloor;
    hasCheckpoint_ = true;

    /* Auto-save */
    GameSaveData data = createSaveData(player, currentFloor);
    return SaveManager::saveGame(data, constants::SAVE_FILENAME);
}

Position CheckpointSystem::getLastCheckpoint() const {
    return lastCheckpoint_;
}

bool CheckpointSystem::hasCheckpoint() const {
    return hasCheckpoint_;
}

int CheckpointSystem::getCheckpointFloor() const {
    return checkpointFloor_;
}

void CheckpointSystem::setCheckpoint(const Position& pos, int floor) {
    lastCheckpoint_ = pos;
    checkpointFloor_ = floor;
    hasCheckpoint_ = true;
}

GameSaveData CheckpointSystem::createSaveData(const Player& player, int currentFloor) const {
    GameSaveData data;
    data.saveVersion = constants::SAVE_VERSION;
    data.playerName = player.getName();
    data.hp = player.getHp();
    data.maxHp = player.getMaxHp();
    data.attack = player.getAttack();
    data.defense = player.getDefense();
    data.gold = player.getGold();
    data.experience = player.getExperience();
    data.level = player.getLevel();
    data.posX = player.getPosition().x;
    data.posY = player.getPosition().y;

    data.weaponData = player.getEquippedWeapon().serialize();

    for (const auto& w : player.getStoredWeapons()) {
        data.storedWeaponsData.push_back(w.serialize());
    }

    for (const auto& item : player.getInventory().getItems()) {
        data.inventoryData.push_back(item.toString());
    }

    data.checkpointX = lastCheckpoint_.x;
    data.checkpointY = lastCheckpoint_.y;
    data.currentFloor = currentFloor;

    return data;
}

} // namespace roguelike
