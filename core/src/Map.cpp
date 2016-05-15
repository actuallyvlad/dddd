#include <algorithm>
#include <cmath>
#include "main.hpp"

static const unsigned int ROOM_MAX_SIZE = 15;
static const unsigned int ROOM_MIN_SIZE = 6;
static const unsigned int MAX_ROOMS = 20;
static const unsigned int MAX_ROOM_MONSTERS = 3;
static const unsigned int MAX_ROOM_ITEMS = 2;

Tile::Tile(bool canWalk, bool blockSight, bool explored, tileType type) :
    canWalk(canWalk), blockSight(canWalk || blockSight), explored(explored),
    type(type) {
}

Map::Map(unsigned int width, unsigned int height) : 
        width(width), height(height) {
    tiles = new Tile[width * height];
    std::fill_n(tiles, width * height, Tile(false, true));
    rooms = new std::vector<Room>;
    unsigned int num_rooms = 0;
    unsigned int prev_cx = 0;
    unsigned int prev_cy = 0;
    unsigned int cur_cx = 0;
    unsigned int cur_cy = 0;
    TCODRandom *rng = TCODRandom::getInstance();
    
    for (unsigned int i = 0; i < MAX_ROOMS; ++i) {
        unsigned int w = rng->getInt(ROOM_MIN_SIZE, ROOM_MAX_SIZE);
        unsigned int h = rng->getInt(ROOM_MIN_SIZE, ROOM_MAX_SIZE);
        unsigned int x = rng->getInt(0, width - w - 1);
        unsigned int y = rng->getInt(0, height - h - 1);
        
        rooms->push_back(Room(x, y, w, h));
        
        bool overlap = false;
        size_t last = rooms->size() - 1;
        
        for (size_t j = 0; j < last; ++j) {
            if ( (*rooms)[last].intersects((*rooms)[j]) ) {
                overlap = true;
                rooms->pop_back();
                break;
            }
        }
        
        if ( !overlap ) {
            digRoom((*rooms)[num_rooms]);
            placeItems((*rooms)[num_rooms]);
            placeMonsters((*rooms)[num_rooms]);
            
            if (num_rooms > 0) {
                (*rooms)[num_rooms].getCenter(cur_cx, cur_cy);
                (*rooms)[num_rooms-1].getCenter(prev_cx, prev_cy);
                
                if (rng->getInt(0, 1)) {
                    digHTunnel(prev_cx, cur_cx, prev_cy);
                    digVTunnel(prev_cy, cur_cy, cur_cx);
                }
                else {
                    digVTunnel(prev_cy, cur_cy, prev_cx);
                    digHTunnel(prev_cx, cur_cx, cur_cy);
                }
            }
            
            ++num_rooms;
        }
    }
}

bool Map::isWall(unsigned int x, unsigned int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false;
    }
    
    return (tiles[x + y*width].type == Tile::WALL);
}

bool Map::isExplored(unsigned int x, unsigned int y) const {
    return tiles[x + y*width].explored;
}

void Map::setGround(unsigned int x, unsigned int y) {
        tiles[x + y * width].canWalk = true;
        tiles[x + y * width].blockSight = false;
        tiles[x + y * width].type = Tile::GROUND;
}

void Map::setWall(unsigned int x, unsigned int y) {
    tiles[x + y*width].canWalk = false;
    tiles[x + y*width].blockSight = true;
    tiles[x + y*width].type = Tile::WALL;
}

bool Map::canWalk(unsigned int x, unsigned int y) const {
    if ( isWall(x, y) ) {
        return false;
    }
    
    for (const auto& entity : engine.entities) {
        if (entity->x == x && entity->y == y && entity->blocks) {
            return false;
        }
    }
    
    return true;
}

void Map::placeItems(Room &room) {
    TCODRandom *rng = TCODRandom::getInstance();
    unsigned int numOfItems = rng->getInt(0, MAX_ROOM_ITEMS);
    unsigned int free_x = 0;
    unsigned int free_y = 0;
    
    while ( numOfItems > 0 ) {
        unsigned int chance = rng->getInt(0, 100);
        setFree(room, free_x, free_y);
        
        if ( chance < 30 ) {
            engine.items->inventory.push_back(new Healer(free_x, free_y, '!', 
                TCODColor::violet, "health potion", false, true, 25));
        }
        else if ( chance < 45 ) {
            engine.items->inventory.push_back(new LightningBolt(free_x, 
                free_y, '?', TCODColor::lightYellow, "scroll of lightning bolt",
                false, true, 5, 15));
        }
        else {
            engine.items->inventory.push_back(new Fireball(free_x, 
                free_y, '?', TCODColor::lightYellow, "scroll of fireball",
                false, true, 5, 50));
        }
        
        --numOfItems;
    }
}

void Map::placeMonsters(Room &room) {
    TCODRandom *rng = TCODRandom::getInstance();
    unsigned int numOfMonsters = rng->getInt(0, MAX_ROOM_MONSTERS);
    unsigned int free_x = 0;
    unsigned int free_y = 0;
    
    while (numOfMonsters > 0) {
        unsigned int chance = rng->getInt(0, 100);
        setFree(room, free_x, free_y);
        
        if (chance < 20) {
            engine.entities.push(new Monster(free_x, free_y, 'z', 
                colors::zombie, "Zombie", true, true, 3, false,
                50, 50, 20, 20, 8, 5, 1));
        }
        else if (chance < 40) {
            engine.entities.push(new Monster(free_x, free_y, 'w', 
                colors::darkWizard, "Dark Wizard", true, true, 3, false,
                50, 50, 50, 50, 10, 4, 1));
        }
        else if (chance < 60) {
            engine.entities.push(new Monster(free_x, free_y, 'i', 
                colors::imp, "Imp", true, true, 3, false, 40, 40,
                40, 40, 6, 4, 2));
        }
        else if (chance < 80) {
            engine.entities.push(new Monster(free_x, free_y, 's', 
                colors::skeletonArcher, "Skeleton Archer", true, true, 4, 
                false, 50, 50, 10, 10, 5, 5, 2));
        }
        else {
            engine.entities.push(new Monster(free_x, free_y, 'n', 
                colors::poltergeist, "Poltergeist", false, true, 3, false,
                40, 40, 20, 20, 8, 10, 1));
        }
        
        --numOfMonsters;
    }
}

/* The field of vision algorithm
 * It starts at player coordinates and cast 360 rays
 * (if step is 1, less is step is more than 1) in every direction,
 * until it hits a wall.
 * 
 * When ray hits ground, ground coordinates are added to fov.
 * 
 * Ray is casted by adding to px (initialy it is player's x coord)
 * value of sin(i degrees) and to py (player's y) value of cos(i degrees),
 * fovRadius times, and checking for collision with wall every step. */
void Map::computeFOV(Entity &entity) const {
    double px = 0;
    double py = 0;
    
    if (entity.fov.size()) {
        entity.fov.clear();
    }
    
    entity.fov.push_back(std::make_pair(entity.x, entity.y));
    
    if (entity.canExplore) {
        tiles[entity.x + entity.y * width].explored = true;
    }
    
    unsigned int step = 5;
    
    for (unsigned int i = 0; i <= 360; i += step) {
        double dx = fovMath::getCos(i);
        double dy = fovMath::getSin(i);
        
        px = entity.x;
        py = entity.y;
        
        for (unsigned int j = 0; j < entity.fovRadius; ++j) {
            px += dx;
            py += dy;
            
            // final coordinates of a ray
            unsigned int f_x = std::round(px);
            unsigned int f_y = std::round(py);
            
            if ( f_x < 0 || f_y < 0 || f_x > width || f_y > height) {
                break;
            }
            
            entity.fov.push_back(std::make_pair(f_x, f_y));
            
            if (entity.canExplore) {
                tiles[f_x + f_y * width].explored = true;
            }
            
            if (tiles[f_x + f_y * width].blockSight) {
                break;
            }
        }
    }
}
void Map::setFree(unsigned int &x, unsigned int &y) {
    TCODRandom *rng = TCODRandom::getInstance();
    unsigned int rand_x = 0;
    unsigned int rand_y = 0;
    bool found = false;
    
    while( !found ) {
        rand_x = rng->getInt(1, width-1);
        rand_y = rng->getInt(1, height-1);
        
        if ( canWalk(rand_x, rand_y) ) {
            x = rand_x;
            y = rand_y;
            found = true;
        }
    }
}

void Map::setFree(const Room &room, unsigned int &x, unsigned int &y) {
    TCODRandom *rng = TCODRandom::getInstance();
    unsigned int rand_x = 0;
    unsigned int rand_y = 0;
    bool found = false;
    
    while( !found ) {
        rand_x = rng->getInt(room.x, room.w);
        rand_y = rng->getInt(room.y, room.h);
        
        if ( canWalk(rand_x, rand_y) ) {
            x = rand_x;
            y = rand_y;
            found = true;
        }
    }
}

void Map::digHTunnel(unsigned int x1, unsigned int x2, unsigned int y) {
        unsigned int min = std::min(x1, x2);
        unsigned int max = std::max(x1, x2);
        
        for (unsigned int x = min; x <= max; ++x) {
            if ( isWall(x, y) ) {
                setGround(x, y);
            }
        }
}

void Map::digVTunnel(unsigned int y1, unsigned int y2, unsigned int x) {
        unsigned int min = std::min(y1, y2);
        unsigned int max = std::max(y1, y2);
        
        for (unsigned int y = min; y <= max; ++y) {
            if ( isWall(x, y) ) {
                setGround(x, y);
            }
        }
}

void Map::digRoom(const Room &room) {
    for (unsigned int x = room.x + 1; x < room.w; ++x) {
        for (unsigned int y = room.y + 1; y < room.h; ++y) {
            setGround(x, y);
        }
    }
}

void Map::render(Entity &viewer, bool updateFOV) const {
    if (updateFOV) {
        computeFOV(viewer);
    }
    
    for (unsigned int x = 0; x < width; ++x) {
        for (unsigned int y = 0; y < height; ++y) {
            bool inSight = viewer.canSee(x, y);
            bool explored = isExplored(x, y);
            bool wall = isWall(x, y);
            
            if ( wall ) {
                TCODConsole::root->setChar(x, y, '#');
                if (inSight) {
                    TCODConsole::root->setCharForeground(x, y, colors::lightWall);
                }
                else if (explored) {
                    TCODConsole::root->setCharForeground(x, y, colors::darkWall);
                }
                else {
                    TCODConsole::root->setCharForeground(x, y, colors::bgDefault);
                }
            }
            else {
                TCODConsole::root->setChar(x, y, '.');
                if (inSight) {
                    TCODConsole::root->setCharForeground(x, y, colors::lightGround);
                }
                else if (explored) {
                    TCODConsole::root->setCharForeground(x, y, colors::darkGround);
                }
                else {
                    TCODConsole::root->setCharForeground(x, y, colors::bgDefault);
                }
            }
        }
    }
}

Map::~Map() {
    delete[] tiles;
    delete rooms;
}
