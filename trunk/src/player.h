#include <math.h>

class PlayerData
{
private:
    int level;
    int totalexp;
    int currentexp;
    int skillpoints;

    int rateoffire; //rate of fire measured in frames
    int bulletcount;
    bool laser;

	float movespeed;

    int requiredexp;

    //Checks for level up and adjusts level, exp, and skillpoints accordingly
    void levelUp()
    {
		//In case they get a lot of EXP we use a while loop
		while (currentexp > requiredexp)
		{
			//Wrap extra EXP around
			currentexp -= requiredexp;

			//Increase the level and skillpoints
			level += 1;
			skillpoints += 3;

			//Increase the required EXP
			requiredexp += level * 50;
		}
    }

public:
    PlayerData()
    {
		level = 0;
		totalexp = 0;
		currentexp = 0;
		requiredexp = 50;
		skillpoints = 0;

		rateoffire = 20;
		bulletcount = 1;
		laser = false;
		
		movespeed = 3.0f;
    }

    ~PlayerData()
    {
	
    }

    int getLevel() { return level; };
    int getTotalExp() { return totalexp; };
    int getCurrentExp() { return currentexp; };
    int getRequiredExp() { return requiredexp; };
    int getSkillPoints() { return skillpoints; };
    int getRateOfFire() { return rateoffire; };
    int getBulletCount() { return bulletcount; };
    float getMoveSpeed() { return movespeed; };
    bool getLaser() { return laser; };

    void setLevel(int l) { level = l; };
    void incExp(int e)
    {
		totalexp += e;
		currentexp += e;
		levelUp();
    }
    void setSkillPoints(int sp) { skillpoints = sp; };
    void setRateOfFire(int rof) { rateoffire = rof; };
    void setBulletCount(int bc) { bulletcount = bc; };
    void setLaser(bool la) { laser = la; };
    void setMoveSpeed(float m) { movespeed = m; };
};


class Player
{
private:
    PlayerData data;
    SDL_Surface *shipimage;
	ProjectileManager *ProjMngr;
	
	int framesSinceLastShot;

    float posx, posy;
    float velx, vely;
    
    Uint8 *keystates;

public:
    //Specify image filename
    Player(string imagefn)
    {
		shipimage = load_image( imagefn );
		posx = 5.0f, posy = 320.0f - 32.0f;
		velx = 0.0f, vely = 0.0f;
		
		keystates = SDL_GetKeyState( NULL );
		
		ProjMngr = new ProjectileManager;
			
		framesSinceLastShot = data.getRateOfFire();
    }

    ~Player()
    {
    	delete ProjMngr;
		SDL_FreeSurface( shipimage );
    }
    
    void Update()
    {
    	keystates = SDL_GetKeyState( NULL );
    	
    	float velchange = data.getMoveSpeed();
    	float maxvel = data.getMoveSpeed();
    	
    	velx *= 0.95f;
    	vely *= 0.95f;
    	
    	if ( keystates[ SDLK_UP ] )
    		vely -= velchange;
		
		if ( keystates[ SDLK_DOWN ] )
			vely += velchange;
			
		if ( keystates[ SDLK_LEFT ] )
			velx -= velchange;
			
		if ( keystates[ SDLK_RIGHT ] )
			velx += velchange;
			
		//Cap velocity 
		if (velx > maxvel)
			velx = maxvel;
		if (velx < -maxvel)
			velx = -maxvel;
		
		if (vely > maxvel)
			vely = maxvel;
		if (vely < -maxvel)
			vely = -maxvel;
	
		//Adjust position based on velocity;
		posx += velx;
		posy += vely;
	
		//Boundaries
		if ( posx < 5 ) { posx = 5; velx = 0; }
		if ( posx > 640 - 133 ) { posx = 640 - 133; velx = 0; }
		
		if ( posy < 5 ) { posy = 5; vely = 0; }
		if ( posy > 480 - ( 32 + 69 ) ) { posy = 480 - ( 32 + 69 ); vely = 0; }
		
		framesSinceLastShot++;
		
		if ( keystates[ SDLK_z ] && framesSinceLastShot >= data.getRateOfFire() )
		{
			int bposmult = 1;
			int xplus = 0;
			int yplus = 0;
			int distance = 0;
			int xdistance = -20;
			for ( int i = 0; i < data.getBulletCount(); i++ )
			{
				if ( data.getBulletCount() % 2 == 0 )
				{
					distance = 18;
					
					xplus = (bposmult - 1) * xdistance;
					if ( i % 2 == 0 )
					{
						yplus = -bposmult * distance;
					}
					else
					{
						yplus = bposmult * distance;
						bposmult++;
					}
					
				}
				else
				{
					distance = 20;
					if ( i == 0 )
					{
						bposmult = 0;
						yplus = 0;
					}
					else if ( i % 2 == 1 )
					{
						bposmult++;
						yplus = bposmult * distance;
					}
					else if ( i % 2 == 0 )
					{
						yplus = -bposmult * distance;
					}
					xplus = (bposmult - 1) * xdistance;
				}
				
				ProjMngr->AddProjectile( posx + 75 + xplus, posy + 18 + yplus, data.getLaser() );
				data.incExp( 1 );
			}
			framesSinceLastShot = 0;
			
		}
		
		ProjMngr->Update();
	}
	
	void Draw( SDL_Surface *apply_s)
	{
		apply_surface( posx, posy, shipimage, apply_s );
		ProjMngr->Draw( apply_s );
	}
	
	float getPositionX() { return posx; }
	float getPositionY() { return posy; }
	int getProjectileCount() { return ProjMngr->getCount(); }
	
	float getExpPercent()
	{
		return ( (float)data.getCurrentExp() / (float)data.getRequiredExp() ) * 100;
	}
};
