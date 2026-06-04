/**
 * @file combat_system.h
 * @brief Sistem combat terpisah dari entity.
 *
 * Menangani damage calculation, attack resolution, death handling,
 * dan reward distribution.
 */

#pragma once

#include "../entity/player.h"
#include "../entity/enemy.h"

#include <string>
#include <vector>

namespace roguelike {

/**
 * @brief Hasil dari satu resolusi serangan.
 */
struct CombatResult {
    int damageDealt;
    bool targetDied;
    std::string message;
};

/**
 * @brief Sistem combat yang memproses pertarungan antara entity.
 */
class CombatSystem {
public:
    /**
     * @brief Menghitung damage dari attacker ke defender.
     * @return Damage yang dihasilkan (minimal MIN_DAMAGE).
     *
     * Rumus: max(MIN_DAMAGE, attackerAttack - defenderDefense)
     */
    static int calculateDamage(int attackerAttack, int defenderDefense);

    /**
     * @brief Memproses serangan pemain ke enemy.
     * @return Hasil pertarungan.
     */
    static CombatResult playerAttackEnemy(Player& player, Enemy& enemy);

    /**
     * @brief Memproses serangan enemy ke pemain.
     * @return Hasil pertarungan.
     */
    static CombatResult enemyAttackPlayer(Enemy& enemy, Player& player);

    /**
     * @brief Memberikan reward ke pemain setelah enemy mati.
     * @return Pesan reward yang diterima.
     */
    static std::string grantRewards(Player& player, const Enemy& enemy);
};

} // namespace roguelike
