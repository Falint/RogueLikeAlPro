/**
 * @file game.h
 * @brief Kelas Game — orchestrator utama game loop.
 *
 * Mengkoordinasikan seluruh subsistem:
 * Input -> Update (move, AI, combat, loot, checkpoint) -> Render.
 */

#pragma once

#include "input_handler.h"
#include "renderer.h"
#include "../world/map.h"
#include "../entity/player.h"
#include "../entity/enemy.h"
#include "../entity/boss.h"
#include "../loot/loot_table.h"
#include "../checkpoint/checkpoint_system.h"
#include "../utility/types.h"

#include <vector>
#include <memory>
#include <string>

namespace roguelike {

class Game {
public:
    Game();
    void run();

private:
    void showMainMenu();
    void startNewGame();
    bool loadGameFromFile();
    void initializeFloor(int floorIndex, bool goingUp = false);
    void placePlayer(bool goingUp = false);
    void spawnEnemies(int floorIndex);
    void spawnBoss(int floorIndex);
    void placeLootOnMap();

    void gameLoop();
    void processInput(InputAction action);
    void movePlayer(Direction dir);
    void updateEnemyAI();
    void resolveCombat();
    void checkLootPickup();
    void checkCheckpoint();
    void checkStairs();

    void handleInventoryMenu();
    void handleWeaponMenu();
    void handleUpgradeMenu();
    void handleManualSave();

    void handleGameOver();
    void handleVictory();
    void respawnAtCheckpoint();

    void addMessage(const std::string& msg);
    void processLootDrop(const Enemy& enemy);

    GameState gameState_;
    int currentFloor_;
    Player player_;
    Map currentMap_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<LootOnMap> lootOnMap_;
    std::vector<std::string> messages_;
    CheckpointSystem checkpointSystem_;

    struct WeaponDrop {
        Position pos;
        Weapon weapon;
    };
    std::vector<WeaponDrop> weaponDropsOnMap_;
    bool bossDefeated_;
};

} // namespace roguelike
