#ifndef ITEM_HPP
#define ITEM_HPP

class Entity;

class Item : public Object {
    public:
        bool onMap;
        
        Item(unsigned int x, unsigned int y, unsigned int ch, 
            const TCODColor &color, std::string name, bool blocks, bool onMap);
        
        virtual ~Item() {};
        
        bool pick(Entity &wearer);
        virtual bool use(Entity &wearer);
};

class Healer : public Item {
    public:
        double amount;
        
        Healer(unsigned int x, unsigned int y, unsigned int ch, 
            const TCODColor &color, std::string name, bool blocks, bool onMap,
            double amount);
        ~Healer() {};
        
        bool use(Entity &wearer);
};

#endif
