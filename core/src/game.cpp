#include "main.hpp"

Engine engine(80, 50);

int main() {
    bool running = true;
    
    while ( running && !TCODConsole::isWindowClosed() ) {
        running = engine.update();
        engine.render();
        TCODConsole::flush();
    }
    
    return 0;
}
