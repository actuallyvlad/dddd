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
    
    Entity *npc = new Entity(60, 13, '@', colors::berserker, "npc");
    
    entities.push(npc);
    map = new Map(80, 45);
    map->setFree(player->x, player->y);
    map->setFree(npc->x, npc->y);
}

Engine::~Engine() {
    entities.clearAndDelete();
    delete map;
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
    
    for (const auto& entity : entities) {
            if ( entity != player && player->canSee(entity->x, entity->y) ) {
                entity->render();
            }
    }
    
    player->render();
    
    TCODConsole::root->print(1, screenHeight - 2, "HP: %d / %d",
        (int)player->hp, (int)player->maxHp);
}

void Engine::sendToBack(Entity *entity) {
    entities.remove(entity);
    entities.insertBefore(entity, 0);
}
