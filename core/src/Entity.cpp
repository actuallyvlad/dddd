#include <stdio.h>
#include "main.hpp"

Entity::Entity(unsigned int x, unsigned int y, unsigned int ch, 
    const TCODColor &color, std::string name, bool blocks, bool canDie,
    unsigned int fovRadius, bool canExplore, double maxHp, 
    double hp, double mp, double atk, double defense, unsigned int spd) :
    
    Object(x, y, ch, color, name, blocks), canDie(canDie), fovRadius(fovRadius), 
    canExplore(canExplore), maxHp(maxHp), hp(hp), mp(mp), atk(atk), 
    defense(defense), spd(spd) {
}

bool Entity::canSee(unsigned int x, unsigned int y) const {
    for (const auto& tile : fov) {
        if (x == tile.first && y == tile.second) {
            return true;
        }
    }
    
    return false;
}

bool Entity::moveOrAttack(unsigned int x, unsigned int y) {
    if ( engine.map->isWall(x, y) ) {
        return false;
    }
    
    for (const auto& entity : engine.entities) {
        if (entity->x == x && entity->y == y) {
            //printf("The %s laughs at your puny efforts to attack him!\n", 
            //    entity->name);
            return false;
        }
    }
    
    this->x = x;
    this->y = y;
    
    return true;
}

void Entity::attack(Entity &target) {
    if (target.canDie && !target.isDead()) {
        if (atk - target.defense > 0) {
            //printf("%s attacks %s for %g hit points.\n", name, target.name,
            //   power - target.defense);
        }
        else {
            //printf("%s attacks %s but it has no effect!\n", name, target.name);
        }
        
        target.takeDamage(atk);
    }
    else {
        //printf("%s attacks %s in vain.\n", name, target.name);
    }
}

double Entity::takeDamage(double damage) {
    damage -= defense;
    
    if (damage > 0) {
        hp -= damage;
        if (hp <= 0) {
            this->die();
        }
    }
    else {
        damage = 0;
    }
    
    return damage;
}

void Entity::die() {
    ch = '%';
    color = TCODColor::darkRed;
    name = name + " corpse";
    blocks = false;
    engine.sendToBack(this);
}
