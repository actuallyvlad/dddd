#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <vector>

class Container {
    public:
        std::vector<Item*> inventory;
        unsigned int size;
        
        Container(unsigned int size);
        ~Container();
        
        bool add(Item* item);
        void remove(Item* item);
};

#endif
