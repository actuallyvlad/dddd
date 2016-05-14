#ifndef ENGINE_HPP
#define ENGINE_HPP

class Engine {
    public:
        enum GameStatus {
            STARTUP,
            IDLE,
            NEW_TURN,
            VICTORY,
            DEFEAT
        } gameStatus;
        
        unsigned int screenWidth;
        unsigned int screenHeight;
        TCOD_key_t lastKey;
        TCODList<Entity *> entities;
        Player *player;
        Map *map;
        bool updateFOV;
        
        Engine(unsigned int screenWidth = 80, 
            unsigned int screenHeight = 50);
        
        ~Engine();
        
        bool update();
        void render();
        void sendToBack(Entity *entity);
};

extern Engine engine;

#endif
