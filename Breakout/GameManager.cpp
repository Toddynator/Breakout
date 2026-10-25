#include "GameManager.h"
#include "Ball.h"
#include "PowerupManager.h"
#include <iostream>

GameManager::GameManager(sf::RenderWindow* window)
    : _window(window), _paddle(nullptr), _ball(nullptr), _brickManager(nullptr), _powerupManager(nullptr),
    _messagingSystem(nullptr), _ui(nullptr), _pause(false), _time(0.f), _lives(3), _pauseHold(0.f), _levelComplete(false),
    _powerupInEffect({ none,0.f }), _timeLastPowerupSpawned(0.f), _levelCurrentWaitTimeSeconds(0.f), _levelRandomNumberGenerator(_levelRandomNumberGeneratorSeed)
{
    _font.loadFromFile("font/montS.ttf");
    _masterText.setFont(_font);
    _masterText.setPosition(50, 400);
    _masterText.setCharacterSize(48);
    _masterText.setFillColor(sf::Color::Yellow);
}

void GameManager::initialize()
{
    _paddle = new Paddle(_window);
    _brickManager = new BrickManager(_window, this);
    _messagingSystem = new MessagingSystem(_window);
    _ball = new Ball(_window, 400.0f, this); 
    _powerupManager = new PowerupManager(_window, _paddle, _ball);
    _ui = new UI(_window, _lives, this);

    // Create bricks, can use a fixed generation for the first level but procedurally generate afterwards or use a set of predefined levels.
    _brickManager->createBricks(5, 10, 80.0f, 30.0f, 5.0f);
    //_brickManager->createBricks(3 + _levelRandomNumberGenerator() % 3, 5 + +_levelRandomNumberGenerator() % 5, 80.0f, 30.0f, 5.0f);
}

void GameManager::update(float dt)
{
    _powerupInEffect = _powerupManager->getPowerupInEffect();
    _ui->updatePowerupText(_powerupInEffect, _powerupManager->getStartTimeOfCurrentPowerup());
    _powerupInEffect.second -= dt;
    

    if (_lives <= 0)
    {
        _masterText.setString("Game over.");
        return;
    }
    if (_levelComplete)
    {
        _masterText.setString("Level completed.");

        _levelCurrentWaitTimeSeconds += dt;
        // Wait for a small amount of time between levels as a small break for the player
        const float levelWaitTimeSeconds = 3.0f;
        if (_levelCurrentWaitTimeSeconds > levelWaitTimeSeconds)
        {
            // Create new level
            // TODO: Add variation / some procedural generation
            //_brickManager->createBricks(5, 10, 80.0f, 30.0f, 5.0f);
            _brickManager->createBricks(3 + _levelRandomNumberGenerator() % 3, 7 + +_levelRandomNumberGenerator() % 3, 80.0f, 30.0f, 5.0f);

            _levelComplete = false;
            _masterText.setString("");
        }
        return;
    }
    // pause and pause handling
    if (_pauseHold > 0.f) _pauseHold -= dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        if (!_pause && _pauseHold <= 0.f)
        {
            _pause = true;
            _masterText.setString("paused.");
            _pauseHold = PAUSE_TIME_BUFFER;
        }
        if (_pause && _pauseHold <= 0.f)
        {
            _pause = false;
            _masterText.setString("");
            _pauseHold = PAUSE_TIME_BUFFER;
        }
    }
    if (_pause)
    {
        return;
    }

    // timer.
    _time += dt;


    if (_time > _timeLastPowerupSpawned + POWERUP_FREQUENCY && rand()%700 == 0)      // TODO parameterise
    {
        _powerupManager->spawnPowerup();
        _timeLastPowerupSpawned = _time;
    }

    // move paddle
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) _paddle->moveRight(dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) _paddle->moveLeft(dt);
    // Mouse Input, only move paddle towards mouse when holding LMB so that keyboard A & D controls are still useable.
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mouseDesktopPosition = sf::Mouse::getPosition();
        sf::Vector2i mouseScreenPosition;
        // Since I can't seem to access WindowBase from RenderWindow to then get a position in window coords, I have to constract the mouse screen position myself from its desktop position and window position + size.
        mouseScreenPosition = sf::Vector2i{ mouseDesktopPosition.x - _window->getPosition().x, mouseDesktopPosition.y - _window->getPosition().y};

        /*std::cout << "\nMouseDesktopX: " << mouseDesktopPosition.x
            << " MouseScreenPosX: " << mouseScreenPosition.x;*/
           
        if (_paddle->getSpritePosition().x + (_paddle->getSpriteSize().x * 0.5f) > mouseScreenPosition.x) {
            _paddle->moveLeft(dt, mouseScreenPosition.x - (_paddle->getSpriteSize().x * 0.5f));
        }
        else if (_paddle->getSpritePosition().x + (_paddle->getSpriteSize().x * 0.5f) < mouseScreenPosition.x) {
            _paddle->moveRight(dt, mouseScreenPosition.x);
        }
    }

    // update everything 
    _paddle->update(dt);
    _ball->update(dt);
    _powerupManager->update(dt);
    _ui->updateScoreText(); // Update only after collisions have been computed
}

void GameManager::loseLife()
{
    _lives--;
    _ui->lifeLost(_lives);

    // TODO screen shake.
}

void GameManager::render()
{
    _paddle->render();
    _ball->render();
    _brickManager->render();
    _powerupManager->render();
    _window->draw(_masterText);
    _ui->render();
}

void GameManager::levelComplete()
{
    _levelComplete = true;
}

sf::RenderWindow* GameManager::getWindow() const { return _window; }
UI* GameManager::getUI() const { return _ui; }
void GameManager::incrementScore(int scoreToAdd)
{
    _score += scoreToAdd;
}
int GameManager::getScore()
{
    return _score;
}
Paddle* GameManager::getPaddle() const { return _paddle; }
BrickManager* GameManager::getBrickManager() const { return _brickManager; }
PowerupManager* GameManager::getPowerupManager() const { return _powerupManager; }
