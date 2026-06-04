/**
 * @file checkpoint_system.h
 * @brief Sistem checkpoint — auto-save saat pemain menginjak tile checkpoint.
 *
 * Menyimpan seluruh progress: posisi, inventory, gold, senjata, statistik.
 * Checkpoint terakhir menjadi titik respawn saat game over.
 */

#pragma once

#include "../entity/player.h"
#include "../save/save_manager.h"
#include "../utility/types.h"

namespace roguelike {

/**
 * @brief Mengelola checkpoint dan auto-save.
 */
class CheckpointSystem {
public:
    CheckpointSystem();

    /**
     * @brief Memeriksa apakah pemain ada di tile checkpoint dan melakukan auto-save.
     * @param player Player saat ini.
     * @param currentFloor Floor saat ini.
     * @return true jika checkpoint diaktivasi dan save berhasil.
     */
    bool checkAndSave(const Player& player, int currentFloor);

    /**
     * @brief Mendapatkan posisi checkpoint terakhir.
     */
    Position getLastCheckpoint() const;

    /**
     * @brief Mengecek apakah ada checkpoint yang tersimpan.
     */
    bool hasCheckpoint() const;

    /**
     * @brief Mendapatkan floor dari checkpoint terakhir.
     */
    int getCheckpointFloor() const;

    /**
     * @brief Mengatur checkpoint manual (saat load game).
     */
    void setCheckpoint(const Position& pos, int floor);

private:
    /**
     * @brief Membuat GameSaveData dari state player saat ini.
     */
    GameSaveData createSaveData(const Player& player, int currentFloor) const;

    Position lastCheckpoint_;
    int checkpointFloor_;
    bool hasCheckpoint_;
};

} // namespace roguelike
