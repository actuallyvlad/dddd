#include <cmath>
#include "main.hpp"

Entity::Entity(unsigned int x, unsigned int y, unsigned int ch, 
    const TCODColor &color, std::string name, bool blocks, bool canDie,
    unsigned int fovRadius, bool canExplore, double maxHp, 
    double hp, double maxMp, double mp, double atk, double defense, 
    unsigned int spd, unsigned int bagSize) :
    
    Object(x, y, ch, color, name, blocks), canDie(canDie), fovRadius(fovRadius), 
    canExplore(canExplore), maxHp(maxHp), hp(hp), maxMp(maxMp), mp(mp), atk(atk), 
    defense(defense), spd(spd), bagSize(bagSize) {
        bag = new Container(bagSize);
}

Entity::~Entity() {
    delete bag;
}

bool Entity::canSee(unsigned int x, unsigned int y) const {
    for (const auto& tile : fov) {
        if (x == tile.first && y == tile.second) {
            return true;
        }
    }
    
    return false;
}

double Entity::getDistance(unsigned int target_x, unsigned int target_y) {
    int dx =  x - target_x;
    int dy = y - target_y;
    
    return sqrt(dx*dx + dy*dy);
}

bool Entity::moveOrAttack(unsigned int x, unsigned int y) {
    if ( engine.map->isWall(x, y) ) {
        return false;
    }
    
    for (const auto& entity : engine.entities) {
        if (entity->x == x && entity->y == y) {
            engine.gui->message(TCODColor::white, {"The "s, name, 
                " laughs at your puny efforts to attack him!\n"s});
            return false;
        }
    }
    
    this->x = x;
    this->y = y;
    
    return true;
}

void Entity::attack(Entity &target) {
    unsigned int atkTimes = spd;
    
    while (atkTimes != 0) {
        if (target.canDie && !target.isDead()) {
            if (atk - target.defense > 0) {
                engine.gui->message(TCODColor::white, {name, " attacks "s, target.name,
                    " for "s, std::to_string( (int)(atk - target.defense) ), 
                    " hit points.\n"s});
            }
            else {
                engine.gui->message(TCODColor::white, {name, " attacks "s, target.name,
                    " but it has no effect!\n"s});
            }
            
            target.takeDamage(atk);
        }
        else {
            engine.gui->message(TCODColor::white, {name, " attacks "s, target.name,
                " in vain.\n"s});
        }
        
        --atkTimes;
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

double Entity::heal(double amount) {
    hp += amount;
    
    if ( hp > maxHp ) {
        amount -= hp - maxHp;
        hp = maxHp;
    }
    
    return amount;
}

void Entity::die() {
    ch = '%';
    color = TCODColor::darkRed;
    name = name + " corpse";
    blocks = false;
    
    if ( bag->inventory.size() > 0) {
        for ( const auto& item : bag->inventory ) {
            item->x = x;
            item->y = y;
            item->onMap = true;
        }
    }
    
    engine.sendToBack(this);
}
