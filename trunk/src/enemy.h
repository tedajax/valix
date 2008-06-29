class Enemy
{
private:
	float posx, posy;
	float velx, vely;
	short type;
	short AIstage;
	SDL_Surface *image;
	bool destroy;
	
	SDL_Surface* getImageFromType( short t )
	{
		SDL_Surface *returnsurface;
		switch ( t )
		{
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
	
	void Update()
	{
		switch ( type )
		{
			case 1:
				switch ( AIstage )
				{
					case 0:
						if ( velx > -6.0f )
							velx -= 0.2f;
							
						if ( posx <= 100 )
							AIstage = 1;
						
						break;
					case 1:
						velx += 0.4f;
						
						if ( posx > 640 )
							destroy = true;
							
						break;
				}
				break;
				
			case 2:
				velx = -3;
				if ( posx < -64 )
					destroy = true;
				break;
		}
		
		posx += velx;
		posy += vely;
	}	
	
	void Draw( SDL_Surface *apply_s )
	{
		apply_surface( posx, posy, image, apply_s );
	}
	
	bool Destroy() { return destroy; }
};
