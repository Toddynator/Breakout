#include "Paddle.h"
#include <iostream>

Paddle::Paddle(sf::RenderWindow* window)
    : _window(window), _width(PADDLE_WIDTH), _timeInNewSize(0.0f), _isAlive(true)
{
    _sprite.setFillColor(sf::Color::Cyan);
    _sprite.setPosition((window->getSize().x - _width) / 2.0f, window->getSize().y - 50.0f);
    _sprite.setSize(sf::Vector2f(_width, PADDLE_HEIGHT));
}

Paddle::~Paddle()
{
}

void Paddle::moveLeft(float dt)
{
    float position = _sprite.getPosition().x;

    //if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && position > 0)
    if (position > 0)
    {
        _sprite.move(sf::Vector2f(-dt * PADDLE_SPEED, 0));
    }
}

// Overload : Clamps to a target position if it overshoots
void Paddle::moveLeft(float dt, float targetXPosition)
{
    float position = _sprite.getPosition().x;

    if (position > 0 && position > targetXPosition)
    {
        _sprite.move(sf::Vector2f(-dt * PADDLE_SPEED, 0));
        // If paddle overshoots, clamp it to the targetXPos
        if (_sprite.getPosition().x < targetXPosition) {
            _sprite.setPosition(sf::Vector2f(targetXPosition, _sprite.getPosition().y));
        }
    }
}

void Paddle::moveRight(float dt)
{
    float position = _sprite.getPosition().x;

    //if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && position < _window->getSize().x - _width)
    if (position < _window->getSize().x - _width)
    {
        _sprite.move(sf::Vector2f(dt * PADDLE_SPEED, 0));
    }
}

// Overload : Clamps to a target position if it overshoots
void Paddle::moveRight(float dt, float targetXPosition)
{
    float position = _sprite.getPosition().x;

    if (position < _window->getSize().x - _width && position < targetXPosition)
    {
        _sprite.move(sf::Vector2f(dt * PADDLE_SPEED, 0));
        // If paddle overshoots, clamp it to the targetXPos
        if (_sprite.getPosition().x > targetXPosition) {
            _sprite.setPosition(sf::Vector2f(targetXPosition, _sprite.getPosition().y));
        }
    }
}

void Paddle::update(float dt)
{
    if (_timeInNewSize > 0)
    {
        _timeInNewSize -= dt;
    }
    else
    {
        setWidth(1.0f, 0.0f); // Reset to default width after duration
    }
}

void Paddle::render()
{
    _window->draw(_sprite);
}

sf::FloatRect Paddle::getBounds() const
{
    return _sprite.getGlobalBounds();
}

// update width by SF of coeff. 
// ensure centre remains consistent.
void Paddle::setWidth(float coeff, float duration)
{
    _width = coeff * PADDLE_WIDTH;
    _sprite.setSize(sf::Vector2f(_width, _sprite.getSize().y));
    _timeInNewSize = duration;
    float newX = _sprite.getPosition().x + (_width - PADDLE_WIDTH) / 2;
    _sprite.setPosition(newX, _sprite.getPosition().y);
}

const sf::Vector2i Paddle::getSpritePosition()
{
    return sf::Vector2i{ _sprite.getPosition() };
}

const sf::Vector2f Paddle::getSpriteSize()
{
    return sf::Vector2f(_sprite.getSize());
}
