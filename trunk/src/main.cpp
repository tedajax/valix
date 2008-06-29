#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "addblit.h"
#include <cassert>
#include <string>
#include <cstdlib>
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

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

SDL_Surface *screen = 0;
SDL_Surface *playerinfo = 0;
SDL_Surface *bg = 0;
SDL_Surface *infobar = 0;
SDL_Surface *skillbutton = 0;
SDL_Surface *expbar = 0;

TTF_Font *font = 0;
SDL_Color textColor = { 255, 255, 255 };

const int FRAMES_PER_SECOND = 60;

SDL_Event event;

Player *MainShip;
EnemyManager *EMngr;
ScrollingBG *ScrBG;

bool init()
{
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	return false;

    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

    if ( screen == NULL )
	return false;

    if ( TTF_Init() == -1 )	return false;

    SDL_WM_SetCaption( "Valix", NULL );

    return true;
}

bool load_files()
{
	MainShip = new Player( "media/images/mainship.png" );	
	
	bg = load_image( "media/images/background.png" );
	infobar = load_image( "media/images/infobar.png" );
	skillbutton = load_image( "media/images/skillbutton.png" );
	expbar = load_image( "media/images/expbar.png" );
	font = TTF_OpenFont( "media/fonts/tahoma.ttf", 12 );
	
	ScrBG = new ScrollingBG( bg, 1 );
	EMngr = new EnemyManager();
	
	for (int x = 0; x < 480; x += 64)
		EMngr->AddEnemy( 2, 640, x );
	
    return true;
}

void clean_up()
{
	delete MainShip;
    SDL_Quit();
    TTF_Quit();
}

void Update()
{
	ScrBG->Update();
	MainShip->Update();
	EMngr->Update();
}

void Draw()
{
	ScrBG->Draw( screen );
	
	MainShip->Draw( screen );
	
	EMngr->Draw( screen );
	
	apply_surface( 0, 452, infobar, screen );
	apply_surface( 0, 455, skillbutton, screen );
	apply_surface( 26, 456, expbar, screen );
	
	boxRGBA( screen, 26, 456, 26 + (int)MainShip->getExpPercent() * 2, 479, 255, 255, 0, 255 );
		
	stringstream out;
	string playerdebuginfo;
	
	out.str("");
	out << MainShip->getPositionX() << " : " << MainShip->getPositionY() << " : " << MainShip->getProjectileCount() << " : " << (int)MainShip->getExpPercent() * 2;
	playerdebuginfo = out.str();
	
	playerinfo = TTF_RenderText_Solid( font, playerdebuginfo.c_str(), textColor );
	apply_surface( 5, 5, playerinfo, screen );
}

int main( int argc, char* args[] )
{
    bool quit = false;

    if ( init() == false )
		return 1;

    if ( load_files() == false )
		return 2;

    Uint8 *keystates = SDL_GetKeyState( NULL );
	    
	Timer fps;
		
    while ( quit == false )
    {
    	fps.start();
    	
    	
		while ( SDL_PollEvent( &event ) )
		{
			if ( event.type == SDL_QUIT )
			quit = true;
		}
		
		keystates = SDL_GetKeyState( NULL );

		if ( keystates[ SDLK_ESCAPE ] )
			quit = true;

		Update();
		Draw();
					
		if ( SDL_Flip( screen ) == -1 )
			return 3;
		
		if ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
		{
			SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
		}
    }

    clean_up();

    return 0;
}
