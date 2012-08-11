#include "header.h"
#include "Enemy.h"

Enemy::Enemy(float x, float y, sf::Texture& tex) //Used for bosses
    :healthBar(75, 10)
{
	sprite.setTexture(tex);
	sprite.setTextureRect(getRandomSprite());
	width = sprite.getLocalBounds().width;
	height = sprite.getLocalBounds().height;
	sprite.setOrigin(width / 2, height / 2);
	sprite.setPosition(x, y);
	maxHealth = health = 100;
	healthBar.attach(&sprite);
	vel = 1;
}

Enemy::Enemy(const std::vector<global::enemy_ptr>& vec, sf::Texture& tex) //Makes sure the enemy doesnt overlap another
    :healthBar(75, 10)
{
	sprite.setTexture(tex);
	sprite.setTextureRect(getRandomSprite());
	width = sprite.getLocalBounds().width;
	height = sprite.getLocalBounds().height;
	sprite.setOrigin(width / 2, height / 2);
	sprite.setPosition(getRandomSpawn(vec));
	maxHealth = health = 100;
	healthBar.attach(&sprite);
	vel = 1;
}

Enemy::~Enemy()
{
}

sf::IntRect Enemy::getRandomSprite()
{
	boost::random::uniform_int_distribution<> dist(1, 2);
	int genNum = dist(global::rngEng);
	switch (genNum)
	{
		case 1: return sf::IntRect(380, 0, 40, 57);
        case 2: return sf::IntRect(421, 0, 40, 57);
        default: return sf::IntRect(0, 0, 0, 0);
	}
}

sf::Vector2f Enemy::getRandomSpawn(const std::vector<global::enemy_ptr>& vec)
{
	boost::random::uniform_int_distribution<> dist(20, 1260);
	int genPosX = 0;
	//Keep generating a new x position until it doesn't insersect any other enemy
	for(;;)
	{
		genPosX = dist(global::rngEng);
		sf::FloatRect genRect((float)genPosX, -30.f, 40.f, 57.f);
		for (std::vector<global::enemy_ptr>::const_iterator iter = vec.begin(); iter != vec.end(); iter++)
		{
			if ((*iter)->getSprite().getGlobalBounds().intersects(genRect))
			{
				continue;
			}
		}
		return sf::Vector2f((float)genPosX, -10.f);
	}
}

bool Enemy::checkCollision(sf::Vector2f c)
{
	if (sprite.getGlobalBounds().contains(c)) return true;
	else return false;
}

bool Enemy::checkCollision(std::list<bullet_ptr>& list)
{
	for (std::list<bullet_ptr>::iterator iter = list.begin(); iter != list.end();)
	{
		//If the sprite collided AND it's inside the screen
		if (sprite.getGlobalBounds().intersects((*iter)->getSprite().getGlobalBounds()) && sprite.getPosition().y > 0)
		{
			list.erase(iter);
			return true;
		}
		else ++iter;
	}
	return false;
}

void Enemy::damageEnemy(unsigned int delta)
{ 
	health -= delta; 
}

void Enemy::update()
{
	this->sprite.move(0, (float)vel);
	healthBar.update(&this->sprite, (float)maxHealth, (float)health);
}

void Enemy::render(sf::RenderWindow& rw)
{
	rw.draw(sprite);
	healthBar.render(rw);
}