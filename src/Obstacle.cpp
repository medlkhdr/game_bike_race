#include "../include/Obstacle.h"
#include <cstdlib>

// Obstacle Class Implementation
Obstacle::Obstacle() {
}

Obstacle::Obstacle(const sf::Texture& texture) {
    init(texture);
}

Obstacle::~Obstacle() {
}

void Obstacle::init(const sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setScale(0.20f, 0.20f);
    
    shadow = sprite;
    shadow.setColor(sf::Color(0, 0, 0, 150));
    shadow.setPosition(5.f, 5.f); // Offset for shadow effect
}

void Obstacle::spawn(float roadLeft, float laneWidth, float padLeft, int lane, int lanes, int windowWidth) {
    float rw = static_cast<float>(windowWidth);
    float cx_lane = roadLeft + padLeft * rw + laneWidth * (lane + 0.5f);
    
    sprite.setPosition(
        cx_lane - sprite.getGlobalBounds().width / 2.f,
        -sprite.getGlobalBounds().height - (std::rand() % 101 + 50)
    );
}

void Obstacle::update(float speed) {
    sprite.move(0, speed);
    
    // Update shadow position
    shadow.setPosition(sprite.getPosition().x + 5.f, sprite.getPosition().y + 5.f);
}

void Obstacle::draw(sf::RenderWindow& window) {
    window.draw(shadow);
    window.draw(sprite);
}

bool Obstacle::isOffScreen(int windowHeight) const {
    return sprite.getPosition().y > windowHeight;
}

// ObstacleManager Class Implementation
ObstacleManager::ObstacleManager() {
}

ObstacleManager::~ObstacleManager() {
}

void ObstacleManager::init(const std::vector<sf::Texture>& textures) {
    this->textures = textures;
}

void ObstacleManager::update(float speed, float roadWidth, float padLeft, float padRight, int lanes, 
                           sf::RenderWindow& window, bool braking, float obstacleSpeed) {
    float actualSpeed = braking ? -obstacleSpeed : obstacleSpeed;
    
    // Spawn obstacles based on random chance
    spawnObstacle(roadWidth, padLeft, padRight, lanes, window);
    
    // Update and check collisions for existing obstacles
    for (auto it = obstacles.begin(); it != obstacles.end(); ) {
        it->update(actualSpeed);
        
        // If obstacle is off screen, remove it
        if (it->isOffScreen(window.getSize().y)) {
            it = obstacles.erase(it);
        } else {
            ++it;
        }
    }
}

void ObstacleManager::draw(sf::RenderWindow& window) {
    for (auto& obstacle : obstacles) {
        obstacle.draw(window);
    }
}

void ObstacleManager::spawnObstacle(float roadWidth, float padLeft, float padRight, int lanes, 
                                  sf::RenderWindow& window) {
    // Only spawn if chance is met and there's enough space from the previous obstacle
    if (std::rand() % 100 < 10 && (obstacles.empty() || 
        obstacles.back().getSprite().getPosition().y > 150.f)) {
        
        int lane = std::rand() % lanes;
        int textureIndex = std::rand() % textures.size();
        
        Obstacle obstacle(textures[textureIndex]);
        
        float roadLeft = (window.getSize().x - roadWidth) / 2.f;
        float laneWidth = (roadWidth - (padLeft + padRight) * roadWidth) / lanes;
        
        obstacle.spawn(roadLeft, laneWidth, padLeft, lane, lanes, roadWidth);
        obstacles.push_back(obstacle);
    }
}
