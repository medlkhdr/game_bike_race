#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Obstacle.h"

class Collectible {
public:
    enum Type { BOTTLE, COIN };
    
    Collectible();
    Collectible(Type type, const sf::Texture& texture);
    ~Collectible();
    
    void init(Type type, const sf::Texture& texture);
    void update(float speed);
    void draw(sf::RenderWindow& window);
    
    // Getters
    sf::Sprite& getSprite() { return sprite; }
    const sf::Sprite& getSprite() const { return sprite; }  // Added const version
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    Type getType() const { return type; }
    bool isOffScreen(int windowHeight) const;
    
private:
    sf::Sprite sprite;
    Type type;
};

class CollectibleManager {
public:
    CollectibleManager();
    ~CollectibleManager();
    
    void init(const sf::Texture& bottleTexture, const sf::Texture& coinTexture, 
             sf::Sound& drinkSound, sf::Sound& coinSound);
    
    void update(float speed, float roadWidth, float padLeft, float padRight, int lanes, 
               sf::RenderWindow& window, sf::Sprite& player, float& stamina, 
               float maxStamina, float bottleStamina, int& score);
    
    void draw(sf::RenderWindow& window);
    
    void spawnBottle(float roadWidth, float padLeft, float padRight, int lanes, 
                    sf::RenderWindow& window, const std::vector<Obstacle>& obstacles);
    
    void spawnCoin(float roadWidth, float padLeft, float padRight, int lanes, 
                  sf::RenderWindow& window, const std::vector<Obstacle>& obstacles);
    
    std::vector<Collectible>& getBottles() { return bottles; }
    std::vector<Collectible>& getCoins() { return coins; }
    
private:
    std::vector<Collectible> bottles;
    std::vector<Collectible> coins;
    sf::Texture bottleTexture;
    sf::Texture coinTexture;
    sf::Sound* drinkSound;
    sf::Sound* coinSound;
};

#endif // COLLECTIBLE_H