// Pang.cpp : Defines the entry point for the console application.
//

#define NOMINMAX
#include "header.h"
#include <Windows.h>
#include <list>
#include <boost/random.hpp>
#include <ctime>
#include <boost/shared_ptr.hpp>

class Player;
class Bullet;
class Enemy;
namespace
{
	static sf::Texture spriteSheet; //DEFINE IN GAME STATE
	static boost::random::mt19937 rngEng;
	typedef boost::shared_ptr<Enemy> enemy_ptr;
}

class Bullet
{
private:
	sf::Sprite bulletSprite;
	static UINT damage;
	float speed;
	sf::Vector2f position;
public:
	Bullet(float x, float y)
	{
		speed = 10.f;
		position.x = x;
		position.y = y;
		bulletSprite.setTexture(spriteSheet);
		bulletSprite.setTextureRect(sf::IntRect(380, 58, 8, 21));
		bulletSprite.setOrigin(bulletSprite.getLocalBounds().width / 2, bulletSprite.getLocalBounds().height / 2);
	}
	~Bullet() { }
	void update()
	{
		position.y -= speed;
		bulletSprite.setPosition(position);
	}
	void render(sf::RenderWindow& rw)
	{
		rw.draw(bulletSprite);
	}

	sf::Sprite getSprite() const { return bulletSprite; }
	static UINT getDamage() { return damage; }
};
UINT Bullet::damage = 20;

class HealthBar
{
private:
	sf::RectangleShape barBg;
	sf::RectangleShape bar;
	float width;
	float height;
public:
	HealthBar(float width, float height)
		:width(width), height(height)
	{
		barBg.setSize(sf::Vector2f(width, height));
		barBg.setFillColor(sf::Color(sf::Color::White));
		bar.setSize(sf::Vector2f(width, height));
		bar.setFillColor(sf::Color::Red);
	}
	HealthBar(float width, float height, float x, float y)
		:width(width), height(height)
	{
		barBg.setSize(sf::Vector2f(width, height));
		barBg.setFillColor(sf::Color(sf::Color::White));
		barBg.setPosition(x, y);
		bar.setSize(sf::Vector2f(width, height));
		bar.setFillColor(sf::Color::Red);
		bar.setPosition(x, y);
	}
	void attach(sf::Sprite* object)
	{
		barBg.setOrigin(width / 2, height / 2);
		bar.setOrigin(width / 2, height / 2);
		//Just making sure it's in the middle and above the object
		barBg.setPosition(object->getPosition().x, object->getPosition().y - object->getLocalBounds().height / 2 - 10);
		bar.setPosition(barBg.getPosition());
	}
	void update(sf::Sprite* object, float max, float current)
	{
		barBg.setPosition(object->getPosition().x, object->getPosition().y - object->getLocalBounds().height / 2 - 10);
		bar.setPosition(barBg.getPosition());
		float percent = current / max;
		bar.setSize(sf::Vector2f(width * percent, height));
	}
	void render(sf::RenderWindow& rw)
	{
		rw.draw(barBg);
		rw.draw(bar);
	}
};

class Enemy
{
private:
	UINT health;
	UINT maxHealth;
	sf::Sprite sprite;
	float width, height;
	int vel;
	sf::Clock fireRate;
public:
	HealthBar healthBar;
	Enemy(float x, float y)
		:healthBar(75, 10)
	{
		sprite.setTexture(spriteSheet);
		sprite.setTextureRect(getRandomSprite());
		width = sprite.getLocalBounds().width;
		height = sprite.getLocalBounds().height;
		sprite.setOrigin(width / 2, height / 2);
		sprite.setPosition(x, y);
		maxHealth = health = 100;
		healthBar.attach(&sprite);
		vel = 1;
	}

	Enemy(const std::vector<enemy_ptr>& vec)
		:healthBar(75, 10)
	{
		sprite.setTexture(spriteSheet);
		sprite.setTextureRect(getRandomSprite());
		width = sprite.getLocalBounds().width;
		height = sprite.getLocalBounds().height;
		sprite.setOrigin(width / 2, height / 2);
		sprite.setPosition(getRandomSpawn(vec));
		maxHealth = health = 100;
		healthBar.attach(&sprite);
		vel = 1;
	}
	~Enemy() { }

	sf::IntRect getRandomSprite()
	{
		boost::random::uniform_int_distribution<> dist(1, 2);
		int genNum = dist(rngEng);
		switch (genNum)
		{
		case 1: return sf::IntRect(380, 0, 40, 57);
		case 2: return sf::IntRect(421, 0, 40, 57);
		default: return sf::IntRect(0, 0, 0, 0);
		}
	}

	sf::Vector2f getRandomSpawn(const std::vector<enemy_ptr>& vec)
	{
		boost::random::uniform_int_distribution<> dist(20, 1260);
		int genPosX = 0;
		//Keep generating a new x position until it doesn't insersect any other enemy
		for(;;)
		{
			genPosX = dist(rngEng);
			sf::FloatRect genRect((float)genPosX, -30.f, 40.f, 57.f);
			for (std::vector<enemy_ptr>::const_iterator iter = vec.begin(); iter != vec.end(); iter++)
			{
				if ((*iter)->getSprite().getGlobalBounds().intersects(genRect))
				{
					continue;
				}
			}
			return sf::Vector2f((float)genPosX, -10.f);
		}
	}

	void update()
	{
		this->sprite.move(0, (float)vel);
		healthBar.update(&this->sprite, (float)maxHealth, (float)health);
	}

	void render(sf::RenderWindow& rw)
	{
		rw.draw(sprite);
		healthBar.render(rw);
	}

	UINT getHealth() { return health; }

	const sf::Sprite& getSprite() { return sprite; }	 
	UINT getHealth() const { return health; }
	void damageEnemy(UINT delta) { health -= delta; std::cout << "Health: " << health << std::endl; }
	bool checkCollision(sf::Vector2f c)
	{
		if (sprite.getGlobalBounds().contains(c))
			return true;
		else return false;
	}

	bool checkCollision(std::list<Bullet>& list)
	{
		for (std::list<Bullet>::iterator iter = list.begin(); iter != list.end();)
		{
			//If the sprite collided AND it's inside the screen
			if (sprite.getGlobalBounds().intersects(iter->getSprite().getGlobalBounds()) && sprite.getPosition().y > 0)
			{
				list.erase(iter);
				return true;
			}
			else ++iter;
		}
		return false;
	}
};

enum DIRECTION { UP, DOWN, LEFT, RIGHT, STATIONARY, TRANSITION_TO_STATIONARY };
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
	UINT lives;
	UINT hp;
	UINT missiles; //NI

	UINT frame;
	sf::Vector2f position;
	const float speed;

	DIRECTION playerDirection;

	std::list<Bullet> playersBullets;

	//Clock to regulate the firing of bullets
	sf::Clock bulletClock;

	HealthBar playerHealthBar;
public:
	Player()
		:speed(7.75f), playerHealthBar(150.f, 50.f, 20, 650)
	{
		isAlive = true;
		lives = 3;
		hp = 100;
		missiles = 3;
		frame = 5;
		position.x = 640;
		position.y = 620;
		bulletClock.restart();
	}
	bool init()
	{
		for (int i = 0; i < sizeof(playerShip) / sizeof(playerShip[0]); i++)
		{
			playerShip[i].setTexture(spriteSheet);
		}

		playerShip[0].setTextureRect(sf::IntRect(0, 0, 94, 100));
		playerShip[1].setTextureRect(sf::IntRect(190, 0, 94, 100));
		playerShip[2].setTextureRect(sf::IntRect(285, 0, 94, 100));
		playerShip[3].setTextureRect(sf::IntRect(190, 101, 94, 100));
		playerShip[4].setTextureRect(sf::IntRect(285, 101, 94, 100));
		playerShip[5].setTextureRect(sf::IntRect(95, 0, 94, 100));
		playerShip[6].setTextureRect(sf::IntRect(0, 101, 94, 100));
		playerShip[7].setTextureRect(sf::IntRect(95, 101, 94, 100));

		for (int i = 0; i < sizeof(playerShip) / sizeof(playerShip[0]); i++)
		{
			playerShip[i].setOrigin(playerShip[i].getLocalBounds().width / 2, playerShip[i].getLocalBounds().height / 2);
		}
		return true;
	}
	void update()
	{
		switch (playerDirection)
		{
		case STATIONARY:
			break;
		case TRANSITION_TO_STATIONARY:
			switch (frame)
			{
			case 1: frame = 5; playerDirection = STATIONARY; break;
			case 2: frame = 1; break;
			case 3: frame = 5; playerDirection = STATIONARY; break;
			case 4: frame = 3; break;
			}
			break;
		case UP: 
			//NI
			break;
		case DOWN:
			//NI
			break;
		case LEFT:
			if (frame == 0)
				frame = 1;
				frame = 2;
			move(-speed, 0);
			break;
		case RIGHT:
			if (frame == 0)
			{
				frame = 3;
				break;
			}
			frame++;
			if (frame > 4)
				frame = 4;
			move(speed, 0);
			break;
		}
		playerShip[frame].setPosition(position);

		if (!playersBullets.empty())
		{
			for (std::list<Bullet>::iterator iter = playersBullets.begin(); iter != playersBullets.end();)
			{
				if (iter->getSprite().getPosition().y < 0 - iter->getSprite().getOrigin().y) 
				{
					iter = playersBullets.erase(iter);
				}
				else 
					++iter;
			}
		}
		if (!playersBullets.empty())
		{
			for (std::list<Bullet>::iterator iter = playersBullets.begin(); iter != playersBullets.end(); iter++)
			{
				iter->update();
			}
		}
	}

	void render(sf::RenderWindow& rw)
	{
		rw.draw(playerShip[frame]);
		for (std::list<Bullet>::iterator iter = playersBullets.begin(); iter != playersBullets.end(); iter++)
		{
			iter->render(rw);
		}
		playerHealthBar.render(rw);
	}

	void kill()
	{
		isAlive = false;
		lives--;
	}

	void fireBullet()
	{
		//If it has been less than 250ms since the last bullet was fired return
		if (bulletClock.getElapsedTime().asMilliseconds() < 250)
			return;
		bulletClock.restart();
		Bullet b(playerShip[frame].getPosition().x, playerShip[frame].getPosition().y - 50);
		playersBullets.push_back(b);
	}

	std::list<Bullet>& getBulletList() { return playersBullets; }

	bool checkCollision(sf::Vector2f c)
	{
		if (playerShip[frame].getGlobalBounds().contains(c))
			return true;
		else return false;
	}
	void move(float x, float y)
	{
		position.x += x;
		position.y += y;
		if (position.x  - 47 < 0)
			position.x = 47;

		if (position.x + 47 > 1280)
			position.x = 1280 - 47;

		if (position.y - 47 < 0)
			position.y = 47;

		if (position.y + 47 > 720)
			position.y = 720 - 47;
	}

	UINT getHp() const { return hp; }
	UINT getLives() const { return lives; }
	UINT getMissiles() const { return missiles; }
	UINT getFrame() const { return frame; }
	float getSpeed() const { return speed; }

	void setHp(UINT hp) { this->hp = hp; }
	void setLives(UINT hp) { this->lives = hp; }
	void setMissiles(UINT hp) { this->missiles = hp; }
	void setDirection(DIRECTION d) { playerDirection = d; }
};

#if defined _WIN32 && defined NDEBUG
#define WIN32_LEAN_AND_MEAN
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
{
//------------------------------Various Initialisations-----------------------------
	sf::RenderWindow rw(sf::VideoMode(1280,720), "Not Space Invaders");
	rw.setVerticalSyncEnabled(true);
	sf::Event event;
	Player player;
	std::vector<enemy_ptr> enemyVec; //A vector of smart pointers. Each enemy spawned goes in here
	sf::Clock spawnClock;
	if (spriteSheet.loadFromFile("resources/images/sheet.png") == false)
	{
		MessageBoxA(NULL, "Could not open file: \"resources/images/sheet.png\"", "Error Opening File", MB_ICONERROR | MB_OK);
		return 1;
	}
	player.init(); //Must init player after loading sprites
	rngEng.seed((unsigned int)std::time(0)); //Seed the RNG engine with the current time
	sf::Font font;
	if (font.loadFromFile("resources/fonts/Opificio_rounded.ttf") == false)
	{
		MessageBoxA(NULL, "Could not open file: \"resources/fonts/Opificio_rounded.ttf\"", "Error Opening File", MB_ICONERROR | MB_OK);
		return 1;
	}
//----------------------------------------------------------------------------------
//-------------------------Game Help------------------------------------------------
	sf::Text readyPlayText("Arrow Keys to Move\nSpace to Shoot\nPress Enter to Begin", font, 72);
	readyPlayText.setOrigin(readyPlayText.getLocalBounds().width / 2, readyPlayText.getLocalBounds().height / 2);
	readyPlayText.setPosition(640, 360);

	rw.clear(sf::Color::Black);
	rw.draw(readyPlayText);
	rw.display();
	for (;;)
	{	
		if (rw.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) break;
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) return 0;
			if (event.type == sf::Event::Closed) return 0;
		}
	}
//----------------------------------------------------------------------------------
	spawnClock.restart(); //Start clock just before entering the loop
	while (rw.isOpen())
	{
//---------------------HANDLE EVENTS-------------------------------
		while (rw.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed: return 0; break;
			case sf::Event::KeyPressed:
				{
					if (event.key.code == sf::Keyboard::Escape) return 0;
					if (event.key.code == sf::Keyboard::Left)
					{
						player.setDirection(LEFT);
					}
					if (event.key.code == sf::Keyboard::Right)
					{
						player.setDirection(RIGHT);
					}
					if (event.key.code == sf::Keyboard::Space)
					{
						player.fireBullet();
					}
				}
				break;
			case sf::Event::KeyReleased: 
				if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right)
					player.setDirection(TRANSITION_TO_STATIONARY); break;
			}
		}
//----------------------------------------------------------------------------------------------------
//--------------------UPDATE-------------
		//Time related stuff
		//If it has been 1.5s since last spawn of enemy
		if (spawnClock.getElapsedTime().asMilliseconds() >= 3000)
		{
			enemy_ptr temp(new Enemy(enemyVec));
			enemyVec.push_back(temp);
			spawnClock.restart();
		}

		//Check all collisions
		for (std::vector<enemy_ptr>::iterator iter = enemyVec.begin(); iter != enemyVec.end(); iter++)
		{
			if ((*iter)->checkCollision(player.getBulletList()))
			{
				(*iter)->damageEnemy(Bullet::getDamage());
			}
		}

		//Check health of anything
		//Enemies
		for (std::vector<enemy_ptr>::iterator iter = enemyVec.begin(); iter != enemyVec.end();)
		{
			if ((*iter)->getHealth() <= 0)
			{
				std::cout << "Enemy Killed!" << std::endl;
				iter = enemyVec.erase(iter);
			}
			else
				++iter;
		}

		//Finally Update everything
		player.update();
		for (std::vector<enemy_ptr>::iterator iter = enemyVec.begin(); iter != enemyVec.end(); iter++)
		{
			(*iter)->update();
		}
//----------------------------------------------------------------------------------------------------
//-----------------RENDERING-----------------------------------------
		rw.clear();
		player.render(rw);
		for (std::vector<enemy_ptr>::iterator iter = enemyVec.begin(); iter != enemyVec.end(); iter++)
		{
			(*iter)->render(rw);
		}
		rw.display();
//-------------------------------------------------------------------
	}
	return EXIT_SUCCESS;
}

