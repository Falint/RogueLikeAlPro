/**
 * @file game.cpp
 * @brief Implementasi Game — orchestrator game loop.
 */

#include "game.h"
#include "../world/map_generator.h"
#include "../combat/combat_system.h"
#include "../ai/ai_controller.h"
#include "../weapon/weapon_upgrade.h"
#include "../save/save_manager.h"
#include "../utility/constants.h"
#include "../utility/random_manager.h"

#include <iostream>
#include <algorithm>

namespace roguelike {

Game::Game()
    : gameState_(GameState::PLAYING)
    , currentFloor_(0)
    , bossDefeated_(false)
{
}

void Game::run() {
    /* Enter Alternate Screen Buffer agar frame tidak menumpuk di scrollback terminal */
    std::cout << "\033[?1049h";
    
    showMainMenu();
    if (gameState_ != GameState::QUIT) {
        gameLoop();
    }
    
    /* Leave Alternate Screen Buffer saat keluar game */
    std::cout << "\033[?1049l";
}

void Game::showMainMenu() {
    /*
     * Menu utama: [1] New Game, [2] Load Game, [Q] Quit.
     * Menggunakan number keys yang sudah di-mapping oleh InputHandler.
     */
    while (true) {
        bool hasSave = SaveManager::saveFileExists(constants::SAVE_FILENAME);
        Renderer::renderMainMenu(hasSave);

        InputAction action = InputHandler::getInput();

        if (action == InputAction::QUIT) {
            gameState_ = GameState::QUIT;
            return;
        }

        /* [1] atau [Enter] atau [S] → New Game */
        if (action == InputAction::USE_ITEM_1 ||
            action == InputAction::CONFIRM) {
            startNewGame();
            return;
        }

        /* [2] → Load Game (hanya jika save file ada) */
        if (action == InputAction::USE_ITEM_2 && hasSave) {
            if (loadGameFromFile()) {
                return;
            }
        }
    }
}

void Game::startNewGame() {
    gameState_ = GameState::PLAYING;
    currentFloor_ = 0;
    player_ = Player("Hero", {0, 0});
    bossDefeated_ = false;
    messages_.clear();

    initializeFloor(currentFloor_);
    addMessage("Welcome to the dungeon! Use WASD to move.");
}

bool Game::loadGameFromFile() {
    auto saveData = SaveManager::loadGame(constants::SAVE_FILENAME);
    if (!saveData.has_value()) {
        addMessage("Failed to load save file!");
        return false;
    }

    const auto& data = saveData.value();

    /* Restore player state */
    player_ = Player(data.playerName, {data.posX, data.posY});
    player_.setHp(data.hp);
    player_.setMaxHp(data.maxHp);
    player_.setAttack(data.attack);
    player_.setDefense(data.defense);
    player_.setGold(data.gold);
    player_.setExperience(data.experience);
    player_.setLevel(data.level);

    /* Restore weapon */
    if (!data.weaponData.empty()) {
        try {
            player_.equipWeapon(Weapon::deserialize(data.weaponData));
        } catch (...) { /* keep default weapon */ }
    }

    /* Restore stored weapons */
    for (const auto& wStr : data.storedWeaponsData) {
        try {
            player_.storeWeapon(Weapon::deserialize(wStr));
        } catch (...) { /* skip corrupt entry */ }
    }

    /* Restore inventory */
    for (const auto& iStr : data.inventoryData) {
        try {
            player_.getInventory().addItem(Item::fromString(iStr));
        } catch (...) { /* skip corrupt entry */ }
    }

    /* Restore checkpoint */
    if (data.checkpointX >= 0 && data.checkpointY >= 0) {
        checkpointSystem_.setCheckpoint({data.checkpointX, data.checkpointY},
                                         data.currentFloor);
    }

    currentFloor_ = data.currentFloor;
    gameState_ = GameState::PLAYING;
    bossDefeated_ = false;

    initializeFloor(currentFloor_);

    /* Place player at saved position if valid, otherwise use room start */
    if (currentMap_.isWalkableAt(data.posX, data.posY)) {
        player_.setPosition(data.posX, data.posY);
    }

    addMessage("Game loaded successfully!");
    return true;
}

void Game::initializeFloor(int floorIndex, bool goingUp) {
    currentMap_ = MapGenerator::generate(floorIndex);
    enemies_.clear();
    lootOnMap_.clear();
    weaponDropsOnMap_.clear();
    bossDefeated_ = false;

    placePlayer(goingUp);
    
    if ((floorIndex + 1) % constants::BOSS_FLOOR_INTERVAL == 0) {
        spawnBoss(floorIndex);
    } else {
        spawnEnemies(floorIndex);
    }

    placeLootOnMap();
}

void Game::placePlayer(bool goingUp) {
    if (goingUp) {
        // Pemain naik tangga dari bawah, maka muncul di tangga turun
        Position startPos = currentMap_.getStairsDownPosition();
        if (startPos.x != -1) {
            player_.setPosition(startPos);
            return;
        }
    } else {
        // Pemain turun tangga dari atas, maka muncul di tangga naik
        Position startPos = currentMap_.getStairsUpPosition();
        if (startPos.x != -1) {
            player_.setPosition(startPos);
            return;
        }
    }
    
    // Fallback: tengah ruangan pertama
    const auto& rooms = currentMap_.getRooms();
    if (!rooms.empty()) {
        Position startPos = rooms.front().center();
        player_.setPosition(startPos);
    }
}

void Game::spawnEnemies(int floorIndex) {
    int count = constants::ENEMIES_BASE_COUNT + (floorIndex * constants::ENEMIES_ADD_PER_FLOOR);

    const auto& rooms = currentMap_.getRooms();
    auto& rng = RandomManager::instance();

    for (int i = 0; i < count; ++i) {
        /* Place enemy in random room (not the first room where player spawns) */
        if (rooms.size() < 2) continue;

        size_t roomIdx = static_cast<size_t>(rng.getInt(1, static_cast<int>(rooms.size()) - 1));
        const Room& room = rooms[roomIdx];

        int ex = rng.getInt(room.x + 1, room.x + room.width - 2);
        int ey = rng.getInt(room.y + 1, room.y + room.height - 2);

        if (currentMap_.isWalkableAt(ex, ey)) {
            enemies_.push_back(std::make_unique<Enemy>(Position{ex, ey}, floorIndex));
        }
    }
}

void Game::spawnBoss(int floorIndex) {
    const auto& rooms = currentMap_.getRooms();
    if (rooms.size() < 2) return;

    /* Place boss in the last room — far from spawn */
    const Room& bossRoom = rooms.back();
    Position bossPos = bossRoom.center();

    /* Offset slightly so not on stairs */
    if (currentMap_.getTile(bossPos) == TileType::STAIRS_DOWN || currentMap_.getTile(bossPos) == TileType::STAIRS_UP) {
        bossPos.x += 1;
    }

    if (currentMap_.isWalkableAt(bossPos)) {
        enemies_.push_back(std::make_unique<Boss>(bossPos, floorIndex));
    }
}

void Game::placeLootOnMap() {
    auto& rng = RandomManager::instance();
    const auto& rooms = currentMap_.getRooms();

    /* Place some gold and potions on the map */
    constexpr int lootPerFloor = 4;
    for (int i = 0; i < lootPerFloor; ++i) {
        if (rooms.empty()) break;
        size_t roomIdx = static_cast<size_t>(rng.getInt(0, static_cast<int>(rooms.size()) - 1));
        const Room& room = rooms[roomIdx];

        int lx = rng.getInt(room.x + 1, room.x + room.width - 2);
        int ly = rng.getInt(room.y + 1, room.y + room.height - 2);
        Position pos{lx, ly};

        if (currentMap_.isWalkableAt(pos)) {
            if (rng.rollChance(60)) {
                lootOnMap_.push_back({pos, constants::TILE_GOLD});
            } else {
                lootOnMap_.push_back({pos, constants::TILE_POTION});
            }
        }
    }
}

void Game::gameLoop() {
    while (gameState_ == GameState::PLAYING) {
        Renderer::render(currentMap_, player_, enemies_, lootOnMap_,
                         currentFloor_, messages_);

        InputAction action = InputHandler::getInput();
        processInput(action);
    }

    if (gameState_ == GameState::GAME_OVER) {
        handleGameOver();
    } else if (gameState_ == GameState::VICTORY) {
        handleVictory();
    }
}

void Game::processInput(InputAction action) {
    Direction dir = InputHandler::actionToDirection(action);

    if (dir != Direction::NONE) {
        Position oldPos = player_.getPosition();
        movePlayer(dir);
        updateEnemyAI();
        resolveCombat();
        checkLootPickup();
        checkCheckpoint();
        
        // Hanya cek stairs jika pemain benar-benar berpindah tempat (baru menginjak)
        if (oldPos != player_.getPosition()) {
            checkStairs();
        }
        return;
    }

    switch (action) {
        case InputAction::OPEN_INVENTORY:
            handleInventoryMenu();
            break;
        case InputAction::OPEN_WEAPON_MENU:
            handleWeaponMenu();
            break;
        case InputAction::OPEN_UPGRADE_MENU:
            handleUpgradeMenu();
            break;
        case InputAction::SAVE_GAME:
            handleManualSave();
            break;
        case InputAction::QUIT:
            gameState_ = GameState::QUIT;
            break;
        default:
            break;
    }
}

void Game::movePlayer(Direction dir) {
    Position offset = directionToOffset(dir);
    Position newPos = {player_.getPosition().x + offset.x,
                       player_.getPosition().y + offset.y};

    if (!currentMap_.isWalkableAt(newPos)) return;

    /* Check if position has an enemy */
    for (auto& enemy : enemies_) {
        if (enemy->isAlive() && enemy->getPosition() == newPos) {
            /* Attack instead of move */
            auto result = CombatSystem::playerAttackEnemy(player_, *enemy);
            addMessage(result.message);

            if (result.targetDied) {
                std::string rewardMsg = CombatSystem::grantRewards(player_, *enemy);
                addMessage(rewardMsg);
                processLootDrop(*enemy);

                if (enemy->getType() == EntityType::BOSS) {
                    bossDefeated_ = true;
                    addMessage("*** BOSS DEFEATED! Proceed downstairs! ***");
                }
            }
            return;
        }
    }

    player_.setPosition(newPos);

    /* Pick up weapon drops */
    for (auto it = weaponDropsOnMap_.begin(); it != weaponDropsOnMap_.end(); ) {
        if (it->pos == newPos) {
            addMessage("Found weapon: " + it->weapon.getDisplayName());
            player_.storeWeapon(it->weapon);
            it = weaponDropsOnMap_.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::updateEnemyAI() {
    for (auto& enemy : enemies_) {
        AIController::moveTowardsPlayer(*enemy, player_.getPosition(),
                                         currentMap_, enemies_);
    }
}

void Game::resolveCombat() {
    for (auto& enemy : enemies_) {
        if (!enemy->isAlive()) continue;

        if (AIController::isAdjacentToPlayer(*enemy, player_.getPosition())) {
            auto result = CombatSystem::enemyAttackPlayer(*enemy, player_);
            addMessage(result.message);

            if (result.targetDied) {
                gameState_ = GameState::GAME_OVER;
                return;
            }
        }
    }
}

void Game::checkLootPickup() {
    const Position& ppos = player_.getPosition();

    for (auto it = lootOnMap_.begin(); it != lootOnMap_.end(); ) {
        if (it->pos == ppos) {
            auto& rng = RandomManager::instance();

            if (it->displayChar == constants::TILE_GOLD) {
                int amount = rng.getInt(constants::GOLD_DROP_MIN, constants::GOLD_DROP_MAX);
                player_.addGold(amount);
                addMessage("Picked up " + std::to_string(amount) + " gold!");
            } else if (it->displayChar == constants::TILE_POTION) {
                Item potion;
                potion.name = "Health Potion";
                potion.type = ItemType::POTION;
                potion.value = constants::POTION_HEAL_AMOUNT;

                if (player_.getInventory().addItem(potion)) {
                    addMessage("Picked up Health Potion!");
                } else {
                    addMessage("Inventory full! Cannot pick up potion.");
                    ++it;
                    continue;
                }
            }

            it = lootOnMap_.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::checkCheckpoint() {
    if (currentMap_.getTile(player_.getPosition()) == TileType::CHECKPOINT) {
        if (checkpointSystem_.checkAndSave(player_, currentFloor_)) {
            addMessage("*** Checkpoint reached! Progress saved. ***");
        }
    }
}

void Game::checkStairs() {
    if (currentMap_.getTile(player_.getPosition()) == TileType::STAIRS_DOWN) {
        ++currentFloor_;
        addMessage("Descending to floor " + std::to_string(currentFloor_ + 1) + "...");
        initializeFloor(currentFloor_, false);
    } else if (currentMap_.getTile(player_.getPosition()) == TileType::STAIRS_UP) {
        if (currentFloor_ > 0) {
            --currentFloor_;
            addMessage("Ascending to floor " + std::to_string(currentFloor_ + 1) + "...");
            initializeFloor(currentFloor_, true);
        } else {
            addMessage("You cannot go back any further. The dungeon entrance is sealed!");
        }
    }
}

void Game::handleInventoryMenu() {
    while (true) {
        Renderer::renderInventoryMenu(player_);
        InputAction action = InputHandler::getInput();

        if (action == InputAction::CANCEL || action == InputAction::OPEN_INVENTORY) {
            return;
        }

        /* Use item by number key */
        int itemIndex = -1;
        if (action >= InputAction::USE_ITEM_0 && action <= InputAction::USE_ITEM_9) {
            itemIndex = static_cast<int>(action) - static_cast<int>(InputAction::USE_ITEM_0);
        }

        if (itemIndex >= 0 && itemIndex < static_cast<int>(player_.getInventory().getItemCount())) {
            const Item& item = player_.getInventory().getItem(static_cast<size_t>(itemIndex));

            if (item.type == ItemType::POTION) {
                int healAmount = player_.getInventory().useItem(static_cast<size_t>(itemIndex));
                player_.heal(healAmount);
                addMessage("Used potion! Healed " + std::to_string(healAmount) + " HP.");
            } else if (item.type == ItemType::GOLD_BAG) {
                int goldAmount = player_.getInventory().useItem(static_cast<size_t>(itemIndex));
                player_.addGold(goldAmount);
                addMessage("Opened gold bag! Got " + std::to_string(goldAmount) + " gold.");
            }
        }
    }
}

void Game::handleWeaponMenu() {
    while (true) {
        Renderer::renderWeaponMenu(player_);
        InputAction action = InputHandler::getInput();

        if (action == InputAction::CANCEL || action == InputAction::OPEN_WEAPON_MENU) {
            return;
        }

        int wpnIndex = -1;
        if (action >= InputAction::USE_ITEM_0 && action <= InputAction::USE_ITEM_9) {
            wpnIndex = static_cast<int>(action) - static_cast<int>(InputAction::USE_ITEM_0);
        }

        auto& stored = player_.getStoredWeapons();
        if (wpnIndex >= 0 && wpnIndex < static_cast<int>(stored.size())) {
            /* Swap: current equipped goes to stored, selected goes to equipped */
            Weapon oldEquipped = player_.getEquippedWeapon();
            Weapon newEquipped = stored[static_cast<size_t>(wpnIndex)];

            stored.erase(stored.begin() + wpnIndex);
            player_.equipWeapon(newEquipped);

            /* Remove the duplicate that equipWeapon adds (since we manually manage) */
            /* equipWeapon stores old weapon, but we already removed from stored.
               The old weapon was added by equipWeapon, so we need to remove the last added */
            if (!stored.empty() && oldEquipped.isValid()) {
                /* equipWeapon already pushed old to stored, which is correct */
            }

            addMessage("Equipped " + newEquipped.getDisplayName());
        }
    }
}

void Game::handleUpgradeMenu() {
    while (true) {
        Renderer::renderUpgradeMenu(player_);
        InputAction action = InputHandler::getInput();

        if (action == InputAction::CANCEL) {
            return;
        }

        if (action == InputAction::OPEN_UPGRADE_MENU) {
            int gold = player_.getGold();
            if (WeaponUpgrade::tryUpgrade(player_.getEquippedWeapon(), gold)) {
                player_.setGold(gold);
                addMessage("Weapon upgraded! " + player_.getEquippedWeapon().getDisplayName());
            } else {
                addMessage("Cannot upgrade weapon.");
            }
        }
    }
}

void Game::handleManualSave() {
    checkpointSystem_.checkAndSave(player_, currentFloor_);
    addMessage("Game saved successfully!");
}

void Game::handleGameOver() {
    Renderer::renderGameOver(player_);

    while (true) {
        InputAction action = InputHandler::getInput();

        /* [1] atau [Enter] → Respawn at checkpoint */
        if (action == InputAction::CONFIRM || action == InputAction::USE_ITEM_1) {
            if (checkpointSystem_.hasCheckpoint()) {
                respawnAtCheckpoint();
                gameLoop();
                return;
            } else {
                startNewGame();
                gameLoop();
                return;
            }
        }

        if (action == InputAction::QUIT) {
            gameState_ = GameState::QUIT;
            return;
        }
    }
}

void Game::handleVictory() {
    Renderer::renderVictory(player_);

    while (true) {
        InputAction action = InputHandler::getInput();
        if (action == InputAction::QUIT) {
            gameState_ = GameState::QUIT;
            return;
        }
    }
}

void Game::respawnAtCheckpoint() {
    Position cp = checkpointSystem_.getLastCheckpoint();
    int floor = checkpointSystem_.getCheckpointFloor();

    /* Reload dari save file */
    if (loadGameFromFile()) {
        gameState_ = GameState::PLAYING;
        addMessage("Respawned at last checkpoint.");
    } else {
        /* Fallback: reset player HP and place at checkpoint */
        currentFloor_ = floor;
        initializeFloor(currentFloor_);
        player_.setPosition(cp);
        player_.setHp(player_.getMaxHp());
        gameState_ = GameState::PLAYING;
        addMessage("Respawned at checkpoint (HP restored).");
    }
}

void Game::addMessage(const std::string& msg) {
    messages_.push_back(msg);

    /* Batasi jumlah pesan agar tidak terlalu banyak */
    constexpr size_t maxStoredMessages = 50;
    if (messages_.size() > maxStoredMessages) {
        messages_.erase(messages_.begin());
    }
}

void Game::processLootDrop(const Enemy& enemy) {
    LootDrop drop;

    if (enemy.getType() == EntityType::BOSS) {
        drop = LootTable::generateBossLoot();
    } else {
        drop = LootTable::generateEnemyLoot();
    }

    /* Add bonus gold */
    if (drop.bonusGold > 0) {
        player_.addGold(drop.bonusGold);
        addMessage("Loot: +" + std::to_string(drop.bonusGold) + " gold");
    }

    /* Add items to inventory */
    for (const auto& item : drop.items) {
        if (player_.getInventory().addItem(item)) {
            addMessage("Loot: " + item.name);
        } else {
            addMessage("Inventory full! " + item.name + " dropped on ground.");
        }
    }

    /* Place weapons on map at enemy position */
    for (const auto& weapon : drop.weapons) {
        weaponDropsOnMap_.push_back({enemy.getPosition(), weapon});
        lootOnMap_.push_back({enemy.getPosition(), constants::TILE_WEAPON});
        addMessage("Weapon dropped: " + weapon.getDisplayName());
    }
}

} // namespace roguelike
