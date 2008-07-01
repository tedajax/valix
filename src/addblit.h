#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include <cassert>
using namespace std;

//For loading images
SDL_Surface *load_image( string filename )
{
    //Temporary for holding newly loaded image
    SDL_Surface *loadedimage = NULL;

    //Optimized image that will be used
    SDL_Surface *optimizedimage = NULL;

    //Load the image
    loadedimage = IMG_Load( filename.c_str() );

    if ( loadedimage != NULL )
    {
        //Create optimized image
        optimizedimage = SDL_DisplayFormat( loadedimage );

        //Free the old image (delete)
        SDL_FreeSurface( loadedimage );
    }

    if ( optimizedimage != NULL )
    {
        Uint32 colorkey = SDL_MapRGB( optimizedimage->format, 0xFF, 0, 0xFF );
        SDL_SetColorKey( optimizedimage, SDL_SRCCOLORKEY, colorkey );
    }

    //Return the optimized image
    return optimizedimage;
};


//Apply an image to the screen
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* dest )
{
    //Temp rectangle to hold offsets
    SDL_Rect offset;

    //Assign offset values specified in parameters
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, NULL, dest, &offset );
};
