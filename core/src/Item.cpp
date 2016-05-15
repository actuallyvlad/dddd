#include <cmath>
#include "main.hpp"

Item::Item(unsigned int x, unsigned int y, unsigned int ch, 
    const TCODColor &color, std::string name, bool blocks, bool onMap) :
    Object(x, y, ch, color, name, blocks), onMap(onMap) {
}

bool Item::pick(Entity &wearer) {
    if ( wearer.bag->add(this) ) {
        onMap = false;
        x = 0;
        y = 0;
        return true;
    }
    
    return false;
}

void Item::drop(Entity &wearer) {
    if ( wearer.bag->inventory.size() > 0 ) {
        wearer.bag->remove(this);
        this->x = wearer.x;
        this->y = wearer.y;
        this->onMap = true;
        engine.items->inventory.push_back(this);
        engine.gui->message(TCODColor::lightGrey, {wearer.name, " drops a "s,
            this->name, ".\n"s});
    }
}

bool Item::use(Entity &wearer) {
    if ( wearer.bag->inventory.size() > 0 ) {
        wearer.bag->remove(this);
        return true;
    }
    
    return false;
}

Healer::Healer(unsigned int x, unsigned int y, unsigned int ch, 
    const TCODColor &color, std::string name, bool blocks, bool onMap, 
    double amount) : 
    Item(x, y, ch, color, name, blocks, onMap), amount(amount) {
}

bool Healer::use(Entity &wearer) {
    if ( wearer.canDie ) {
        double amountHealed = wearer.heal(amount);
        
        if (amountHealed > 0) {
            return Item::use(wearer);
        }
    }
    
    return false;
}

LightningBolt::LightningBolt(unsigned int x, unsigned int y, 
    unsigned int ch, const TCODColor &color, std::string name, bool blocks, 
    bool onMap, double range, double damage) : 
    Item(x, y, ch, color, name, blocks, onMap), range(range), damage(damage) {
}

bool LightningBolt::use(Entity &wearer) {
    Entity* closestMonster = engine.getClosestMonster(wearer.x, wearer.y,
        range);
    
    if ( !closestMonster ) {
        engine.gui->message(TCODColor::lightGrey, 
            {"No enemy is close enough to strike."s});
        return false;
    }
    
    engine.gui->message(TCODColor::lightBlue, {"A lightning bolt strikes the "s,
            closestMonster->name, " with a loud thunder!\n"s,});
    engine.gui->message(TCODColor::lightBlue, {"The damage is "s, 
        std::to_string((int) round(damage)), " hit points.\n"s});
    
    closestMonster->takeDamage(damage);
    
    return Item::use(wearer);
}

Fireball::Fireball(unsigned int x, unsigned int y, unsigned int ch, 
    const TCODColor &color, std::string name, bool blocks, bool onMap,
    double range, double damage) : 
    LightningBolt(x, y, ch, color, name, blocks, onMap, range, damage) {
}

bool Fireball::use(Entity &wearer) {
    engine.gui->message(TCODColor::cyan, 
        {"Move target with move keys, when ready to attack press Enter"s});
    
    unsigned int x = 0;
    unsigned int y = 0;
    
    if ( !engine.pickATile(x, y, wearer.fovRadius) ) {
        return false;
    }
    
    engine.gui->message(TCODColor::orange, 
        {"The fireball explodes, burning everything within "s, 
        std::to_string((int) round(range)), " tiles!\n"s});
    
    for ( const auto& entity : engine.entities ) {
        if ( entity->canDie && !entity->isDead() 
            && entity->getDistance(x, y) <= range ) {
            
            engine.gui->message(TCODColor::orange, {"The "s, entity->name, 
                " gets burned for "s, std::to_string( (int) round(damage) ), 
                " hit points.\n"s});
            
            entity->takeDamage(damage);
        }
    }
    
    return Item::use(wearer);
}


