#ifndef GUI_HPP
#define GUI_HPP

#include <initializer_list>

class Menu {
    public:
        enum MenuItemCode {
            NONE,
            NEW_GAME,
            EXIT
        };
        
        ~Menu();
        
        void clear();
        void addItem(MenuItemCode code, std::string label);
        MenuItemCode pick();
    protected:
        struct MenuItem {
            MenuItemCode code;
            std::string label;
        };
        
        std::vector<MenuItem *> items;
};

class Gui {
    public:
        Gui();
        ~Gui();
        
        Menu menu;
        
        void render();
        void clear();
        void message(const TCODColor &color, 
            std::initializer_list<std::string> text);
    
    protected:
        TCODConsole *console;
        
        struct Message {
            std::string text;
            TCODColor color;
            Message(std::string text, const TCODColor &color);
        };
        
        TCODList<Message *> log;
        
        void renderBar(unsigned int x, unsigned int y, unsigned int width,
            std::string name, double value, double maxValue, 
            const TCODColor &barColor, const TCODColor &backColor);
};

#endif
