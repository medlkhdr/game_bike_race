#include "../include/Menu.h"
#include <cmath>

// Menu Class Implementation
Menu::Menu() : selectedItem(0) 
{
    labels = {"Jouer", "A propos", "Quitter"};
}

Menu::~Menu() {
}

void Menu::init(const sf::Font& font) {
    menuItems.resize(labels.size());
    shadowItems.resize(labels.size());
    
    for (size_t i = 0; i < labels.size(); ++i) {
        menuItems[i].setFont(font);
        menuItems[i].setString(labels[i]);
        menuItems[i].setCharacterSize(32);
        menuItems[i].setFillColor(sf::Color::White);
        
        shadowItems[i] = menuItems[i];
        shadowItems[i].setFillColor(sf::Color::Black);
    }
}

void Menu::update(float dt) {
    static sf::Clock pulseTimer;
    float time = pulseTimer.getElapsedTime().asSeconds();
    int alpha = static_cast<int>(127.5f * (std::sin(time * 2 * 3.1415f) + 1));
    
    for (int i = 0; i < menuItems.size(); ++i) {
        if (i == selectedItem)
        {
            menuItems[i].setFillColor(sf::Color(255, 255, 0, alpha));
           shadowItems[i].setFillColor(sf::Color(0, 0, 0, alpha));
        } else {
            menuItems[i].setFillColor(sf::Color::White);
            shadowItems[i].setFillColor(sf::Color::Black);
        }
    }
}

void Menu::draw(sf::RenderWindow& window) {
    updatePositions(window);
    
    for (size_t i = 0; i < menuItems.size(); ++i) {
        window.draw(shadowItems[i]);
        window.draw(menuItems[i]);
    }
}

void Menu::moveUp()
{
    selectedItem = (selectedItem + menuItems.size() - 1) % menuItems.size();
}

void Menu::moveDown() {
    selectedItem = (selectedItem + 1) % menuItems.size();
}

void Menu::updatePositions(sf::RenderWindow& window) {
    float centerX = window.getSize().x / 2.f;
    float startY = window.getSize().y / 2.f - 80.f;
    
    for (size_t i = 0; i < menuItems.size(); ++i) {
        sf::FloatRect bounds = menuItems[i].getLocalBounds();
        float x = centerX - (bounds.width / 2.f + bounds.left);
        float y = startY + i * 60.f - bounds.top;
        
        shadowItems[i].setPosition(x + 2, y + 2);
        menuItems[i].setPosition(x, y);
    }
}

// AproposScreen Class Implementation
AproposScreen::AproposScreen() : currentTextIndex(0) {
    // Initialize the about page texts
    aproposTexts = {{
        "Bienvenue dans notre projet de mini-jeu de velo",
        "Realise par Mahmoud Moukouch & Mohamed Lakhdar",
        "Encadre par Professeur Rachida Hannane",
        "Dans notre filiere IAPS4 a l'Universite FSSM Marrakech",
        "Ce jeu est conçu pour offrir une experience immersive",
        "Avec des graphismes futuristes et un gameplay dynamique",
        "Le but est de collecter des objets tout en evitant des obstacles",
        "- Collecte de bouteilles pour gagner des points",
        "- Evitez les autres velos sur la route",
        "- Profitez de l'adrenaline d'une course a grande vitesse",
        "- Compteur de score pour suivre vos progres",
        "- Limite de temps pour rendre le defi encore plus excitant",
        "Nous esperons que vous apprecierez ce jeu innovant!",
        "Merci de jouer et bonne chance!"
    }};
}

AproposScreen::~AproposScreen() {
}

void AproposScreen::init(const sf::Font& font) {
    aproposText.setFont(font);
    aproposText.setCharacterSize(28);
    aproposText.setFillColor(sf::Color::White);
    
    aproposShadow.setFont(font);
    aproposShadow.setCharacterSize(28);
    aproposShadow.setFillColor(sf::Color::Black);
    
    returnButton.setFont(font);
    returnButton.setString("RETOUR");
    returnButton.setCharacterSize(32);
    returnButton.setFillColor(sf::Color::White);
    
    returnShadow = returnButton;
    returnShadow.setFillColor(sf::Color::Black);
}

void AproposScreen::update(float dt) {
    // Update for pulsating return button
    static sf::Clock pulseTimer;
    float time = pulseTimer.getElapsedTime().asSeconds();
    int alpha = static_cast<int>(127.5f * (std::sin(time * 2 * 3.1415f) + 1));
    
    returnButton.setFillColor(sf::Color(255, 255, 0, alpha));
    returnShadow.setFillColor(sf::Color(0, 0, 0, alpha));
}

void AproposScreen::draw(sf::RenderWindow& window) {
    // Draw scrolling text
    float scrollY = window.getSize().y + 40.f - scrollClock.getElapsedTime().asSeconds() * 60.f;
    float cx = window.getSize().x / 2.f;
    
    for (size_t i = 0; i < aproposTexts[currentTextIndex].size(); ++i) {
        aproposText.setString(aproposTexts[currentTextIndex][i]);
        float px = cx - aproposText.getGlobalBounds().width / 2.f;
        float py = scrollY + i * 40.f;
        
        aproposShadow.setString(aproposTexts[currentTextIndex][i]);
        aproposShadow.setPosition(px + 2, py + 2);
        aproposText.setPosition(px, py);
        
        if (py > -50 && py < window.getSize().y - 80) {
            window.draw(aproposShadow);
            window.draw(aproposText);
        }
    }
    
    // If text scrolled past threshold, restart from beginning
    if (scrollY + aproposTexts[currentTextIndex].size() * 40.f < -100.f) {
        currentTextIndex = (currentTextIndex + 1) % aproposTexts.size();
        scrollClock.restart();
    }
    
    // Draw return button
    sf::FloatRect rb = returnButton.getLocalBounds();
    float rx = window.getSize().x / 2.f - (rb.width / 2.f + rb.left);
    float ry = window.getSize().y - 60.f;
    
    returnShadow.setPosition(rx + 2, ry + 2);
    returnButton.setPosition(rx, ry);
    
    window.draw(returnShadow);
    window.draw(returnButton);
}

void AproposScreen::reset() {
    currentTextIndex = 0;
    scrollClock.restart();
}
