#include <iostream>
#include "main.hpp"

Player::Player(unsigned int x, unsigned int y, unsigned int ch, 
    const TCODColor &color, std::string name, bool blocks, bool canDie,
    unsigned int fovRadius, bool canExplore, double maxHp, 
    double hp, double mp, double atk, double defense, unsigned int spd) :
    
    Entity(x, y, ch, color, name, blocks, canDie, fovRadius, canExplore, maxHp, 
    hp, mp, atk, defense, spd) {
}

void Player::update() {
    if ( isDead() ) {
        return;
    }
    
    int dx = 0;
    int dy = 0;
    
    switch(engine.lastKey.vk) {
        case TCODK_CHAR:
            switch (engine.lastKey.c) {
                case 'w':
                    dy--;
                    break;
                case 'e':
                    dy--;
                    dx++;
                    break;
                case 'd':
                    dx++;
                    break;
                case 'c':
                    dy++;
                    dx++;
                    break;
                case 'x':
                    dy++;
                    break;
                case 'z':
                    dy++;
                    dx--;
                    break;
                case 'a':
                    dx--;
                    break;
                case 'q':
                    dy--;
                    dx--;
                    break;
            }
            break;
        default:
            break;
    }
    
    if (dx != 0 || dy != 0) {
        engine.gameStatus = Engine::NEW_TURN;
        if ( moveOrAttack(x + dx, y + dy) ) {
            engine.updateFOV = true;
        }
    }
}

bool Player::moveOrAttack(unsigned int target_x, unsigned int target_y) {
    if (engine.map->isWall(target_x, target_y)) {
        return false;
    }

    for (const auto& entity : engine.entities) {
        if ( entity->canDie && !entity->isDead() && entity->x == target_x 
            && entity->y == target_y ) {
            attack(*entity);
            return false;
        }
    }
    
    for (const auto& entity : engine.entities) {
        if ( entity->canDie && entity->isDead() && entity->x == target_x 
            && entity->y == target_y) {
            std::cout << "There's a " << entity->name << " here." << std::endl;
        }
    }
    
    x = target_x;
    y = target_y;
    return true;
}

void Player::die() {
    Entity::die();
    std::cout << "You died!" << std::endl;
    engine.gameStatus = Engine::DEFEAT;
}
