#include "main.hpp"

Object::Object(unsigned int x, unsigned int y, unsigned int ch,
    const TCODColor &color, std::string name, bool blocks) :
    x(x), y(y), ch(ch), color(color), name(name), blocks(blocks) {
}

void Object::render() const {
    TCODConsole::root->setChar(x, y, ch);
    TCODConsole::root->setCharForeground(x, y, color);
}

