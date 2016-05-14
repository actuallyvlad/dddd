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
    
    map = new Map(80, 43);
    gui = new Gui();
    
    map->setFree(player->x, player->y);
}

Engine::~Engine() {
    entities.clearAndDelete();
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
    
    for (const auto& entity : entities) {
            if ( entity != player && player->canSee(entity->x, entity->y) ) {
                entity->render();
            }
    }
    
    player->render();
    gui->render();
}

void Engine::sendToBack(Entity *entity) {
    entities.remove(entity);
    entities.insertBefore(entity, 0);
}
