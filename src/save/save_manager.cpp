/**
 * @file save_manager.cpp
 * @brief Implementasi SaveManager — file I/O dengan fstream.
 *
 * Validasi: file exists → not empty → header valid → parse data.
 * Semua operasi dibungkus try-catch untuk mencegah crash.
 */

#include "save_manager.h"
#include "../utility/constants.h"

#include <fstream>
#include <sstream>
#include <filesystem>

namespace roguelike {

bool SaveManager::saveGame(const GameSaveData& data, const std::string& filepath) {
    try {
        ensureSaveDirectory(filepath);

        std::ofstream file(filepath, std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            return false;
        }

        std::string content = Serializer::serialize(data);
        file << content;
        file.close();

        return file.good() || !file.fail();
    } catch (...) {
        return false;
    }
}

std::optional<GameSaveData> SaveManager::loadGame(const std::string& filepath) {
    try {
        /* Cek file exists */
        if (!saveFileExists(filepath)) {
            return std::nullopt;
        }

        std::ifstream file(filepath, std::ios::in);
        if (!file.is_open()) {
            return std::nullopt;
        }

        /* Baca seluruh isi file */
        std::ostringstream contentStream;
        contentStream << file.rdbuf();
        file.close();

        std::string content = contentStream.str();

        /* Cek file kosong */
        if (content.empty()) {
            return std::nullopt;
        }

        /* Deserialize */
        return Serializer::deserialize(content);

    } catch (...) {
        return std::nullopt;
    }
}

bool SaveManager::saveFileExists(const std::string& filepath) {
    try {
        return std::filesystem::exists(filepath);
    } catch (...) {
        return false;
    }
}

bool SaveManager::deleteSaveFile(const std::string& filepath) {
    try {
        if (saveFileExists(filepath)) {
            return std::filesystem::remove(filepath);
        }
        return false;
    } catch (...) {
        return false;
    }
}

void SaveManager::ensureSaveDirectory(const std::string& filepath) {
    try {
        std::filesystem::path path(filepath);
        std::filesystem::path dir = path.parent_path();
        if (!dir.empty() && !std::filesystem::exists(dir)) {
            std::filesystem::create_directories(dir);
        }
    } catch (...) {
        /* Jika gagal membuat direktori, saveGame akan gagal saat buka file */
    }
}

} // namespace roguelike
