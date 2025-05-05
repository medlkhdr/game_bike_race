#include "../include/Road.h"
#include <cmath>

Road::Road() : 
    grassOffset(0.f),
    tileHeight(0.f),
    roadTileCount(0)
{
}

Road::~Road() {
}

void Road::init(const sf::Texture& roadTex, const sf::Texture& grassTex) {
    roadTexture = roadTex;
    grassTexture = grassTex;
    
    // Make sure the grass texture is repeatable
    grassTexture.setRepeated(true);
    
    // Compute tile height
    tileHeight = static_cast<float>(roadTexture.getSize().y);
}

void Road::buildRoadTiles(sf::RenderWindow& window) {
    // Compute strip height
    float winH = static_cast<float>(window.getSize().y);
    roadTileCount = static_cast<int>(std::ceil(winH / tileHeight)) + 1;
    float startY = winH - roadTileCount * tileHeight;
    
    roadTiles.clear();
    for (int i = 0; i < roadTileCount; ++i) {
        sf::Sprite tile(roadTexture);
        tile.setPosition(0.f, startY + i * tileHeight);
        roadTiles.push_back(tile);
    }
}

void Road::update(float speed) {
    // Update grass offset for scrolling effect
    grassOffset -= speed;
    if (grassOffset < 0.f)
        grassOffset += static_cast<float>(grassTexture.getSize().y);
    
    // Update road tiles
    for (auto& tile : roadTiles) {
        // Scroll
        tile.move(0, speed);
        
        // If it moves off bottom, jump it back up by the total stacked height
        if (tile.getPosition().y >= tileHeight * roadTileCount) {
            tile.setPosition(
                tile.getPosition().x,
                tile.getPosition().y - roadTileCount * tileHeight
            );
        }
    }
}

void Road::draw(sf::RenderWindow& window) {
    // Draw grass margins
    float rw = getRoadWidth();
    float roadLeft = getRoadLeft(window);
    int winH = static_cast<int>(window.getSize().y);
    int iRoadLeft = static_cast<int>(roadLeft);
    
    sf::RectangleShape gL({ roadLeft, static_cast<float>(winH) });
    sf::RectangleShape gR({ roadLeft, static_cast<float>(winH) });
    gL.setPosition(0, 0);
    gR.setPosition(roadLeft + rw, 0);
    gL.setTexture(&grassTexture);
    gR.setTexture(&grassTexture);
    gL.setTextureRect({ 0, static_cast<int>(grassOffset), iRoadLeft, winH });
    gR.setTextureRect({ 0, static_cast<int>(grassOffset), iRoadLeft, winH });
    window.draw(gL);
    window.draw(gR);
    
    // Draw road tiles
    for (auto& tile : roadTiles) {
        // Re-center X position
        float roadLeft = getRoadLeft(window);
        tile.setPosition(roadLeft, tile.getPosition().y);
        window.draw(tile);
    }
}

float Road::getRoadLeft(const sf::RenderWindow& window) const {
    return (window.getSize().x - getRoadWidth()) / 2.f;
}

float Road::getLaneWidth(int lanes) const {
    float inner = getRoadWidth() - (padLeft + padRight) * getRoadWidth();
    return inner / lanes;
}
