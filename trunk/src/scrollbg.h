class ScrollingBG
{
private:
	SDL_Surface *BackGround;
	SDL_Surface *BackGroundCopy;
	
	int bgx, bgxc;
	
	int velocity;
	
public:
	ScrollingBG( SDL_Surface *bg, int vel )
	{
		BackGround = bg;
		BackGroundCopy = BackGround;
		
		bgx = 0;
		bgxc = 640;
		velocity = vel;
	}
	
	void Update()
	{
		bgx -= velocity;
		bgxc -= velocity;
		
		if ( bgx <= -640 ) bgx = 640;
		if ( bgxc <= -640 ) bgxc = 640;
	}
	
	void Draw( SDL_Surface *apply_s )
	{
		apply_surface( bgx, 0, BackGround, apply_s );
		apply_surface( bgxc, 0, BackGroundCopy, apply_s );
	}
};
