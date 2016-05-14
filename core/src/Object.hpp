#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>

class Object {
    public:
        unsigned int x;
        unsigned int y;
        unsigned int ch;
        TCODColor color;
        std::string name;
        bool blocks;
        
        Object(unsigned int x, unsigned int y, unsigned int ch, 
            const TCODColor &color, std::string name, bool blocks);
        
        void render() const;
};

#endif
