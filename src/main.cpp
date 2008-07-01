//Includes, ugly but it's there
#include "valix.h"
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
#include <cstddef>
#include <cstring>

#define restrict

using namespace std;

//Basic screen constant settings
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 32

//The screen surface is the base SDL_Surface that all the other surfaces are applied to
SDL_Surface *screen = 0;
//The playerinfo surface is used for debugging, player position, bullet count, etc...
SDL_Surface *playerinfo = 0;
//bg surface is where the scrolling background image is applied
SDL_Surface *bg = 0;
//infobar contains all of the data at the bottom of the screen
SDL_Surface *infobar = 0;
//skill button is the little icon in the bottom-left, will later be used to upgrade skills
SDL_Surface *skillbutton = 0;
//surface for displaying the experience bar at the bottom of the screen
SDL_Surface *expbar = 0;

//For displaying information with text...
TTF_Font *font = 0;
//textcolor
SDL_Color textColor = { 255, 255, 255 };

#define FRAMES_PER_SECOND 60

//For exiting the window
SDL_Event event;

//Player object
Player *MainShip;
//An enemy manager for keeping track of, updating, and drawing all the enemies
EnemyManager *EMngr;
//Scrolling background class that simply automatically scrolls the background image seemlessly
ScrollingBG *ScrBG;

//concatstrings prototype
char *const concatstrings( char const *const restrict, char const *const restrict );

//SDL initialization, nothing fancy
bool init()
{
    //Call SDLs initilization function and return false (thus ending the program) if it doesn't work
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
		return false;

    //Set the window attributes
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

    //If the window was created succesfully the program will continue otherwise it will end
    if ( screen == NULL )
		return false;

    //Initialize SDL_ttf
    if ( TTF_Init() == -1 )	return false;

    //Set the window caption
    SDL_WM_SetCaption( "Valix", NULL );

    //Initialization was a success
    return true;
}

//Load media, images, sounds, fonts, etc...
bool load_files()
{
    //Creates the Player class by sending the location of the image for the player to use
    MainShip = new Player( concatstrings( media_directory, "images/mainship.png" ) );	
    
    //Background image for the scrolling background
    bg = load_image( concatstrings( media_directory, "images/background.png" ) );
    //Black box, can be replaced with an SDL_gfx call, but hasn't been done yet
	//may want to create something more decorative in the future
    infobar = load_image( concatstrings( media_directory, "images/infobar.png" ) );
    //Image in the bottom-left corner, used for advancing skills
    skillbutton = load_image( concatstrings( media_directory, "images/skillbutton.png" ) );
    //Experience bar image
    expbar = load_image( concatstrings( media_directory, "images/expbar.png" ) ); 
    //Load the tahoma font
    font = TTF_OpenFont( concatstrings( media_directory, "fonts/tahoma.ttf" ), 12 );
    
    //Create the scrolling background, giving it speed 1 (1 pixel of movement per frame)
    ScrBG = new ScrollingBG( bg, 1 );
    //Create the enemy manager
    EMngr = new EnemyManager();
    
    //Create some testing enemies
    for (int x = 0; x < 480; x += 64)
    //The first parameter specifies the type of enemy, this will indicate both the image to use
    //and the AI pattern for the enemy to use
    //The other parameters are the starting X and Y coordinates to use
    EMngr->AddEnemy( 2, 640, x );
	
    //Hopefully everything worked and we get here without a segfault -_-
    return true;
}

//Simple cleanup function
//Not sure what all needs to go here help appreciated
void clean_up()
{
    //Delete the mainship cause I can
    delete MainShip;
    //Kill SDL
    SDL_Quit();
    //Kill SDL_ttf
    TTF_Quit();
}

//Update Function, called before Draw
void Update()
{
    //Update the background (move it to the side)
    ScrBG->Update();
    //Update the player (accept input, move around, create bullets)
    MainShip->Update();
    //Update every enemy in the enemy manager
    EMngr->Update();
}

//Draw function, called after update
void Draw()
{
    //Draw the background first, apply it to the SDL_Surface "screen"
    ScrBG->Draw( screen );
    
    //Draw the player
    MainShip->Draw( screen );
    
    //Draw the enemies
    EMngr->Draw( screen );
    
    //all the info at the bottom of the screen
    //may later change all of this to SDL_gfx function for easy scaling
    apply_surface( 0, 452, infobar, screen );
    apply_surface( 0, 455, skillbutton, screen );
    apply_surface( 26, 456, expbar, screen );
    
    //Draw the current exp stuff
    boxRGBA( screen, 26, 456, 26 + (int)MainShip->getExpPercent() * 2, 479, 255, 255, 0, 255 );
    	
    //Debugging code
    stringstream out;
    string playerdebuginfo;
    
	//Generate the debug stream
    out.str("");
    out << MainShip->getPositionX() 
		<< " : " << MainShip->getPositionY() 
		<< " : " << MainShip->getProjectileCount() 
		<< " : " 
		<< (int)MainShip->getExpPercent() * 2;
    playerdebuginfo = out.str();
    
	//Use SDL_ttf to 
    playerinfo = TTF_RenderText_Solid( font, playerdebuginfo.c_str(), textColor );
    apply_surface( 5, 5, playerinfo, screen );
    //End debugging code
}

int main( int argc, char* args[] )
{
    //used to determine when to end the program
    bool quit = false;

    //Call init, if it doesn't work kill the game
    if ( !init() )
    	return 1;

    //Load the files if it doesn't work DIE DIE DIE
    if ( !load_files() )
    	return 2;

    //Update the keystates
    Uint8 *keystates = SDL_GetKeyState( NULL );
	    
    //Create a timer for keeping the FPS
    Timer fps;
		
    //Main loop
	while ( quit == false )
	{
		//Start recording milliseconds that go by
    	fps.start();    	
    	
		//Check if the exit button pressed on window
    	while ( SDL_PollEvent( &event ) )
    	{
			if ( event.type == SDL_QUIT )
				quit = true;
    	}
    	
		//Update keystates
    	keystates = SDL_GetKeyState( NULL );

		//Exit if escape key pressed
   		if ( keystates[ SDLK_ESCAPE ] )
   			quit = true;

		//Call update
    	Update();
		//Here some timer stuff has to happen that keeps everything synced properly
		//Draw
    	Draw();
    		
		//Update the screen		
    	if ( SDL_Flip( screen ) == -1 )
			return 3;
    	
		//Delay to maintain a maximum FPS, I have no idea if this works XD
    	if ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
	    {
	   		SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
    	}
	}

    //uh yeah
    clean_up();

    //program exits with no errors
    return 0;
}

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
