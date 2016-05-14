#ifndef MONSTER_HPP
#define MONSTER_HPP

class Monster : public Entity {
    public:
      
        Monster(unsigned int x, unsigned int y, unsigned int ch, 
            const TCODColor &color, std::string name, bool blocks = true, 
            bool canDie = true, unsigned int fovRadius = 5, 
            bool canExplore = false, double maxHp = 20, double hp = 20, 
            double maxMp = 20, double mp = 20, double atk = 2, 
            double defense = 0, unsigned int spd = 1);
        
        ~Monster() {};
        
        void update();
        void die();
        
        protected:
            int moveCount;
            void moveOrAttack(unsigned int target_x, unsigned int target_y);
};

#endif
