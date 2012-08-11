#ifndef _PLAYER_
#define _PLAYER_

#include "header.h"
#include "Bullet.h"
#include "HealthBar.h"

enum DIRECTION { UP, DOWN, LEFT, RIGHT, STATIONARY, TRANSITION_TO_STATIONARY };
typedef boost::shared_ptr<Bullet> bullet_ptr;

class Player
{
private:
	/*
     SPRITES:
     0: default
     1: left_a
     2: left_b
     3: right_a
     4: right_b
     */
	sf::Sprite playerShip[8];
    
	bool isAlive;
	unsigned int lives;
	unsigned int hp;
	unsigned int missiles;
    
	unsigned int frame;
	sf::Vector2f position;
	const float speed;
    
	DIRECTION playerDirection;
    
	std::list<bullet_ptr> playersBullets;
    
	//Clock to regulate the firing of bullets
	sf::Clock bulletClock;
    
	HealthBar playerHealthBar;
public:
	Player(sf::Texture& tex = global::spriteSheet);
	~Player();
    
	void kill();
    
	void fireBullet();
	std::list<bullet_ptr>& getBulletList() { return playersBullets; }
    
	bool checkCollision(sf::Vector2f c);
	void move(float x, float y);

    void update(); 
	void render(sf::RenderWindow& rw);

	//Getters and setters
	unsigned int getHp() const { return hp; }
	unsigned int getLives() const { return lives; }
	unsigned int getMissiles() const { return missiles; }
	unsigned int getFrame() const { return frame; }
	float getSpeed() const { return speed; }
    
	void setHp(unsigned int hp) { this->hp = hp; }
	void setLives(unsigned int hp) { this->lives = hp; }
	void setMissiles(unsigned int hp) { this->missiles = hp; }
	void setDirection(DIRECTION d) { playerDirection = d; }
};
#endif