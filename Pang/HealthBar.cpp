/**
 * @file HealthBar.h
 *
 * Implementation of the health bar class
 *
 */

#include "header.h"
#include "HealthBar.h"

HealthBar::HealthBar(float width, float height)
	:width(width), height(height)
{
	barBg.setSize(sf::Vector2f(width, height));
	barBg.setFillColor(sf::Color(sf::Color::White));
	bar.setSize(sf::Vector2f(width, height));
	bar.setFillColor(sf::Color::Red);
}

HealthBar::HealthBar(float width, float height, float x, float y)
    :width(width), height(height)
{
	barBg.setSize(sf::Vector2f(width, height));
	barBg.setFillColor(sf::Color(sf::Color::White));
	barBg.setPosition(x, y);
	bar.setSize(sf::Vector2f(width, height));
	bar.setFillColor(sf::Color::Red);
	bar.setPosition(x, y);
}

void HealthBar::attach(sf::Sprite* object)
{
	barBg.setOrigin(width / 2, height / 2);
	bar.setOrigin(width / 2, height / 2);

	// Set the position of the bar in the middle and 10px above the obj
	barBg.setPosition(object->getPosition().x, object->getPosition().y - object->getLocalBounds().height / 2 - 10);
	bar.setPosition(barBg.getPosition());
}

void HealthBar::update(sf::Sprite* object, float max, float current)
{
	barBg.setPosition(object->getPosition().x, object->getPosition().y - object->getLocalBounds().height / 2 - 10);
	bar.setPosition(barBg.getPosition());
	float percent = current / max;
	bar.setSize(sf::Vector2f(width * percent, height));
}

void HealthBar::render(sf::RenderWindow& rw)
{
	rw.draw(barBg);
	rw.draw(bar);
}