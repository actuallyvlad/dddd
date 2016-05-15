#include "main.hpp"

Engine::Engine(unsigned int screenWidth, unsigned int screenHeight) : 
    gameStatus(STARTUP), screenWidth(screenWidth), screenHeight(screenHeight) {
    
    TCODConsole::setCustomFont("fonts/Cheepicus_16x16.png", 
            TCOD_FONT_LAYOUT_ASCII_INROW | TCOD_FONT_TYPE_GREYSCALE, 16, 16);
    
    TCODConsole::initRoot(screenWidth, screenHeight, 
        "Downfall of the Darkest Destructive Doom", false);
    
    TCODConsole::root->setDefaultBackground(colors::bgDefault);
    player = new Player(0, 0);
    entities.push(player);
    items = new Container(-1);
    map = new Map(80, 43);
    gui = new Gui();
    
    map->setFree(player->x, player->y);
}

Engine::~Engine() {
    entities.clearAndDelete();
    delete items;
    delete map;
    delete gui;
}

bool Engine::update() {
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, NULL);
    updateFOV = false;
    
    if (gameStatus == STARTUP) {
        updateFOV = true;
    }
    
    gameStatus = IDLE;
    
    switch(lastKey.vk) {
        case TCODK_ENTER:
            if (lastKey.lalt) {
                TCODConsole::setFullscreen( !TCODConsole::isFullscreen() );
                TCODMouse::showCursor( !TCODMouse::isCursorVisible() );
            }
            break;
        case TCODK_ESCAPE:
            return false;
        default:
            player->update();
    }
    
    if (gameStatus == NEW_TURN) {
        for (const auto& entity : entities) {
                if ( entity != player ) {
                    entity->update();
                }
        }
    }
    
    return true;
}

void Engine::render() {
    TCODConsole::root->clear();
    map->render(*player, updateFOV);
    
    for (const auto& item : items->inventory) {
        if ( item->onMap && player->canSee(item->x, item->y) ) {
            item->render();
        }
    }
    
    for (const auto& entity : entities) {
        if ( entity != player && player->canSee(entity->x, entity->y) ) {
            entity->render();
        }
    }
    
    player->render();
    gui->render();
}

bool Engine::pickATile(unsigned int &x, unsigned int &y, double maxRange) {
    TCOD_key_t targetKey;
    
    Object *target = new Object(player->x, player->y, '^', TCODColor::red,
        "target", false);
    
    while ( targetKey.vk != TCODK_ENTER ) {
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &targetKey, NULL);
        render();
        
        for ( const auto& tile : player->fov ) {
            if ( maxRange == 0 || 
                player->getDistance(tile.first, tile.second) <= maxRange ) {
                
                TCODColor color = TCODConsole::root->getCharBackground(tile.first,
                    tile.second);
                
                color = color * TCODColor::red;
                
                TCODConsole::root->setCharBackground(tile.first, tile.second, color);
            }
        }
        
        if ( maxRange == 0 
            || player->getDistance(target->x, target->y) <= maxRange ) {
            
            target->render();
        }
        
        TCODConsole::flush();
        
        switch(targetKey.vk) {
            case TCODK_CHAR:
                switch (targetKey.c) {
                    case 'w':
                        if (player->getDistance(target->x, target->y - 1) 
                            <= maxRange) {
                            
                            target->y--;
                        }
                        break;
                    case 'e':
                        if (player->getDistance(target->x + 1, target->y - 1) 
                            <= maxRange) {
                            
                            target->y--;
                            target->x++;
                        }
                        break;
                    case 'd':
                        if (player->getDistance(target->x + 1, target->y) 
                            <= maxRange) {
                            
                            target->x++;
                        }
                        break;
                    case 'c':
                        if (player->getDistance(target->x + 1, target->y + 1) 
                            <= maxRange) {
                            
                            target->y++;
                            target->x++;
                        }
                        break;
                    case 'x':
                        if (player->getDistance(target->x, target->y + 1) 
                            <= maxRange) {
                            
                            target->y++;
                        }
                        break;
                    case 'z':
                        if (player->getDistance(target->x - 1, target->y + 1) 
                            <= maxRange) {
                            
                            target->y++;
                            target->x--;
                        }
                        break;
                    case 'a':
                        if (player->getDistance(target->x - 1, target->y) 
                            <= maxRange) {
                            
                            target->x--;
                        }
                        break;
                    case 'q':
                        if (player->getDistance(target->x - 1, target->y - 1) 
                            <= maxRange) {
                            
                            target->y--;
                            target->x--;
                        }
                        break;
                    default:
                        break;
                }
                break;
            case TCODK_ESCAPE:
                delete target;
                return false;
            default:
                break;
        }
    }
    
    x = target->x;
    y = target->y;
    delete target;
    return true;
}

Entity* Engine::getClosestMonster(unsigned int x, unsigned int y, 
    double range) const {
    Entity* closest = NULL;
    double bestDistance = 1E6;
    
    for (const auto& entity : entities) {
        if ( entity != player && entity->canDie && !entity->isDead() ) {
            double distance = entity->getDistance(x, y);
            if ( distance < bestDistance 
                && (distance <= range || range == 0.0) ) {
                
                bestDistance = distance;
                closest = entity;
            }
        }
    }
    
    return closest;
}

void Engine::sendToBack(Entity *entity) {
    entities.remove(entity);
    entities.insertBefore(entity, 0);
}
