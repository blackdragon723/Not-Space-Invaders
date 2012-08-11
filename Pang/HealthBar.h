/**
 * @file HealthBar.h
 *
 * Class to display object's health graphically
 *
 */
#ifndef _HEALTHBAR_
#define _HEALTHBAR_

#include "header.h"

class HealthBar
{
private:
	sf::RectangleShape barBg;
	sf::RectangleShape bar;
	float width;
	float height;
public:
	// Constructor to create healthbar
	// Takes two params: width and height of bar
	// Used to create a healthbar relative to object
	// Call HealthBar::attach to attach to object
	HealthBar(float width, float height);

	// Constructor to create healthbar
	// Takes four params, width, height, x and y
	// Creates a bar at a fixed position
	HealthBar(float width, float height, float x, float y);

	// Make the healthbar dynamic (relative) to a specific object
	// Takes one paramater: the object that the bar is being attached to
	void attach(sf::Sprite* object);

	// Called to update a bar that is fixed
	// two params: max health and current health
	void update(float max, float current);

	// Called to update the bar relative to an object
	// Three params: the attached object, max health and current health
	void update(sf::Sprite* object, float max, float current);

	// Renders the bar to the target renderwindow
	void render(sf::RenderWindow& rw);
};
#endif