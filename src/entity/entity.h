/**
 * @file entity.h
 * @brief Base class untuk semua entity dalam game (Player, Enemy, Boss).
 *
 * Entity memiliki HP, attack, defense, posisi, dan flag hidup/mati.
 * Menggunakan virtual methods untuk polymorphism.
 */

#pragma once

#include "../utility/types.h"

#include <string>

namespace roguelike {

/**
 * @brief Tipe entity untuk identifikasi dan rendering.
 */
enum class EntityType {
    PLAYER,
    ENEMY,
    BOSS
};

/**
 * @brief Base class untuk seluruh entity dalam game.
 */
class Entity {
public:
    Entity(const std::string& name, int hp, int attack, int defense, Position pos, EntityType type);
    virtual ~Entity() = default;

    /** @brief Menerima damage. HP tidak akan kurang dari 0. */
    void takeDamage(int amount);

    /** @brief Menyembuhkan HP. HP tidak melebihi maxHP. */
    void heal(int amount);

    /** @brief Mengecek apakah entity masih hidup. */
    bool isAlive() const;

    /** @brief Mendapatkan karakter render untuk entity ini. */
    char getRenderChar() const;

    // ─── Getters ─────────────────────────────────────────────────────────
    const std::string& getName() const;
    int getHp() const;
    int getMaxHp() const;
    int getAttack() const;
    int getDefense() const;
    const Position& getPosition() const;
    EntityType getType() const;

    // ─── Setters ─────────────────────────────────────────────────────────
    void setPosition(const Position& pos);
    void setPosition(int x, int y);
    void setHp(int hp);
    void setMaxHp(int maxHp);
    void setAttack(int attack);
    void setDefense(int defense);

protected:
    std::string name_;
    int hp_;
    int maxHp_;
    int attack_;
    int defense_;
    Position position_;
    EntityType type_;
};

} // namespace roguelike
