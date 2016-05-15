#include "main.hpp"

static const unsigned int PANEL_HEIGHT = 7;
static const unsigned int BAR_WIDTH = 20;
// message x position
static const unsigned int MSG_X = BAR_WIDTH + 2;
static const unsigned int MSG_HEIGHT = PANEL_HEIGHT;
static const unsigned int MSG_WIDTH = engine.screenWidth - MSG_X;

Gui::Gui() {
    console = new TCODConsole(engine.screenWidth, PANEL_HEIGHT);
}

Gui::~Gui() {
    delete console;
    clear();
}

void Gui::clear() {
    log.clearAndDelete();
}

Menu::~Menu() {
    clear();
}

void Menu::clear() {
    for ( const auto& item : items ) {
        delete item;
    }
    
    items.clear();
}

void Menu::addItem(MenuItemCode code, std::string label) {
    MenuItem *item = new MenuItem();
    item->code = code;
    item->label = label;
    items.push_back(item);
}

Menu::MenuItemCode Menu::pick() {
    static TCODImage img1("images/menu_background_1.png");
    
    int selectedItem = 0;
    
    while ( !TCODConsole::isWindowClosed() ) {
        img1.blit2x(TCODConsole::root, 0, 0);
        TCODConsole::root->setDefaultForeground(TCODColor::lightGrey);
        TCODConsole::root->print(20, 2, 
            "Downfall of the Darkest Destructive Doom");
        
        int currentItem = 0;
        
        for ( const auto& item : items ) {
            if ( currentItem == selectedItem ) {
                TCODConsole::root->setDefaultForeground(TCODColor::lighterOrange);
            }
            else {
                TCODConsole::root->setDefaultForeground(TCODColor::lightGrey);
            }
            
            TCODConsole::root->print(10, 10 + currentItem * 3, 
                item->label.c_str());
            
            ++currentItem;
        }
        
        TCODConsole::flush();
        
        TCOD_key_t key;
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
        
        switch(key.vk) {
            case TCODK_CHAR:
                switch(key.c) {
                    case 'w':
                        --selectedItem;
                        
                        if ( selectedItem < 0 ) {
                            selectedItem = items.size() - 1;
                        }
                        
                        break;
                    
                    case 'x':
                        selectedItem = (selectedItem + 1) % items.size();
                        break;
                }
                
                break;
            case TCODK_ENTER:
                return items[selectedItem]->code;
            default:
                break;
        }
    }
    
    return NONE;
}

void Gui::render() {
    console->setDefaultBackground(colors::bgDefault);
    console->clear();
    
    // draw the hp bar
    renderBar(1, 1, BAR_WIDTH, "HP", engine.player->hp, engine.player->maxHp,
        TCODColor::lightRed, TCODColor::darkerRed);
    
    // draw the mp bar
    renderBar(1, 2, BAR_WIDTH, "MP", engine.player->mp, engine.player->maxMp,
        TCODColor::lightBlue, TCODColor::darkerBlue);
    
    // blit the GUI console on the root console
    TCODConsole::blit(console, 0, 0, engine.screenWidth, PANEL_HEIGHT,
        TCODConsole::root, 0, engine.screenHeight - PANEL_HEIGHT);
}

void Gui::renderBar(unsigned int x, unsigned int y, unsigned int width,
    std::string name, double value, double maxValue, 
    const TCODColor &barColor, const TCODColor &backColor) {

    console->setDefaultBackground(backColor);
    console->rect(x, y, width, 1, false, TCOD_BKGND_SET);
    
    unsigned int barWidth = (unsigned int) (value / maxValue * width);
    
    if ( barWidth > 0 ) {
        console->setDefaultBackground(barColor);
        console->rect(x, y, barWidth, 1, false, TCOD_BKGND_SET);
    }
    
    console->setDefaultForeground(TCODColor::white);
    
    console->printEx(x + width / 2, y, TCOD_BKGND_NONE, TCOD_CENTER,
        "%s : %g/%g", name.c_str(), value, maxValue);
    
    // draw the message log
    unsigned int counter = 1;
    double colorCoefficient = 0.4;
    
    for (const auto& message : log) {
        console->setDefaultForeground(message->color * colorCoefficient);
        console->print(MSG_X, counter, message->text.c_str());
        ++counter;
        
        if ( colorCoefficient < 1.0 ) {
            colorCoefficient += 0.3;
        }
    }
}

Gui::Message::Message(std::string text, const TCODColor &color) :
    text(text), color(color) {
}

void Gui::message(const TCODColor &color, 
    std::initializer_list<std::string> text) {
    
    std::string buffer = "";
    for (const auto& line : text) {
        buffer += line;
    }
    
    unsigned int last_pos = 0;
    
    for (size_t i = 0; i < buffer.length(); ++i) {
        if (i % MSG_WIDTH == 0 && i != 0) {
            Message *msg = new Message(buffer.substr(last_pos, MSG_WIDTH), color);
            log.push(msg);
            last_pos = i;
        }
    }
    
    if ( last_pos < buffer.length() ) {
        Message *msg = new Message(buffer.substr(last_pos, 
            buffer.length()), color);
        log.push(msg);
    }
    
    while ( (unsigned int) log.size() >= MSG_HEIGHT ) {
        Message *toRemove = log.get(0);
        log.remove(toRemove);
        delete toRemove;
    }
}
