#ifndef _ENEMY_
#define _ENEMY_

#include "header.h"
#include "globals.h"
#include "HealthBar.h"
#include "Bullet.h"

typedef boost::shared_ptr<Bullet> bullet_ptr;

class Enemy
{
private:
	unsigned int health;
	unsigned int maxHealth;
	float width, height;
	int vel;

	sf::Sprite sprite;
	sf::Clock fireRate;

	HealthBar healthBar;
public:
	Enemy(float x, float y, sf::Texture& tex = global::spriteSheet); //Used for bosses
	Enemy(const std::vector<global::enemy_ptr>& vec, sf::Texture& tex = global::spriteSheet); //Makes sure the enemy doesnt overlap another

	~Enemy();
    
	sf::IntRect getRandomSprite();
	sf::Vector2f getRandomSpawn(const std::vector<global::enemy_ptr>& vec);
	
	void damageEnemy(unsigned int delta);
    bool checkCollision(sf::Vector2f c);
    bool checkCollision(std::list<bullet_ptr>& list);
	
    unsigned int getHealth() { return health; }
	const sf::Sprite& getSprite() { return sprite; }	 
	unsigned int getHealth() const { return health; }

	void update();
    void render(sf::RenderWindow& rw);
};

#endif