#ifndef ROAD_H
#define ROAD_H

#include <SFML/Graphics.hpp>
#include <vector>

class Road {
public:
    Road();
    ~Road();
    
    void init(const sf::Texture& roadTexture, const sf::Texture& grassTexture);
    void buildRoadTiles(sf::RenderWindow& window);
    void update(float speed);
    void draw(sf::RenderWindow& window);
    
    // Getters
    float getRoadWidth() const { return roadTexture.getSize().x; }
    float getTileHeight() const { return tileHeight; }
    float getRoadLeft(const sf::RenderWindow& window) const;
    float getLaneWidth(int lanes) const;
    float getPadLeft() const { return padLeft; }
    float getPadRight() const { return padRight; }
    
private:
    sf::Texture roadTexture;
    sf::Texture grassTexture;
    std::vector<sf::Sprite> roadTiles;
    
    float grassOffset;
    float tileHeight;
    int roadTileCount;
    
    // Road geometry
    const float padLeft = 0.15f;
    const float padRight = 0.15f;
    const int LANES = 4;
};

#endif // ROAD_H
