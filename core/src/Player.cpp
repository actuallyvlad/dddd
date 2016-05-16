#include <iostream>
#include "main.hpp"

static const int INVENTORY_WIDTH = 50;
static const int INVENTORY_HEIGHT = 28;
static TCODConsole console(INVENTORY_WIDTH, INVENTORY_HEIGHT);

Player::Player(unsigned int x, unsigned int y, unsigned int ch, 
    const TCODColor &color, std::string name, bool blocks, bool canDie,
    unsigned int fovRadius, bool canExplore, double maxHp, 
    double hp, double maxMp, double mp, double atk, double defense, 
    unsigned int spd, unsigned int bagSize) :
    
    Entity(x, y, ch, color, name, blocks, canDie, fovRadius, canExplore, maxHp, 
    hp, maxMp, mp, atk, defense, spd, bagSize) {
}

void Player::update() {
    if ( isDead() ) {
        return;
    }
    
    int dx = 0;
    int dy = 0;
    
    switch(engine.lastKey.vk) {
        case TCODK_CHAR:
            switch (engine.lastKey.c) {
                case 'w':
                    dy--;
                    break;
                case 'e':
                    dy--;
                    dx++;
                    break;
                case 'd':
                    dx++;
                    break;
                case 'c':
                    dy++;
                    dx++;
                    break;
                case 'x':
                    dy++;
                    break;
                case 'z':
                    dy++;
                    dx--;
                    break;
                case 'a':
                    dx--;
                    break;
                case 'q':
                    dy--;
                    dx--;
                    break;
                default:
                    handleActionKey(engine.lastKey.c);
                    break;
            }
            break;
        default:
            break;
    }
    
    if (dx != 0 || dy != 0) {
        engine.gameStatus = Engine::NEW_TURN;
        if ( moveOrAttack(x + dx, y + dy) ) {
            engine.updateFOV = true;
        }
    }
}

void Player::handleActionKey(int key) {
    switch(key) {
        case 's': {
                bool found = false;
                
                for ( const auto& item : engine.items->inventory ) {
                    if (item->onMap && item->x == x && item->y == y) {
                        found = true;
                        
                        if (item->pick(*this)) {
                            engine.gui->message(TCODColor::lightGrey, 
                                {"You pick up a "s, item->name, "."s});
                            engine.items->remove(item);
                        }
                        else {
                            engine.gui->message(TCODColor::red, 
                                {"Your inventory is full."s});
                        }
                    }
                }
                
                if (!found) {
                    engine.gui->message(TCODColor::lightGrey, 
                        {"There's nothing here that you can pick up."s});
                }
                
                engine.gameStatus = Engine::NEW_TURN;
            }
            break;
            
        case 'r': {
                Item *item = chooseFromInventory();
                if ( item ) {
                    item->drop(*this);
                    engine.gameStatus = Engine::NEW_TURN;
                }
            }
            break;
            
        case 'i': {
                Item *item = chooseFromInventory();
                
                if ( item ) {
                    item->use(*this);
                    if (engine.gameStatus == Engine::IDLE) {
                        engine.gameStatus = Engine::NEW_TURN;
                    }
                }
            }
            break;
        
        default:
            break;
    }
}

Item* Player::chooseFromInventory() {
    console.setDefaultBackground(TCODColor::darkRed);
    console.printFrame(0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, true,
        TCOD_BKGND_DEFAULT, "Inventory");
    
    console.setDefaultForeground(TCODColor::white);
    int shortcut = 'a';
    unsigned int y = 1;
    
    for (const auto& item : bag->inventory) {
        console.print(2, y, "(%c) %s", shortcut, item->name.c_str());
        ++y;
        ++shortcut;
    }
    
    TCODConsole::blit(&console, 0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT,
        TCODConsole::root, engine.screenWidth / 2 - INVENTORY_WIDTH / 2,
        engine.screenHeight / 2 - INVENTORY_HEIGHT / 2);
    
    TCODConsole::flush();
    
    TCOD_key_t key;
    TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);
    
    if ( key.vk == TCODK_CHAR ) {
        int itemIndex = key.c - 'a';
        
        if ( itemIndex >= 0 && (unsigned int) itemIndex < bag->inventory.size() ) {
            return bag->inventory[itemIndex];
        }
    }
    
    return NULL;
}


bool Player::moveOrAttack(unsigned int target_x, unsigned int target_y) {
    if (engine.map->isWall(target_x, target_y)) {
        return false;
    }

    for (const auto& entity : engine.entities) {
        if ( entity->canDie && !entity->isDead() && entity->x == target_x 
            && entity->y == target_y ) {
            attack(*entity);
            return false;
        }
    }
    
    for (const auto& entity : engine.entities) {
        if ( entity->canDie && entity->isDead() && entity->x == target_x 
            && entity->y == target_y) {
            
            engine.gui->message(TCODColor::white, {"There's a "s, entity->name,
                " here.\n"s});
        }
    }
    
    x = target_x;
    y = target_y;
    return true;
}

void Player::die() {
    Entity::die();
    engine.gui->message(TCODColor::white, {"You died!\n"s});
    engine.gameStatus = Engine::DEFEAT;
}
