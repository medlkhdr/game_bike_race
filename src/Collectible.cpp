#include "../include/Collectible.h"
#include <cstdlib>

// Collectible Class Implementation
Collectible::Collectible() : type(BOTTLE) {
}

Collectible::Collectible(Type type, const sf::Texture& texture) : type(type) {
    init(type, texture);
}

Collectible::~Collectible() {
}

void Collectible::init(Type type, const sf::Texture& texture) {
    this->type = type;
    sprite.setTexture(texture);
    
    // Set scale based on type
    if (type == BOTTLE) {
        sprite.setScale(0.23f, 0.23f);
    } else if (type == COIN) {
        sprite.setScale(0.16f, 0.16f);
    }
}

void Collectible::update(float speed) {
    sprite.move(0, speed);
}

void Collectible::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool Collectible::isOffScreen(int windowHeight) const {
    return sprite.getPosition().y > windowHeight;
}

// CollectibleManager Class Implementation
CollectibleManager::CollectibleManager() : drinkSound(nullptr), coinSound(nullptr) {
}

CollectibleManager::~CollectibleManager() {
}

void CollectibleManager::init(const sf::Texture& bottleTex, const sf::Texture& coinTex, 
                            sf::Sound& drinkSound, sf::Sound& coinSound) {
    this->bottleTexture = bottleTex;
    this->coinTexture = coinTex;
    this->drinkSound = &drinkSound;
    this->coinSound = &coinSound;
}

void CollectibleManager::update(float speed, float roadWidth, float padLeft, float padRight, int lanes, 
                             sf::RenderWindow& window, sf::Sprite& player, float& stamina, 
                             float maxStamina, float bottleStamina, int& score) {
    // Spawn new collectibles
    spawnBottle(roadWidth, padLeft, padRight, lanes, window, {});
    spawnCoin(roadWidth, padLeft, padRight, lanes, window, {});
    
    // Update bottles
    for (auto it = bottles.begin(); it != bottles.end(); ) {
        it->update(speed);
        
        if (player.getGlobalBounds().intersects(it->getBounds())) {
            drinkSound->play();
            stamina = std::min(maxStamina, stamina + bottleStamina);
            it = bottles.erase(it);
        } else if (it->isOffScreen(window.getSize().y)) {
            it = bottles.erase(it);
        } else {
            ++it;
        }
    }
    
    // Update coins
    for (auto it = coins.begin(); it != coins.end(); ) {
        it->update(speed);
        
        if (player.getGlobalBounds().intersects(it->getBounds())) {
            coinSound->play();
            score += 100;
            it = coins.erase(it);
        } else if (it->isOffScreen(window.getSize().y)) {
            it = coins.erase(it);
        } else {
            ++it;
        }
    }
}

void CollectibleManager::draw(sf::RenderWindow& window) {
    // Draw bottles
    for (auto& bottle : bottles) {
        bottle.draw(window);
    }
    
    // Draw coins
    for (auto& coin : coins) {
        coin.draw(window);
    }
}

void CollectibleManager::spawnBottle(float roadWidth, float padLeft, float padRight, int lanes, 
                                    sf::RenderWindow& window, const std::vector<Obstacle>& obstacles) {
    if (std::rand() % 1000 < 5) {
        int lane = std::rand() % lanes;
        Collectible bottle(Collectible::BOTTLE, bottleTexture);
        
        float rw = roadWidth;
        float roadL = (window.getSize().x - rw) / 2.f;
        float lp = padLeft * rw;
        float inner = rw - (padLeft + padRight) * rw;
        float laneW = inner / lanes;
        float bx = roadL + lp + laneW * (lane + 0.5f) - bottle.getBounds().width / 2.f;
        float by = -bottle.getBounds().height - (std::rand() % 100);
        
        bottle.getSprite().setPosition(bx, by);
        
        // Check for overlap with existing bottles
        for (const auto &existingBottle : bottles) {
            if (std::abs(existingBottle.getSprite().getPosition().y - bottle.getSprite().getPosition().y) < 100.f)
                return;
        }
        
        // Check for overlap with obstacles
        for (const auto &obs : obstacles) {
            if (bottle.getBounds().intersects(obs.getBounds()))
                return;
        }
        
        // Check for overlap with coins
        for (const auto &coin : coins) {
            if (bottle.getBounds().intersects(coin.getBounds()))
                return;
        }
        
        bottles.push_back(bottle);
    }
}

void CollectibleManager::spawnCoin(float roadWidth, float padLeft, float padRight, int lanes, 
                                  sf::RenderWindow& window, const std::vector<Obstacle>& obstacles) {
    if (std::rand() % 1000 < 4) {
        int lane = std::rand() % lanes;
        Collectible coin(Collectible::COIN, coinTexture);
        
        float rw = roadWidth;
        float roadL = (window.getSize().x - rw) / 2.f;
        float lp = padLeft * rw;
        float inner = rw - (padLeft + padRight) * rw;
        float laneW = inner / lanes;
        float cx = roadL + lp + laneW * (lane + 0.5f) - coin.getBounds().width / 2.f;
        float cy = -coin.getBounds().height - (std::rand() % 150);
        
        coin.getSprite().setPosition(cx, cy);
        
        // Check for overlap with existing coins
        for (const auto &existingCoin : coins) {
            if (std::abs(existingCoin.getSprite().getPosition().y - coin.getSprite().getPosition().y) < 100.f)
                return;
        }
        
        // Check for overlap with obstacles
        for (const auto &obs : obstacles) {
            if (coin.getBounds().intersects(obs.getBounds()))
                return;
        }
        
        // Check for overlap with bottles
        for (const auto &bottle : bottles) {
            if (coin.getBounds().intersects(bottle.getBounds()))
                return;
        }
        
        coins.push_back(coin);
    }
}





