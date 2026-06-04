/**
 * @file constants.h
 * @brief Pusat seluruh konstanta game — menghilangkan magic number dari codebase.
 *
 * Setiap nilai numerik yang digunakan di lebih dari satu tempat, atau yang
 * merepresentasikan parameter desain game, WAJIB dideklarasikan di sini.
 */

#pragma once

#include <cstdint>

namespace roguelike {
namespace constants {

// ─── Map Dimensions ──────────────────────────────────────────────────────────
constexpr int MAP_WIDTH  = 120;
constexpr int MAP_HEIGHT = 60;

// ─── Floor / Dungeon ─────────────────────────────────────────────────────────
// Mode Endless: Konstanta lantai spesifik dihapus karena infinite
constexpr int BOSS_FLOOR_INTERVAL = 3;

// ─── Room Generation ─────────────────────────────────────────────────────────
constexpr int MIN_ROOM_SIZE      = 4;
constexpr int MAX_ROOM_SIZE      = 10;
constexpr int MAX_ROOMS_PER_MAP  = 25;

// ─── Tile Characters ─────────────────────────────────────────────────────────
constexpr char TILE_WALL       = '#';
constexpr char TILE_FLOOR      = '.';
constexpr char TILE_PLAYER     = 'P';
constexpr char TILE_ENEMY      = 'E';
constexpr char TILE_BOSS       = 'B';
constexpr char TILE_GOLD       = 'G';
constexpr char TILE_WEAPON     = 'W';
constexpr char TILE_CHECKPOINT = 'C';
constexpr char TILE_STAIRS_DOWN = '>';
constexpr char TILE_STAIRS_UP   = '<';
constexpr char TILE_POTION     = '+';

// ─── Player Defaults ─────────────────────────────────────────────────────────
constexpr int PLAYER_BASE_HP      = 100;
constexpr int PLAYER_BASE_ATTACK  = 10;
constexpr int PLAYER_BASE_DEFENSE = 5;
constexpr int PLAYER_START_GOLD   = 0;
constexpr int PLAYER_START_EXP    = 0;
constexpr int PLAYER_START_LEVEL  = 1;

// ─── Player Leveling ─────────────────────────────────────────────────────────
constexpr int EXP_PER_LEVEL        = 100;  // EXP dibutuhkan = level * EXP_PER_LEVEL
constexpr int HP_GAIN_PER_LEVEL    = 20;
constexpr int ATTACK_GAIN_PER_LEVEL  = 3;
constexpr int DEFENSE_GAIN_PER_LEVEL = 2;

// ─── Enemy Base Stats (Floor Easy) ──────────────────────────────────────────
constexpr int ENEMY_BASE_HP      = 30;
constexpr int ENEMY_BASE_ATTACK  = 6;
constexpr int ENEMY_BASE_DEFENSE = 2;
constexpr int ENEMY_BASE_EXP     = 25;
constexpr int ENEMY_BASE_GOLD    = 10;

// ─── Enemy Scaling per Floor (Linear) ────────────────────────────────────────
constexpr int ENEMY_HP_ADD_PER_FLOOR      = 15;
constexpr int ENEMY_ATTACK_ADD_PER_FLOOR  = 3;
constexpr int ENEMY_DEFENSE_ADD_PER_FLOOR = 1;
constexpr int ENEMY_EXP_ADD_PER_FLOOR     = 10;
constexpr int ENEMY_GOLD_ADD_PER_FLOOR    = 5;

// ─── Boss Stats (Solo & Sangat Sulit) ────────────────────────────────────────
constexpr double BOSS_HP_MULTIPLIER      = 15.0;
constexpr double BOSS_ATTACK_MULTIPLIER  = 4.0;
constexpr double BOSS_DEFENSE_MULTIPLIER = 3.0;
constexpr double BOSS_EXP_MULTIPLIER     = 10.0;
constexpr double BOSS_GOLD_MULTIPLIER    = 20.0;

// ─── Enemy Count per Floor ───────────────────────────────────────────────────
constexpr int ENEMIES_BASE_COUNT = 8;
constexpr int ENEMIES_ADD_PER_FLOOR = 2;

// ─── AI ──────────────────────────────────────────────────────────────────────
constexpr int ENEMY_DETECTION_RANGE = 8;
constexpr int BOSS_DETECTION_RANGE  = 20; // boss selalu agresif, jarak besar

// ─── Combat ──────────────────────────────────────────────────────────────────
constexpr int MIN_DAMAGE = 1;  // damage tidak pernah di bawah ini

// ─── Weapon Upgrade ──────────────────────────────────────────────────────────
constexpr int WEAPON_UPGRADE_BASE_COST   = 50;
constexpr int WEAPON_UPGRADE_COST_FACTOR = 50; // cost = BASE + level * FACTOR
constexpr int WEAPON_UPGRADE_DAMAGE_GAIN = 3;
constexpr int WEAPON_MAX_UPGRADE_LEVEL   = 10;

// ─── Weapon Rarity Damage Bonus ──────────────────────────────────────────────
constexpr int RARITY_COMMON_BONUS    = 0;
constexpr int RARITY_RARE_BONUS      = 5;
constexpr int RARITY_EPIC_BONUS      = 12;
constexpr int RARITY_LEGENDARY_BONUS = 25;

// ─── Inventory ───────────────────────────────────────────────────────────────
constexpr int INVENTORY_MAX_CAPACITY = 20;

// ─── Potion ──────────────────────────────────────────────────────────────────
constexpr int POTION_HEAL_AMOUNT = 30;

// ─── Gold Pickup Range ───────────────────────────────────────────────────────
constexpr int GOLD_DROP_MIN = 5;
constexpr int GOLD_DROP_MAX = 20;

// ─── Save System ─────────────────────────────────────────────────────────────
constexpr int SAVE_VERSION = 1;
constexpr const char* SAVE_DIRECTORY   = "save";
constexpr const char* SAVE_FILENAME    = "save/savegame.dat";
constexpr const char* SAVE_HEADER_TAG  = "[HEADER]";

// ─── Loot Drop Rates (Enemy Biasa) — dalam persen 0–100 ─────────────────────
constexpr int LOOT_ENEMY_GOLD_CHANCE     = 80;
constexpr int LOOT_ENEMY_POTION_CHANCE   = 30;
constexpr int LOOT_ENEMY_COMMON_WPN_CHANCE = 15;
constexpr int LOOT_ENEMY_RARE_WPN_CHANCE   = 5;

// ─── Loot Drop Rates (Boss) — dalam persen 0–100 ────────────────────────────
constexpr int LOOT_BOSS_GOLD_CHANCE           = 100;
constexpr int LOOT_BOSS_EPIC_WPN_CHANCE       = 0;
constexpr int LOOT_BOSS_LEGENDARY_WPN_CHANCE  = 100; // Selalu dapat legendary
constexpr int LOOT_BOSS_POTION_CHANCE         = 100;

// ─── Boss Gold Drop ──────────────────────────────────────────────────────────
constexpr int BOSS_GOLD_DROP_MIN = 300;
constexpr int BOSS_GOLD_DROP_MAX = 800;

// ─── Display ─────────────────────────────────────────────────────────────────
constexpr int VIEWPORT_WIDTH  = 100;
constexpr int VIEWPORT_HEIGHT = 30;
constexpr int HUD_WIDTH = 50;

} // namespace constants
} // namespace roguelike
