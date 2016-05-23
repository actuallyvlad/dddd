#ifndef ROOM_HPP
#define ROOM_HPP

struct Room {
    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;
    
    Room(unsigned int x, unsigned int y, unsigned int dx, 
        unsigned int dy);
    void getCenter(unsigned int &x, unsigned int &y);
    bool intersects(Room otherRoom);
};

#endif
