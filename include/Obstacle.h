#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SFML/Graphics.hpp>
#include <vector>

class Obstacle {
public:
    Obstacle();
    Obstacle(const sf::Texture& texture);
    ~Obstacle();
    
    void init(const sf::Texture& texture);
    void spawn(float roadLeft, float laneWidth, float padLeft, int lane, int lanes, int windowWidth);
    void update(float speed);
    void draw(sf::RenderWindow& window);
    
    // Getters
    sf::Sprite& getSprite() { return sprite; }
    const sf::Sprite& getSprite() const { return sprite; }  // Added const version
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    bool isOffScreen(int windowHeight) const;
    
private:
    sf::Sprite sprite;
    sf::Sprite shadow;
};

class ObstacleManager {
public:
    ObstacleManager();
    ~ObstacleManager();
    
    void init(const std::vector<sf::Texture>& textures);
    void update(float speed, float roadWidth, float padLeft, float padRight, int lanes, 
               sf::RenderWindow& window, bool braking, float obstacleSpeed);
    void draw(sf::RenderWindow& window);
    
    void spawnObstacle(float roadWidth, float padLeft, float padRight, int lanes, 
                      sf::RenderWindow& window);
    
    std::vector<Obstacle>& getObstacles() { return obstacles; }
    
private:
    std::vector<Obstacle> obstacles;
    std::vector<sf::Texture> textures;
};

#endif // OBSTACLE_H