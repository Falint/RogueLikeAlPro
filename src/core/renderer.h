/**
 * @file renderer.h
 * @brief Sistem rendering ASCII ke terminal.
 *
 * Menggambar map, entity, loot, dan HUD menggunakan ANSI escape codes.
 * Mendukung clear screen dan warna terminal.
 */

#pragma once

#include "../world/map.h"
#include "../entity/player.h"
#include "../entity/enemy.h"

#include <vector>
#include <memory>
#include <string>

namespace roguelike {

/**
 * @brief Posisi dan karakter loot yang ada di map.
 */
struct LootOnMap {
    Position pos;
    char displayChar;
};

/**
 * @brief Renderer untuk menampilkan game state ke terminal.
 */
class Renderer {
public:
    /**
     * @brief Membersihkan layar terminal.
     */
    static void clearScreen();

    /**
     * @brief Menampilkan seluruh frame: map + entity + HUD.
     */
    static void render(const Map& map,
                       const Player& player,
                       const std::vector<std::unique_ptr<Enemy>>& enemies,
                       const std::vector<LootOnMap>& lootItems,
                       int currentFloor,
                       const std::vector<std::string>& messages);

    /**
     * @brief Menampilkan menu inventory.
     */
    static void renderInventoryMenu(const Player& player);

    /**
     * @brief Menampilkan menu senjata (equip/swap).
     */
    static void renderWeaponMenu(const Player& player);

    /**
     * @brief Menampilkan menu upgrade senjata.
     */
    static void renderUpgradeMenu(const Player& player);

    /**
     * @brief Menampilkan layar game over.
     */
    static void renderGameOver(const Player& player);

    /**
     * @brief Menampilkan layar kemenangan.
     */
    static void renderVictory(const Player& player);

    /**
     * @brief Menampilkan menu utama.
     */
    static void renderMainMenu(bool hasSaveFile);

private:
    /**
     * @brief Menampilkan HUD (stats bar) di bawah map.
     */
    static void renderHUD(const Player& player, int currentFloor);

    /**
     * @brief Menampilkan log pesan terbaru.
     */
    static void renderMessages(const std::vector<std::string>& messages);

    /**
     * @brief Menampilkan garis horizontal pemisah.
     */
    static void renderSeparator(int width);
};

} // namespace roguelike
