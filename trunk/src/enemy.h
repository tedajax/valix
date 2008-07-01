//Enemy class
//Handles drawing, updating, storing information about
//an individual enemy
class Enemy
{
private:
	//x position (in pixels) and y position (in pixels)
	float posx, posy;
	//x and y velocity (pixels per frame)
	float velx, vely;
	//type of enemy, determines image, AI style
	short type;
	//Used to change between different types of AI movement
	short AIstage;
	//enemy image
	SDL_Surface *image;
	//Tells the enemy manager to remove the enemy if it's true
	bool destroy;
	
	//Determines the image to load based on the type
	SDL_Surface* getImageFromType( short t )
	{
		//Stores the surface to return
		SDL_Surface *returnsurface;
		switch ( t )
		{
			//Set the surface based on type value
			//If no type matches automatically becomes type 1
			case 1:
				returnsurface = load_image( "media/images/enemy.png" );
				break;
			case 2:
				returnsurface = load_image( "media/images/enemy2.png" );
				break;
			default:
				returnsurface = load_image( "media/images/enemy.png" );
				break;
		}
		
		return returnsurface;
	}
	
public:
	//Different constructors
	Enemy( short t )
	{
		type = t;
		posx = 0.0f;
		posy = 0.0f;
		velx = 0.0f;
		vely = 0.0f;
		
		AIstage = 0;
		
		destroy = false;
		
		image = getImageFromType( type );
	}
	
	Enemy( short t, float px, float py )
	{
		type = t;
		posx = px;
		posy = py;
		velx = 0.0f;
		vely = 0.0f;
		
		AIstage = 0;
		
		destroy = false;
		
		image = getImageFromType( type );

	}
	
	Enemy( short t, float px, float py, float vx, float vy )
	{
		type = t;
		posx = px;
		posy = py;
		velx = vx;
		vely = vy;
		
		AIstage = 0;
		
		destroy = false;
		
		image = getImageFromType( type );
	}
	
	~Enemy()
	{
		SDL_FreeSurface( image );
	}
	
	//Basically determines the type, goes to that AI movement style
	//Then using the current stage moves in the manner programmed
	void Update()
	{
		switch ( type )
		{
			case 1:
				switch ( AIstage )
				{
					case 0:
						//Basically send it to the left, restrict maximum velocity as well
						if ( velx > -6.0f )
							velx -= 0.2f;
							
						//Determines when to send the enemy the other direction
						if ( posx <= 100 )
							AIstage = 1;
						
						break;
					case 1:
						//move to the right quickly
						velx += 0.4f;
						
						//destroy the enemy when it is no longer in the drawable area
						if ( posx > 640 )
							destroy = true;
							
						break;
				}
				break;
				
			case 2:
				//slowly move to the left
				velx = -3;
				//destroy once it gets passed the drawable location
				if ( posx < -64 )
					destroy = true;
				break;
		}
		
		//Update the positions based on velocity
		posx += velx;
		posy += vely;
	}	
	
	//Simply apply the SDL surface using the apply_surface function from addblit.h
	void Draw( SDL_Surface *apply_s )
	{
		apply_surface( posx, posy, image, apply_s );
	}
	
	//Lets the enemy manager know to remove the enemy from the game
	bool Destroy() { return destroy; }
};
