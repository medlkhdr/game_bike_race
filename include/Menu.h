

#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
public:
    Menu();
    ~Menu();
    
    void init(const sf::Font& font);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    
    void moveUp();
    void moveDown();
    int getSelectedItem() const { return selectedItem; }
    void setSelectedItem(int item) { selectedItem = item; }
    
private:
    std::vector<std::string> labels;
    std::vector<sf::Text> menuItems;
    std::vector<sf::Text> shadowItems;
    int selectedItem;
    
    void updatePositions(sf::RenderWindow& window);
};

class AproposScreen {
public:
    AproposScreen();
    ~AproposScreen();
    
    void init(const sf::Font& font);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    
    void reset();
    
private:
    std::vector<std::vector<std::string>> aproposTexts;
    sf::Text aproposText;

    sf::Text aproposShadow;
    sf::Text returnButton;
    sf::Text returnShadow;
    
    size_t currentTextIndex;
    sf::Clock scrollClock;
};

#endif // MENU_H