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
        Gui *gui;
        TCOD_key_t lastKey;
        TCODList<Entity *> entities;
        Container *items;
        Player *player;
        Map *map;
        bool updateFOV;
        
        Engine(unsigned int screenWidth = 80, 
            unsigned int screenHeight = 50);
        
        ~Engine();
        
        void init();
        void term();
        void render();
        bool update();
        bool pickATile(unsigned int &x, unsigned int &y, 
            double maxRange = 0.0);
        Entity* getClosestMonster(unsigned int x, unsigned int y, 
            double range) const;
        void sendToBack(Entity *entity);
};

extern Engine engine;

#endif
