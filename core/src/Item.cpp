#include "main.hpp"

Item::Item(unsigned int x, unsigned int y, unsigned int ch, 
    const TCODColor &color, std::string name, bool blocks, bool onMap) :
    Object(x, y, ch, color, name, blocks), onMap(onMap) {
}

bool Item::pick(Entity &wearer) {
    if ( wearer.bag->add(this) ) {
        onMap = false;
        return true;
    }
    
    return false;
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
