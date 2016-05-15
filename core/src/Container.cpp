#include "main.hpp"
#include <algorithm>

Container::Container(unsigned int size) : 
    size(size) {
}

Container::~Container() {
    for ( const auto& item : inventory ) {
        delete item;
    }
    
    inventory.clear();
}

bool Container::add(Item* item) {
    if (size > 0 && inventory.size() >= size) {
        //inventory full
        return false;
    }
    
    inventory.push_back(item);
    return true;
}

void Container::remove(Item* item) {
    for ( const auto& tmp : inventory ) {
        if ( tmp == item ) {
            inventory.erase(std::remove(inventory.begin(), inventory.end(), 
                item), inventory.end());
            break;
        }
    }
}
