// Pang.cpp : Defines the entry point for the console application.
//

#define NOMINMAX
#include "header.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"

using namespace global;

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
	std::vector<global::enemy_ptr> enemyVec; //A vector of smart pointers. Each enemy spawned goes in here
	sf::Clock spawnClock;
	if (spriteSheet.loadFromFile(SPRITEPATH) == false)
	{
		MessageBoxA(NULL, "Could not open file: \"resources/images/sheet.png\"", "Error Opening File", MB_ICONERROR | MB_OK);
		return 1;
	}
	Player player;
	rngEng.seed((unsigned int)std::time(0)); //Seed the RNG engine with the current time
	sf::Font font;
	if (font.loadFromFile(FONTPATH) == false)
	{
		MessageBoxA(NULL, "Could not open file: \"resources/fonts/Opificio_rounded.ttf\"", "Error Opening File", MB_ICONERROR | MB_OK);
		return 1;
	}
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