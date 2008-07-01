#define restrict

#define media_directory "media/"
//#define media_directory "/usr/share/valix/media/"

char *const concatstrings( char const *const restrict, char const *const restrict );
char *const concatmediadir( char const *const restrict );

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <cassert>
#include <cstdlib>
#include <cstddef>
#include <cstring>
#include <iostream>
#include "addblit.h"
#include <ctime>
#include <sstream>
#include <math.h>
#include "rectangle.h"
#include "Timer.h"
#include "linkedlist.h"
#include "projectilemanager.h"
#include "player.h"
#include "scrollbg.h"
#include "enemy.h"
#include "enemymanager.h"

//concatenate 2 char arrays
char *const concatstrings( char const *const restrict a, char const *const restrict b )
{
	assert(a); assert(b);

	using std::size_t; using std::strlen; using std::memcpy;

	size_t const lenA = strlen(a);
	size_t const lenB = strlen(b);

	char *const buf = new char[ lenA + lenB + 1 ];

	register char *p = buf;

	memcpy(p,a,lenA); p += lenA;
	memcpy(p,b,lenB);

	p[lenB] = 0;

	return buf;
}

char *const concatmediadir( char const *const restrict dir )
{
	return concatstrings( media_directory, dir );
}
