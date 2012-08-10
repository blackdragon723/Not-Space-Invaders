// header.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <stdio.h>
#include <ctime>

#include <boost/random.hpp>
#include <boost/shared_ptr.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>

#include <map>
#include <list>
#include <iostream>
#include <cassert>
#include <string>
#include <sstream>

#ifdef __APPLE__
#include "ResourcePath.hpp"
#define SPRITEPATH resourcePath() + "sheet.png"
#define FONTPATH   resourcePath() + "Opificio_rounded.ttf"
#else
#define SPRITEPATH "resources/images/sheet.png"
#define FONTPATH   "resources/fonts/Opificio_rounded.ttf"
#include <windows.h>
#include <tchar.h>
#endif

// TODO: reference additional headers your program requires here

