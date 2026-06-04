/**
 * @file renderer.cpp
 * @brief Implementasi Renderer — output ASCII ke terminal.
 *
 * Menggunakan ANSI escape codes untuk clear screen dan pewarnaan.
 * Rendering order: clear → map → entities overlay → HUD → messages.
 */

#include "renderer.h"
#include "../utility/constants.h"

#include <iostream>
#include <iomanip>
#include <sstream>

namespace roguelike {

void Renderer::clearScreen() {
    /* ANSI escape code: clear screen + move cursor home */
    std::cout << "\033[2J\033[H";
}

void Renderer::render(const Map& map,
                      const Player& player,
                      const std::vector<std::unique_ptr<Enemy>>& enemies,
                      const std::vector<LootOnMap>& lootItems,
                      int currentFloor,
                      const std::vector<std::string>& messages)
{
    clearScreen();

    /* Bangun buffer 2D dari map tiles */
    int width = map.getWidth();
    int height = map.getHeight();

    std::vector<std::vector<char>> buffer(
        static_cast<size_t>(height),
        std::vector<char>(static_cast<size_t>(width))
    );

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            buffer[static_cast<size_t>(y)][static_cast<size_t>(x)] = map.getRenderChar(x, y);
        }
    }

    /* Overlay loot items */
    for (const auto& loot : lootItems) {
        if (map.isInBounds(loot.pos)) {
            buffer[static_cast<size_t>(loot.pos.y)][static_cast<size_t>(loot.pos.x)] = loot.displayChar;
        }
    }

    /* Overlay enemies */
    for (const auto& enemy : enemies) {
        if (enemy->isAlive()) {
            const Position& pos = enemy->getPosition();
            if (map.isInBounds(pos)) {
                buffer[static_cast<size_t>(pos.y)][static_cast<size_t>(pos.x)] = enemy->getRenderChar();
            }
        }
    }

    /* Overlay player */
    {
        const Position& pos = player.getPosition();
        if (map.isInBounds(pos)) {
            buffer[static_cast<size_t>(pos.y)][static_cast<size_t>(pos.x)] = constants::TILE_PLAYER;
        }
    }

    /* Tampilkan floor header */
    std::string floorNames[] = {"Floor 1 - Easy", "Floor 2 - Medium", "Floor 3 - Boss"};
    std::string floorLabel = "Unknown Floor";
    if (currentFloor >= 0 && currentFloor < constants::TOTAL_FLOORS) {
        floorLabel = floorNames[static_cast<size_t>(currentFloor)];
    }
    std::cout << "=== " << floorLabel << " ===\n";

    /* Render buffer ke terminal */
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            char ch = buffer[static_cast<size_t>(y)][static_cast<size_t>(x)];

            /* Warna ANSI berdasarkan karakter */
            switch (ch) {
                case constants::TILE_PLAYER:
                    std::cout << "\033[1;32m" << ch << "\033[0m"; // hijau terang
                    break;
                case constants::TILE_ENEMY:
                    std::cout << "\033[1;31m" << ch << "\033[0m"; // merah terang
                    break;
                case constants::TILE_BOSS:
                    std::cout << "\033[1;35m" << ch << "\033[0m"; // magenta terang
                    break;
                case constants::TILE_GOLD:
                    std::cout << "\033[1;33m" << ch << "\033[0m"; // kuning terang
                    break;
                case constants::TILE_WEAPON:
                    std::cout << "\033[1;36m" << ch << "\033[0m"; // cyan terang
                    break;
                case constants::TILE_CHECKPOINT:
                    std::cout << "\033[1;34m" << ch << "\033[0m"; // biru terang
                    break;
                case constants::TILE_STAIRS:
                    std::cout << "\033[1;33m" << ch << "\033[0m"; // kuning
                    break;
                case constants::TILE_POTION:
                    std::cout << "\033[1;32m" << ch << "\033[0m"; // hijau
                    break;
                case constants::TILE_WALL:
                    std::cout << "\033[0;37m" << ch << "\033[0m"; // abu-abu
                    break;
                default:
                    std::cout << "\033[0;90m" << ch << "\033[0m"; // dark gray untuk floor
                    break;
            }
        }
        std::cout << '\n';
    }

    renderHUD(player, currentFloor);
    renderMessages(messages);

    /* Tampilkan kontrol */
    std::cout << "\033[0;90m[WASD]Move [I]Inventory [E]Weapons [U]Upgrade [P]Save [Q]Quit\033[0m\n";
}

void Renderer::renderHUD(const Player& player, int /*currentFloor*/) {
    renderSeparator(constants::HUD_WIDTH);

    std::cout << " HP: " << player.getHp() << "/" << player.getMaxHp()
              << "  |  ATK: " << player.getTotalAttack()
              << "  |  DEF: " << player.getDefense()
              << "  |  LVL: " << player.getLevel() << "\n";

    std::cout << " Gold: " << player.getGold()
              << "  |  EXP: " << player.getExperience()
              << "/" << player.getExpToNextLevel()
              << "  |  Weapon: " << player.getEquippedWeapon().getDisplayName() << "\n";

    renderSeparator(constants::HUD_WIDTH);
}

void Renderer::renderMessages(const std::vector<std::string>& messages) {
    if (messages.empty()) return;

    /* Tampilkan maksimal 5 pesan terbaru */
    constexpr size_t maxMessages = 5;
    size_t start = (messages.size() > maxMessages) ? messages.size() - maxMessages : 0;

    for (size_t i = start; i < messages.size(); ++i) {
        std::cout << " > " << messages[i] << "\n";
    }
}

void Renderer::renderSeparator(int width) {
    for (int i = 0; i < width; ++i) {
        std::cout << '-';
    }
    std::cout << '\n';
}

void Renderer::renderInventoryMenu(const Player& player) {
    clearScreen();
    std::cout << "=== INVENTORY ===\n\n";
    std::cout << player.getInventory().getDisplayString() << "\n";
    std::cout << "Press [0-9] to use item, [ESC] to close\n";
}

void Renderer::renderWeaponMenu(const Player& player) {
    clearScreen();
    std::cout << "=== WEAPONS ===\n\n";
    std::cout << "  Equipped: " << player.getEquippedWeapon().getDisplayName() << "\n\n";

    const auto& stored = player.getStoredWeapons();
    if (stored.empty()) {
        std::cout << "  No stored weapons.\n";
    } else {
        std::cout << "  Stored weapons:\n";
        for (size_t i = 0; i < stored.size(); ++i) {
            std::cout << "  [" << i << "] " << stored[i].getDisplayName() << "\n";
        }
    }
    std::cout << "\nPress [0-9] to equip weapon, [ESC] to close\n";
}

void Renderer::renderUpgradeMenu(const Player& player) {
    clearScreen();
    std::cout << "=== WEAPON UPGRADE ===\n\n";

    /* Import WeaponUpgrade info — langsung akses via weapon data */
    const Weapon& wpn = player.getEquippedWeapon();
    std::cout << "  Weapon: " << wpn.getDisplayName() << "\n";
    std::cout << "  Current Damage Bonus: +" << wpn.getTotalDamageBonus() << "\n";

    if (wpn.getUpgradeLevel() >= constants::WEAPON_MAX_UPGRADE_LEVEL) {
        std::cout << "  \033[1;33mMAX LEVEL REACHED!\033[0m\n";
    } else {
        int cost = constants::WEAPON_UPGRADE_BASE_COST
                 + wpn.getUpgradeLevel() * constants::WEAPON_UPGRADE_COST_FACTOR;
        std::cout << "  Upgrade Cost: " << cost << " gold\n";
        std::cout << "  Your Gold: " << player.getGold() << "\n";
        std::cout << "  Damage after upgrade: +"
                  << (wpn.getTotalDamageBonus() + constants::WEAPON_UPGRADE_DAMAGE_GAIN) << "\n";

        if (player.getGold() >= cost) {
            std::cout << "\n  Press [U] to upgrade, [ESC] to close\n";
        } else {
            std::cout << "\n  \033[1;31mNot enough gold!\033[0m [ESC] to close\n";
        }
    }
}

void Renderer::renderGameOver(const Player& player) {
    clearScreen();
    std::cout << "\n\n";
    std::cout << "  ╔═══════════════════════════════════╗\n";
    std::cout << "  ║          GAME OVER                ║\n";
    std::cout << "  ╠═══════════════════════════════════╣\n";
    std::cout << "  ║  You have been defeated...        ║\n";
    std::cout << "  ║                                   ║\n";
    std::cout << "  ║  Level: " << std::setw(3) << player.getLevel()
              << "                        ║\n";
    std::cout << "  ║  Gold:  " << std::setw(5) << player.getGold()
              << "                      ║\n";
    std::cout << "  ╠═══════════════════════════════════╣\n";
    std::cout << "  ║  [1] Respawn at checkpoint        ║\n";
    std::cout << "  ║  [Q] Quit game                    ║\n";
    std::cout << "  ╚═══════════════════════════════════╝\n";
}

void Renderer::renderVictory(const Player& player) {
    clearScreen();
    std::cout << "\n\n";
    std::cout << "  ╔═══════════════════════════════════╗\n";
    std::cout << "  ║        ★ VICTORY! ★              ║\n";
    std::cout << "  ╠═══════════════════════════════════╣\n";
    std::cout << "  ║  You have conquered the dungeon!  ║\n";
    std::cout << "  ║                                   ║\n";
    std::cout << "  ║  Final Level: " << std::setw(3) << player.getLevel()
              << "                  ║\n";
    std::cout << "  ║  Gold:        " << std::setw(5) << player.getGold()
              << "                ║\n";
    std::cout << "  ║  Weapon: " << std::setw(20) << std::left
              << player.getEquippedWeapon().getName() << std::right << "   ║\n";
    std::cout << "  ╠═══════════════════════════════════╣\n";
    std::cout << "  ║  [Q] Quit game                    ║\n";
    std::cout << "  ╚═══════════════════════════════════╝\n";
}

void Renderer::renderMainMenu(bool hasSaveFile) {
    clearScreen();
    std::cout << "\n";
    std::cout << "  ╔═══════════════════════════════════╗\n";
    std::cout << "  ║     ROGUELIKE DUNGEON CRAWLER     ║\n";
    std::cout << "  ║         Terminal Edition          ║\n";
    std::cout << "  ╠═══════════════════════════════════╣\n";
    std::cout << "  ║                                   ║\n";
    std::cout << "  ║  [1] New Game                     ║\n";

    if (hasSaveFile) {
        std::cout << "  ║  [2] Load Game                    ║\n";
    }

    std::cout << "  ║  [Q] Quit                         ║\n";
    std::cout << "  ║                                   ║\n";
    std::cout << "  ╚═══════════════════════════════════╝\n";
}

} // namespace roguelike
