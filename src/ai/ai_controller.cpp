/**
 * @file ai_controller.cpp
 * @brief Implementasi AI controller.
 *
 * Algoritma pergerakan: greedy approach — gerakkan satu langkah
 * ke arah yang paling mengurangi jarak Manhattan ke pemain,
 * tanpa menembus dinding atau entity lain.
 */

#include "ai_controller.h"
#include "../utility/constants.h"

#include <cmath>
#include <algorithm>

namespace roguelike {

void AIController::moveTowardsPlayer(Enemy& enemy,
                                      const Position& playerPos,
                                      const Map& map,
                                      const std::vector<std::unique_ptr<Enemy>>& allEnemies)
{
    if (!enemy.isAlive()) return;

    int detectionRange = (enemy.getType() == EntityType::BOSS)
                       ? constants::BOSS_DETECTION_RANGE
                       : constants::ENEMY_DETECTION_RANGE;

    int distance = manhattanDistance(enemy.getPosition(), playerPos);

    /* Jangan bergerak jika di luar jarak deteksi */
    if (distance > detectionRange) return;

    /* Jangan bergerak jika sudah di samping pemain (akan menyerang) */
    if (distance <= 1) return;

    const Position& currentPos = enemy.getPosition();

    /* Coba 4 arah, pilih yang paling mendekat ke pemain */
    struct MoveCandidate {
        Position pos;
        int dist;
    };

    MoveCandidate candidates[] = {
        {{currentPos.x,     currentPos.y - 1}, 0}, // UP
        {{currentPos.x,     currentPos.y + 1}, 0}, // DOWN
        {{currentPos.x - 1, currentPos.y},     0}, // LEFT
        {{currentPos.x + 1, currentPos.y},     0}  // RIGHT
    };

    /* Hitung jarak ke pemain untuk setiap kandidat */
    for (auto& c : candidates) {
        c.dist = manhattanDistance(c.pos, playerPos);
    }

    /* Urutkan berdasarkan jarak terdekat */
    std::sort(std::begin(candidates), std::end(candidates),
              [](const MoveCandidate& a, const MoveCandidate& b) {
                  return a.dist < b.dist;
              });

    /* Coba bergerak ke kandidat terbaik yang valid */
    for (const auto& c : candidates) {
        bool walkable = map.isWalkableAt(c.pos);
        bool notPlayer = (c.pos != playerPos);
        bool notOccupied = !isOccupiedByEnemy(c.pos, enemy, allEnemies);

        if (walkable && notPlayer && notOccupied) {
            enemy.setPosition(c.pos);
            return;
        }
    }
}

bool AIController::isAdjacentToPlayer(const Enemy& enemy, const Position& playerPos) {
    return manhattanDistance(enemy.getPosition(), playerPos) == 1;
}

int AIController::manhattanDistance(const Position& a, const Position& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

bool AIController::isOccupiedByEnemy(const Position& pos,
                                      const Enemy& excludeEnemy,
                                      const std::vector<std::unique_ptr<Enemy>>& allEnemies)
{
    for (const auto& e : allEnemies) {
        if (e.get() != &excludeEnemy && e->isAlive() && e->getPosition() == pos) {
            return true;
        }
    }
    return false;
}

} // namespace roguelike
