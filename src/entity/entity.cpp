/**
 * @file entity.cpp
 * @brief Implementasi base class Entity.
 */

#include "entity.h"
#include "../utility/constants.h"

#include <algorithm>

namespace roguelike {

Entity::Entity(const std::string& name, int hp, int attack, int defense,
               Position pos, EntityType type)
    : name_(name)
    , hp_(hp)
    , maxHp_(hp)
    , attack_(attack)
    , defense_(defense)
    , position_(pos)
    , type_(type)
{
}

void Entity::takeDamage(int amount) {
    hp_ = std::max(0, hp_ - amount);
}

void Entity::heal(int amount) {
    hp_ = std::min(maxHp_, hp_ + amount);
}

bool Entity::isAlive() const {
    return hp_ > 0;
}

char Entity::getRenderChar() const {
    switch (type_) {
        case EntityType::PLAYER: return constants::TILE_PLAYER;
        case EntityType::ENEMY:  return constants::TILE_ENEMY;
        case EntityType::BOSS:   return constants::TILE_BOSS;
    }
    return '?';
}

const std::string& Entity::getName() const { return name_; }
int Entity::getHp() const { return hp_; }
int Entity::getMaxHp() const { return maxHp_; }
int Entity::getAttack() const { return attack_; }
int Entity::getDefense() const { return defense_; }
const Position& Entity::getPosition() const { return position_; }
EntityType Entity::getType() const { return type_; }

void Entity::setPosition(const Position& pos) { position_ = pos; }
void Entity::setPosition(int x, int y) { position_ = {x, y}; }
void Entity::setHp(int hp) { hp_ = std::min(hp, maxHp_); }
void Entity::setMaxHp(int maxHp) { maxHp_ = maxHp; }
void Entity::setAttack(int attack) { attack_ = attack; }
void Entity::setDefense(int defense) { defense_ = defense; }

} // namespace roguelike
