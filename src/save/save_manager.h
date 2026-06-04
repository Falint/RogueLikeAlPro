/**
 * @file save_manager.h
 * @brief Modul file handling profesional untuk save/load game.
 *
 * Menggunakan fstream.
 * Menangani: file tidak ditemukan, file kosong, file corrupt.
 * Tidak menyebabkan crash dalam kondisi apapun.
 */

#pragma once

#include "serializer.h"

#include <string>
#include <optional>

namespace roguelike {

/**
 * @brief Mengelola save dan load game dari file.
 */
class SaveManager {
public:
    /**
     * @brief Menyimpan game state ke file.
     * @param data Data yang akan disimpan.
     * @param filepath Path file tujuan.
     * @return true jika berhasil, false jika gagal.
     */
    static bool saveGame(const GameSaveData& data, const std::string& filepath);

    /**
     * @brief Memuat game state dari file.
     * @param filepath Path file sumber.
     * @return GameSaveData jika berhasil, nullopt jika gagal.
     */
    static std::optional<GameSaveData> loadGame(const std::string& filepath);

    /**
     * @brief Mengecek apakah file save exists.
     */
    static bool saveFileExists(const std::string& filepath);

    /**
     * @brief Menghapus file save.
     * @return true jika berhasil, false jika gagal.
     */
    static bool deleteSaveFile(const std::string& filepath);

private:
    /**
     * @brief Membuat direktori save jika belum ada.
     */
    static void ensureSaveDirectory(const std::string& filepath);
};

} // namespace roguelike
