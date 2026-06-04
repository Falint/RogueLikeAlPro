/**
 * @file main.cpp
 * @brief Entry point game Roguelike Dungeon Crawler.
 *
 * Membuat instance Game dan menjalankan game loop.
 * Seluruh logika ada di dalam class Game.
 */

#include "core/game.h"

int main() {
    roguelike::Game game;
    game.run();
    return 0;
}
