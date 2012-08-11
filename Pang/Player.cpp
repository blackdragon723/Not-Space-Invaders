#include "Player.h"

Player::Player(sf::Texture& tex)
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

	for (int i = 0; i < sizeof(playerShip) / sizeof(playerShip[0]); i++)
	{
		playerShip[i].setTexture(tex);
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
}

Player::~Player()
{
}

void Player::kill()
{
	isAlive = false;
	lives--;
}

void Player::fireBullet()
{
	//If it has been less than 250ms since the last bullet was fired, return early
	if (bulletClock.getElapsedTime().asMilliseconds() < 250)
		return;
	bulletClock.restart();
	bullet_ptr b(new Bullet(playerShip[frame].getPosition().x, playerShip[frame].getPosition().y - 50));
	playersBullets.push_back(b);
}

bool Player::checkCollision(sf::Vector2f c)
{
	if (playerShip[frame].getGlobalBounds().contains(c))
		return true;
	else return false;
}

void Player::move(float x, float y)
{
	position.x += x;
	position.y += y;

	//If position is off the screen keep it in the screen
	if (position.x  - 47 < 0)
		position.x = 47;
        
	if (position.x + 47 > 1280)
		position.x = 1280 - 47;
        
	if (position.y - 47 < 0)
		position.y = 47;
        
	if (position.y + 47 > 720)
		position.y = 720 - 47;
}

void Player::update()
{
	//Animation stuff
	switch (playerDirection)
	{
		case STATIONARY: break;

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

	//Change the position
	playerShip[frame].setPosition(position);
    
	//Check if bullets are off the screen
	//Delete if they are
	if (!playersBullets.empty())
	{
		for (std::list<bullet_ptr>::iterator iter = playersBullets.begin(); iter != playersBullets.end();)
		{
			if ((*iter)->getSprite().getPosition().y < 0 - (*iter)->getSprite().getOrigin().y) 
			{
				iter = playersBullets.erase(iter);
			}
			else 
				++iter;
		}
	}

	//Update each bullet
	if (!playersBullets.empty())
	{
		for (std::list<bullet_ptr>::iterator iter = playersBullets.begin(); iter != playersBullets.end(); iter++)
		{
			(*iter)->update();
		}
	}
}

void Player::render(sf::RenderWindow& rw)
{
	rw.draw(playerShip[frame]);
	playerHealthBar.render(rw);

	//Draw each bullet
	for (std::list<bullet_ptr>::iterator iter = playersBullets.begin(); iter != playersBullets.end(); iter++)
	{
		(*iter)->render(rw);
	}
}