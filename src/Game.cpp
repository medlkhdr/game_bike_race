#include "../include/Game.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

Game::Game() : 
    window(sf::VideoMode(800, 600), "Bike Game", sf::Style::Default),
    gameState(MENU),
    finishLineSpawned(false),
    raceFinished(false),
    finishTriggered(false),
    distanceTraveled(0.f),
    lives(3),
    score(0)
{
    window.setFramerateLimit(60);
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    
    // Initialize game components
    loadAssets();
    
    // UI setup
    staminaLabel.setFont(font);
    staminaLabel.setCharacterSize(24);
    staminaLabel.setFillColor(sf::Color::White);
    staminaLabel.setString("S\nT\nA\nM\nI\nN\nA");
    staminaLabel.setLineSpacing(1.0f);
    
    positionLabel.setFont(font);
    positionLabel.setCharacterSize(24);
    positionLabel.setFillColor(sf::Color::White);
    positionLabel.setString("VOTRE POSITION :");
    
    finishTitle.setFont(font);
    finishTitle.setCharacterSize(64);
    finishTitle.setFillColor(sf::Color::Yellow);
    finishTitle.setString("FELICITATIONS!");
    
    finishScore.setFont(font);
    finishScore.setCharacterSize(32);
    finishScore.setFillColor(sf::Color::White);
    
    returnBtn.setFont(font);
    returnBtn.setCharacterSize(28);
    returnBtn.setFillColor(sf::Color::White);
    returnBtn.setString("RETOUR AU MENU");
}

Game::~Game() {
    // Clean up resources if needed
}

bool Game::loadAssets() {
    // Load font
    if (!font.loadFromFile("resources/fonts/Pixelite.ttf")) {
        std::cerr << "Failed to load font\n";
        return false;
    }
    
    // Load textures
    if (!bgTexture.loadFromFile("resources/images/bgmenu.jpg")) {
        std::cerr << "Failed to load bgmenu.jpg\n";
        return false;
    }
    
    if (!finishLineTexture.loadFromFile("resources/images/finish.png")) {
        std::cerr << "Failed to load finish line texture\n";
        return false;
    }
    
    // Set up background sprite
    bgSprite.setTexture(bgTexture);
    
    // Load audio
    if (bgMusic.openFromFile("resources/audios/bgmenu.ogg")) {
        bgMusic.setLoop(true);
        bgMusic.setVolume(25.f);
        bgMusic.play();
    }
    
    if (!clickBuf.loadFromFile("resources/audios/click.wav") ||
        !crashBuf.loadFromFile("resources/audios/crash.wav") ||
        !drinkBuf.loadFromFile("resources/audios/drink.wav") ||
        !coinBuf.loadFromFile("resources/audios/coin.wav") ||
        !tiredBuf.loadFromFile("resources/audios/tired.wav") ||
        !finishBuf.loadFromFile("resources/audios/finish.wav"))
    {
        std::cerr << "Failed to load one or more sound files\n";
        return false;
    }
    
    // Set up sounds
    clickSound.setBuffer(clickBuf);
    crashSound.setBuffer(crashBuf);
    drinkSound.setBuffer(drinkBuf);
    coinSound.setBuffer(coinBuf);
    finishSound.setBuffer(finishBuf);
    tiredSound.setBuffer(tiredBuf);
    
    // Initialize menu
    menu.init(font);
    
    return true;
}

void Game::resetGame() {
    // Reset game state variables
    lives = 3;
    score = 0;
    finishLineSpawned = false;
    raceFinished = false;
    finishTriggered = false;
    distanceTraveled = 0.f;
    
    // Clear game objects
    trees.clear();
    obstacles.clear();
    bottles.clear();
    coins.clear();
    
    // Reset player
    player.setLane(1);
    player.resetPosition(road.getRoadWidth(), player.getLane(), road.getPadLeft(), 
                        road.getPadRight(), 4, window);
}

void Game::run() {
    while (window.isOpen()) {
        float dt = deltaClock.restart().asSeconds();
        
        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        // Window events
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        // Resize event
        else if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));
            
            // Reposition player and rebuild road
            player.resetPosition(road.getRoadWidth(), player.getLane(), road.getPadLeft(), 
                                road.getPadRight(), 4, window);
            road.buildRoadTiles(window);
        }
        // Keyboard input
        else if (event.type == sf::Event::KeyPressed) {
            if (gameState == MENU) {
                if (event.key.code == sf::Keyboard::Up)
                    menu.moveUp();
                else if (event.key.code == sf::Keyboard::Down)
                    menu.moveDown();
                else if (event.key.code == sf::Keyboard::Enter) {
                    clickSound.play();
                    if (menu.getSelectedItem() == 0) {
                        gameState = LOADING;
                        deltaClock.restart();
                    }
                    else if (menu.getSelectedItem() == 1) {
                        gameState = APROPOS;
                        // Reset apropos state
                    }
                    else if (menu.getSelectedItem() == 2) {
                        window.close();
                    }
                }
            }
            else if (gameState == APROPOS) {
                if (event.key.code == sf::Keyboard::Enter) {
                    clickSound.play();
                    gameState = MENU;
                    menu.setSelectedItem(0);
                }
            }
            else if (gameState == GAME || gameState == HIT) {
                if ((event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) 
                    && player.getLane() > 0) {
                    player.moveLane(player.getLane() - 1, road.getRoadWidth(), road.getPadLeft(), 
                                  road.getPadRight(), 4, window);
                }
                else if ((event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) 
                         && player.getLane() < 3) {
                    player.moveLane(player.getLane() + 1, road.getRoadWidth(), road.getPadLeft(), 
                                  road.getPadRight(), 4, window);
                }
            }
        }
    }
}

void Game::update(float dt) {
    switch (gameState) {
        case LOADING:
            // Loading state logic
            handleLoadingState();
            break;
            
        case MENU:
            // Menu state logic
            handleMenuState();
            break;
            
        case APROPOS:
            // About screen logic
            handleAproposState();
            break;
            
        case GAME:
            // Game state logic
            handleGameState(dt);
            break;
            
        case HIT:
            // Hit state logic
            handleHitState();
            break;
            
        case FINISH:
            // Finish state logic
            handleFinishState();
            break;
    }
}

void Game::render() {
    window.clear();
    
    // Update background scale
    {
        sf::FloatRect bgBounds = bgSprite.getLocalBounds();
        float scaleX = window.getSize().x / bgBounds.width;
        float scaleY = window.getSize().y / bgBounds.height;
        float scale = std::max(scaleX, scaleY);
        bgSprite.setScale(scale, scale);
    }
    
    // Render based on game state
    switch (gameState) {
        case LOADING:
            // Loading screen
            {
                sf::RectangleShape loadingBg({static_cast<float>(window.getSize().x), 
                                           static_cast<float>(window.getSize().y)});
                loadingBg.setFillColor(sf::Color::Black);
                window.draw(loadingBg);
                
                sf::Text loadingText("Chargement en cours...", font, 30);
                loadingText.setFillColor(sf::Color::Yellow);
                loadingText.setPosition(
                    window.getSize().x/2.f - loadingText.getGlobalBounds().width/2.f,
                    window.getSize().y/2.f
                );
                window.draw(loadingText);
            }
            break;
            
        case MENU:
            // Draw background and menu
            window.draw(bgSprite);
            menu.draw(window);
            break;
            
        case APROPOS:
            // Draw background and about screen
            window.draw(bgSprite);
            // Draw about screen
            break;
            
        case GAME:
        case HIT:
            // Game rendering
            road.draw(window);
            
            // Draw trees
            for (auto& tree : trees) {
                window.draw(tree);
            }
            
            // Draw obstacles, player, collectibles
            
            // Draw UI
            drawUI();
            break;
            
        case FINISH:
            // Finish screen
            window.draw(bgSprite);            
            window.draw(finishTitle);
            window.draw(finishScore);
            window.draw(returnBtn);
            break;
    }
    
    window.display();
}

void Game::handleMenuState() {
    // Handle menu animation and updates
}

void Game::handleAproposState() {
    // Handle about screen scrolling and updates
}

void Game::handleLoadingState() {
    // Loading timer logic
    static sf::Clock loadingClock;
    float loadingTime = loadingClock.getElapsedTime().asSeconds();
    
    if (loadingTime > 3.f) {
        resetGame();
        loadingClock.restart();
        gameState = GAME;
    }
}

void Game::handleGameState(float dt) {
    // Check for boosting/braking input
    bool boosting = sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
                   sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
                   sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    
    bool braking = sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
                  sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    
    // Update player stamina and speed
    updateStamina(dt, boosting, braking);
    
    // Update distance traveled
    if (!raceFinished) {
        distanceTraveled += player.getSpeed();
    }
    
    // Update road
    road.update(player.getSpeed());
    
    // Spawn and update finish line
    if (!finishLineSpawned && distanceTraveled >= FINISH_SPAWN_AT) {
        float roadLeft = road.getRoadLeft(window);
        float scale = road.getRoadWidth() / static_cast<float>(finishLineTexture.getSize().x);
        finishLine.setTexture(finishLineTexture);
        finishLine.setScale(scale, scale);
        finishLine.setPosition(roadLeft, -finishLine.getGlobalBounds().height);
        finishLineSpawned = true;
    }
    
    if (finishLineSpawned) {
        finishLine.move(0, player.getSpeed());
        
        // Check for finish line collision
        if (!finishTriggered && player.getBounds().intersects(finishLine.getGlobalBounds())) {
            finishTriggered = true;
            finishTriggerClock.restart();
            finishSound.play();
        }
        
        // After delay, switch to finish state
        if (finishTriggered && finishTriggerClock.getElapsedTime().asSeconds() >= FINISH_DELAY) {
            gameState = FINISH;
            raceFinished = true;
            finishScore.setString("Votre score est " + std::to_string(score));
        }
    }
    
    // Spawn obstacles, trees, collectibles
    spawnObstacle();
    spawnTree();
    
    // Update game objects
    // Update obstacles, collectibles, etc.
}

void Game::handleHitState() {
    static sf::Clock hitClock;
    float hitTime = hitClock.getElapsedTime().asSeconds();
    
    if (hitTime < 2.f) {
        uint8_t alpha = static_cast<uint8_t>(255 * std::abs(std::sin(hitTime * 10.f)));
        player.setColor(sf::Color(255, 255, 255, alpha));
    } else {
        gameState = GAME;
        player.setColor(sf::Color::White);
    }
}

void Game::handleFinishState() {
    // Handle finish state interactions
}

void Game::updateStamina(float dt, bool boosting, bool braking) {
    // This will update player's stamina based on current actions
}

void Game::spawnObstacle() {
    // Spawn obstacles based on random chance
}

void Game::spawnTree() {
    // Spawn trees based on random chance
}

void Game::drawUI() {
    // Draw score and lives
    sf::Text hud;
    hud.setFont(font);
    hud.setCharacterSize(24);
    hud.setFillColor(sf::Color::White);
    hud.setString("Score: " + std::to_string(score) + "  Lives: " + std::to_string(lives));
    hud.setPosition(20.f, 20.f);
    window.draw(hud);
    
    // Draw stamina bar
    const float BAR_W = 20.f, BAR_H = 150.f;
    float barX = window.getSize().x - BAR_W - 20.f;
    float barY = (window.getSize().y - BAR_H) / 2.f;
    
    // Stamina label
    staminaLabel.setPosition(
        barX - staminaLabel.getGlobalBounds().width - 10.f,
        barY - staminaLabel.getCharacterSize()
    );
    window.draw(staminaLabel);
    
    // Stamina bar background
    sf::RectangleShape barBg(sf::Vector2f(BAR_W, BAR_H));
    barBg.setPosition(barX, barY);
    barBg.setFillColor(sf::Color(50, 50, 50, 200));
    window.draw(barBg);
    
    // Stamina bar fill
    float fillH = (player.getStamina() / 5.0f) * BAR_H;
    sf::RectangleShape barFill(sf::Vector2f(BAR_W, fillH));
    barFill.setPosition(barX, barY + (BAR_H - fillH));
    barFill.setFillColor(sf::Color(100, 100, 255, 200));
    window.draw(barFill);
    
    // Draw race progress bar
    const float PB_W = 300.f, PB_H = 15.f;
    float progress = std::min(1.f, distanceTraveled / RACE_DISTANCE);
    float pbX = (window.getSize().x - PB_W) / 2.f;
    float pbY = window.getSize().y - PB_H - 10.f;
    
    // Position label
    positionLabel.setPosition(
        pbX,
        pbY - positionLabel.getCharacterSize() - 5.f
    );
    window.draw(positionLabel);
    
    // Progress bar background
    sf::RectangleShape progressBg(sf::Vector2f(PB_W, PB_H));
    progressBg.setPosition(pbX, pbY);
    progressBg.setFillColor(sf::Color(50, 50, 50, 200));
    window.draw(progressBg);
    
    // Progress bar fill
    sf::RectangleShape progressFill(sf::Vector2f(PB_W * progress, PB_H));
    progressFill.setPosition(pbX, pbY);
    progressFill.setFillColor(sf::Color(100, 255, 100, 220));
    window.draw(progressFill);
}