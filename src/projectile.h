class Projectile
{
private:
    SDL_Surface *laserimage;
    float posx, posy;
    bool laser;
	bool destroy;

public:
    Projectile( float px, float py, bool l )
    {
		laser = l;

		if ( !laser )
			laserimage = load_image( concatmediadir( "images/projectile.png" ) );
		else
			laserimage = load_image( concatmediadir( "images/laser.png" ) );

		posx = px;
		posy = py;
		
		destroy = false;
    };

	Projectile()
	{
		laser = false;
		posx = 0;
		posy = 0;
	}

    ~Projectile()
    {
		SDL_FreeSurface( laserimage );
    };

    void Update()
    {
		posx += 15.0f;
		
		if ( posx > 640 )
			destroy = true;
    };

    void Draw( SDL_Surface *apply_s )
    {
		apply_surface( posx, posy, laserimage, apply_s );
    };
    
    bool Destroy() { return destroy; }
};
