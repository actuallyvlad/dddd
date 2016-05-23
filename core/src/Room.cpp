#include "Room.hpp"

Room::Room(unsigned int x, unsigned int y, unsigned int dx, 
            unsigned int dy) :
    x(x), y(y), w(x + dx), h(y + dy) {
}

void Room::getCenter(unsigned int &cx, unsigned int &cy) {
    cx = (x + w) / 2;
    cy = (y + h) / 2;
}

bool Room::intersects(Room otherRoom) {
    return (x <= otherRoom.w && w >= otherRoom.x
            && y <= otherRoom.h && h >= otherRoom.y);
}
