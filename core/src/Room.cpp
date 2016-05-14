#include "Room.hpp"

Room::Room(unsigned int x, unsigned int y, unsigned int dx, 
            unsigned int dy, unsigned int number) :
    x(x), y(y), w(x + dx), h(y + dy), number(number) {
}

void Room::getCenter(unsigned int &cx, unsigned int &cy) {
    cx = (x + w) / 2;
    cy = (y + h) / 2;
}

bool Room::intersects(Room other_room) {
    return (x <= other_room.w && w >= other_room.x
            && y <= other_room.h && h >= other_room.y);
}
