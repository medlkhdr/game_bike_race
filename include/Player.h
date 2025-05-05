#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    ~Player();
    
    void init(const sf::Texture& texture);
    void draw(sf::RenderWindow& window);
    
    // Updated signatures to match how they're being called
    void resetPosition(float roadWidth, int playerLane, 
                     float padLeft, float padRight, int lanes, const sf::RenderWindow& window);
    void moveLane(int newLane, float roadWidth, 
                float padLeft, float padRight, int lanes, const sf::RenderWindow& window);
    
    void update(float dt);
    
    // Getters
    sf::Sprite& getSprite() { return sprite; }
    const sf::Sprite& getSprite() const { return sprite; }  // Added const version
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    int getLane() const { return lane; }
    float getStamina() const { return stamina; }
    float getSpeed() const { return worldSpeed; }
    
    // Setters
    void setLane(int newLane) { lane = newLane; }
    void addStamina(float amount);
    void drainStamina(float amount);
    void setColor(const sf::Color& color) { sprite.setColor(color); }
    
    // Speed control
    void boost(float dt);
    void brake();
    void normalSpeed(float dt);
    
private:
    sf::Sprite sprite;
    sf::Sprite shadow;
    int lane;
    
    // Stamina properties
    float stamina;
    const float MAX_STAMINA = 5.0f;
    const float STAMINA_DRAIN = 3.0f;
    const float STAMINA_REGEN = 0.5f;
    const float MIN_STAMINA_TO_BOOST = 0.5f;
    
    // Speed properties
    float worldSpeed;
    const float DEFAULT_SPEED = 4.0f;
    const float MAX_SPEED = 12.0f;
    const float ACCEL = 0.2f;
    const float BRAKE_FORCE = 0.5f;
    
    bool triedWhileExhausted;
};

#endif // PLAYER_H






