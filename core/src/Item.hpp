#ifndef ITEM_HPP
#define ITEM_HPP

class Entity;

class Item : public Object {
    public:
        bool onMap;
        
        Item(unsigned int x, unsigned int y, unsigned int ch, 
            const TCODColor &color, std::string name, bool blocks, bool onMap);
        
        virtual ~Item() {};
        
        virtual bool pick(Entity &wearer);
        virtual bool use(Entity &wearer);
        virtual void drop(Entity &wearer);
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

class LightningBolt : public Item {
    public:
        double range;
        double damage;
        
        LightningBolt(unsigned int x, unsigned int y, unsigned int ch, 
            const TCODColor &color, std::string name, bool blocks, bool onMap,
            double range, double damage);
        
        bool use(Entity &wearer);
};

class Fireball : public LightningBolt {
    public:
        
        Fireball(unsigned int x, unsigned int y, unsigned int ch, 
            const TCODColor &color, std::string name, bool blocks, bool onMap,
            double range, double damage);
        bool use(Entity &wearer);
};

#endif
