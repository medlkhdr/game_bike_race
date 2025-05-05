#include "../include/Player.h"
#include <algorithm>

Player::Player() : 
    lane(1),
    stamina(MAX_STAMINA),
    worldSpeed(DEFAULT_SPEED),
    triedWhileExhausted(false)
{
}

Player::~Player() {
}

void Player::init(const sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setScale(0.25f, 0.25f);
    
    shadow = sprite;
    shadow.setScale(0.20f, 0.20f);
    shadow.setColor(sf::Color(0, 0, 0, 150));
}

// Updated to match new signature
void Player::resetPosition(float roadWidth, int playerLane,
                         float padLeft, float padRight, int lanes, const sf::RenderWindow& window)
{
    float rw = roadWidth;
    float roadLeft = (window.getSize().x - rw) / 2.f;
    float leftPad = padLeft * rw;
    float inner = rw - (padLeft + padRight) * rw;
    float laneW = inner / lanes;
    float pw = sprite.getGlobalBounds().width;
    float playerTargetX = roadLeft + leftPad + laneW * (playerLane + 0.5f) - pw / 2.f;
    float py = window.getSize().y - sprite.getGlobalBounds().height - 10.f;
    sprite.setPosition(playerTargetX, py);
}

// Updated to match new signature
void Player::moveLane(int newLane, float roadWidth,
                    float padLeft, float padRight, int lanes, const sf::RenderWindow& window)
{
    lane = newLane;
    resetPosition(roadWidth, lane, padLeft, padRight, lanes, window);
}

void Player::update(float dt) {
    // Update player animation if needed
    
    // Update shadow position
    shadow.setPosition(sprite.getPosition().x + 5.f, sprite.getPosition().y + 5.f);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shadow);
    window.draw(sprite);
}

void Player::addStamina(float amount) {
    stamina = std::min(MAX_STAMINA, stamina + amount);
}

void Player::drainStamina(float amount) {
    stamina = std::max(0.f, stamina - amount);
}

void Player::boost(float dt) {
    if (stamina >= MIN_STAMINA_TO_BOOST) {
        // Allowed to boost
        worldSpeed = std::min(worldSpeed + ACCEL, MAX_SPEED);
        drainStamina(STAMINA_DRAIN * dt);
        triedWhileExhausted = false;
    }
}

void Player::brake() {
    worldSpeed = 0.f;
}

void Player::normalSpeed(float dt) {
    if (worldSpeed < DEFAULT_SPEED)
        worldSpeed = std::min(worldSpeed + ACCEL, DEFAULT_SPEED);
    else if (worldSpeed > DEFAULT_SPEED)
        worldSpeed = std::max(worldSpeed - BRAKE_FORCE, DEFAULT_SPEED);
        
    // Regenerate stamina when not boosting
    addStamina(STAMINA_REGEN * dt);
}




