#ifndef ROOM_HPP
#define ROOM_HPP

struct Room {
    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;
    unsigned int number;
    
    Room(unsigned int x, unsigned int y, unsigned int dx, 
        unsigned int dy, unsigned int number);
    void getCenter(unsigned int &x, unsigned int &y);
    bool intersects(Room other_room);
};

#endif
