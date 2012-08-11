/**
 * @file Bullet.h
 *
 * Implementation of the Bullet class
 *
 */
#include "header.h"
#include "Bullet.h"

//Static var declaration
unsigned int Bullet::damage = 20;
static sf::Texture tex;

Bullet::Bullet(float x, float y)
{
	speed = 10.f;
	position.x = x;
	position.y = y;
	tex.loadFromFile(SPRITEPATH);
	bulletSprite.setTexture(tex);
	bulletSprite.setTextureRect(sf::IntRect(380, 58, 8, 21));
	bulletSprite.setOrigin(bulletSprite.getLocalBounds().width / 2, bulletSprite.getLocalBounds().height / 2);
}

Bullet::~Bullet()
{
}

void Bullet::update()
{
	position.y -= speed;
	bulletSprite.setPosition(position);
}

void Bullet::render(sf::RenderWindow& rw)
{
	rw.draw(bulletSprite);
}

