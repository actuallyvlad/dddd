#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include "Room.hpp"
#include "Entity.hpp"

struct Tile {
    bool canWalk;
    bool blockSight;
    bool explored;
    enum tileType {
        WALL,
        GROUND
    } type;
    
    Tile(bool canWalk = false, bool blockSight = false, bool explored = false,
        tileType = WALL);
};

class Map {
    public:
        unsigned int width;
        unsigned int height;
        
        Map(unsigned int width, unsigned int height);
        ~Map();
        
        bool isWall(unsigned int x, unsigned int y) const;
        bool isExplored(unsigned int x, unsigned int y) const;
        bool canWalk(unsigned int x, unsigned int y) const;
        void setFree(unsigned int &x, unsigned int &y);
        void setFree(const Room &room, unsigned int &x, unsigned int &y); 
        void computeFOV(Entity &entity) const;
        void render(Entity &viewer, bool updateFOV) const;
        
    protected:
        Tile *tiles;
        std::vector<Room> *rooms;
        void digRoom(const Room &room);
        void digHTunnel(unsigned int x1, unsigned int x2, unsigned int y);
        void digVTunnel(unsigned int y1, unsigned int y2, unsigned int x);
        void placeObjects(Room &room);
        void setWall(unsigned int x, unsigned int y);
        void setGround(unsigned int x, unsigned int y);
        //void placeObjects(const Room &room);
};

#endif
