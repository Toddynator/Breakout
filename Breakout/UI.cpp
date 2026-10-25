#include <sstream>

#include "UI.h"
#include "GameManager.h"
#include <iomanip>

UI::UI(sf::RenderWindow* window, int lives, GameManager* gameManager) 
	: _window(window), _gameManager(gameManager)
{
	for (int i = lives; i > 0; --i)
	{
		sf::CircleShape newLife;
		newLife.setFillColor(sf::Color::Red);	
		newLife.setOutlineColor(sf::Color::Cyan);
		newLife.setOutlineThickness(4.0f);
		newLife.setRadius(LIFE_RADIUS);
		newLife.setPosition((LIFE_RADIUS*2 + LIFE_PADDING) * i, LIFE_PADDING);
		_lives.push_back(newLife);
	}
	_powerupText.setCharacterSize(30);
	_powerupText.setPosition(800, 10);
	_powerupText.setFillColor(sf::Color::Cyan);
	_font.loadFromFile("font/montS.ttf");
	_powerupText.setFont(_font);

	// Position & sizes will be based on the window size so that it scales with different resolutions
	float progressBarBackgroundWidth = window->getSize().x * 0.2f;
	float progressBarBackgroundHeight = window->getSize().x * 0.025f;
	float progressBarWidth = progressBarBackgroundWidth;
	float progressBarHeight = progressBarBackgroundHeight;
	_progressBarBackground.setPosition(_powerupText.getPosition().x - progressBarBackgroundWidth * 1.3f, _powerupText.getPosition().y * 1.7f);
	_progressBar.setPosition(_powerupText.getPosition().x - progressBarBackgroundWidth * 1.3f, _powerupText.getPosition().y * 1.7f);
	_progressBarBackground.setSize(sf::Vector2f(progressBarBackgroundWidth, progressBarBackgroundHeight));
	_progressBar.setSize(sf::Vector2f(progressBarWidth, progressBarHeight));
	_progressBarBackground.setFillColor(sf::Color::Blue);
	_progressBar.setFillColor(sf::Color::Red);
}

UI::~UI()
{
}


void UI::updatePowerupText(std::pair<POWERUPS, float> powerup, float startTimeOfPowerup)
{
	std::ostringstream oss;

	switch (powerup.first)
	{
	case bigPaddle:
		//oss << std::fixed << std::setprecision(2) << powerup.second;
		//_powerupText.setString("big " + oss.str());
		_powerupText.setString("big");
		_powerupText.setFillColor(paddleEffectsColour);
		break;
	case smallPaddle:
		//oss << std::fixed << std::setprecision(2) << powerup.second;
		//_powerupText.setString("small " + oss.str());
		_powerupText.setString("small");
		_powerupText.setFillColor(paddleEffectsColour);
		break;
	case slowBall:
		//oss << std::fixed << std::setprecision(2) << powerup.second;
		//_powerupText.setString("slow " + oss.str());
		_powerupText.setString("slow");
		_powerupText.setFillColor(ballEffectsColour);
		break;
	case fastBall:
		//oss << std::fixed << std::setprecision(2) << powerup.second;
		//_powerupText.setString("fast " + oss.str());
		_powerupText.setString("fast");
		_powerupText.setFillColor(ballEffectsColour);
		break;
	case fireBall:
		//oss << std::fixed << std::setprecision(2) << powerup.second;
		//_powerupText.setString("fire " + oss.str());
		_powerupText.setString("fire");
		_powerupText.setFillColor(extraBallEffectsColour);
		break;
	case none:
		_powerupText.setString("");
		
		break;
	}

	_updatePowerupProgressBar(powerup.second, startTimeOfPowerup);
}

// Update progress bar using normalised value of the powerups cooldown to size it from 0 width to filling the full bar 
void UI::_updatePowerupProgressBar(float powerupTimeLeft, float startTimeOfPowerup)
{
	float progressNormalised = 1.0f;
	if (startTimeOfPowerup != 0.f) {
		progressNormalised = std::clamp(powerupTimeLeft / startTimeOfPowerup, 0.f, 1.f);
	}

	// Progress bar should match the full size of the background bar, so I can use that as the max value to then multiply by the normalised progress.
	_progressBar.setSize(sf::Vector2f(_progressBarBackground.getSize().x * progressNormalised, _progressBar.getSize().y));
}

void UI::lifeLost(int lives)
{
	_lives[_lives.size() - 1 - lives].setFillColor(sf::Color::Transparent);
}

void UI::render()
{
	_window->draw(_powerupText);
	for (sf::CircleShape life : _lives)
	{
		_window->draw(life);
	}
	if (_powerupText.getString() != "") {
		_window->draw(_progressBarBackground);
		_window->draw(_progressBar);
	}
}