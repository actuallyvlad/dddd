#include "main.hpp"

Engine engine(80, 50);

int main() {
    bool running = true;
    
    while ( running && !TCODConsole::isWindowClosed() ) {
        running = engine.update();
        engine.render();
        TCODConsole::flush();
    }
    
    if ( engine.gameStatus == Engine::DEFEAT 
        || engine.gameStatus == Engine::VICTORY ) {
        
        while ( !TCODConsole::isWindowClosed() ) {
            TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &engine.lastKey, NULL);
            if (engine.lastKey.vk == TCODK_ENTER || engine.lastKey.vk == TCODK_ESCAPE) {
                break;
            }
        }
    }
    
    return 0;
}
