#include <initializer_list>

class Gui {
    public:
        Gui();
        ~Gui();
        void render();
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
