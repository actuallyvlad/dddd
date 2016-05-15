#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <vector>

class Entity : public Object {
    public:
        std::vector<std::pair<unsigned int, unsigned int> > fov;
        
        bool canDie;
        unsigned int fovRadius;
        bool canExplore;
        double maxHp;
        double hp;
        double maxMp;
        double mp;
        double atk;
        double defense;
        unsigned int spd;
        unsigned int bagSize;
        Container* bag;
        //unsigned int rng;
        
        Entity(unsigned int x, unsigned int y, unsigned int ch, 
            const TCODColor &color, std::string name, bool blocks = true, 
            bool canDie = true, unsigned int fovRadius = 5, 
            bool canExplore = true, double maxHp = 100, double hp = 100, 
            double maxMp = 100, double mp = 100, double atk = 5, 
            double defense = 1, unsigned int spd = 1, 
            unsigned int bagSize = 5);
        
        virtual ~Entity();
        
        inline virtual void update() { }; //printf("%s whistles.\n", name);
        bool canSee(unsigned int x, unsigned int y) const;
        bool moveOrAttack(unsigned int x, unsigned int y);
        double takeDamage(double damage);
        double heal(double amount);
        void attack(Entity &target);
        inline bool isDead() { return hp <= 0; }
        virtual void die();
};

#endif
