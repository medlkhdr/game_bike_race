#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "GameState.h"
#include "Player.h"
#include "Road.h"
#include "Obstacle.h"
#include "Collectible.h"
#include "Menu.h"

class Game {
public:
    Game();
    ~Game();
    
    void run();

private:
    // Window and game settings
    sf::RenderWindow window;
    GameState gameState;
    sf::Clock deltaClock;
    sf::Font font;
    
    // Game entities
    Player player;
    Road road;
    std::vector<Obstacle> obstacles;
    std::vector<Collectible> bottles;
    std::vector<Collectible> coins;
    std::vector<sf::Sprite> trees;
    
    // Finish line
    sf::Sprite finishLine;
    bool finishLineSpawned;
    bool raceFinished;
    bool finishTriggered;
    sf::Clock finishTriggerClock;
    
    // Game progress
    float distanceTraveled;
    int lives;
    int score;
    
    // UI elements
    Menu menu;
    sf::Text staminaLabel;
    sf::Text positionLabel;
    sf::Text finishTitle;
    sf::Text finishScore;
    sf::Text returnBtn;
    
    // Assets
    sf::Texture bgTexture;
    sf::Texture finishLineTexture;
    sf::Sprite bgSprite;
    
    // Audio
    sf::Music bgMusic;
    sf::SoundBuffer clickBuf, crashBuf, drinkBuf, coinBuf, finishBuf, tiredBuf;
    sf::Sound clickSound, crashSound, drinkSound, coinSound, finishSound, tiredSound;
    
    // Game constants
    const float RACE_DISTANCE = 5000.f;
    const float FINISH_SPAWN_AT = 4500.f;
    const float FINISH_DELAY = 2.0f;
    
    // Methods
    void processEvents();
    void update(float dt);
    void render();
    
    void handleMenuState();
    void handleAproposState();
    void handleLoadingState();
    void handleGameState(float dt);
    void handleHitState();
    void handleFinishState();
    
    bool loadAssets();
    void resetGame();
    void spawnObstacle();
    void spawnTree();
    void updateStamina(float dt, bool boosting, bool braking);
    void drawUI();
};

#endif // GAME_H
