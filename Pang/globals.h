#ifndef _GLOBALS_
#define _GLOBALS_

#include "header.h"

class Enemy;
class Bullet;

namespace global
{
	static sf::Texture spriteSheet;
	static boost::random::mt19937 rngEng;
	typedef boost::shared_ptr<Enemy> enemy_ptr;
}

#endif