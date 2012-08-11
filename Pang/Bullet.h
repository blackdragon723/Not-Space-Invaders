/**
 * @file Bullet.h
 *
 * Bullet class used by player and enemy
 *
 */

#ifndef _BULLET_
#define _BULLET_

#include "header.h"

class Bullet
{
private:
	sf::Sprite bulletSprite;
	static unsigned int damage;
	float speed;
	sf::Vector2f position;
public:
	//Default constructor, takes two params
	//x,y is the spawn point of the bullet
	Bullet(float x, float y);

	//Default destructor
	~Bullet();

	//Update the bullet each frame (speed, etc)
	void update();

	//Renders the bullet to the target render window
	void render(sf::RenderWindow& rw);
    
	// Inline getter functions
	sf::Sprite getSprite() const { return bulletSprite; }
	static unsigned int getDamage() { return damage; }
};

#endif