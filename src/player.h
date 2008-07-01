#include <math.h>

//This class stores player statistics
//as well as skills, experience, etc...
class PlayerData
{
private:
    int level; //current experience level
    int totalexp; //Total exp ever gained
    int currentexp; //current exp for the current level
	int totalpoints; //total skillpoints ever gained
    int skillpoints; //skillpoints remaining after using skillpoints

    int rateoffire; //rate of fire measured in frames
    int bulletcount; //number of bullets to fire at one time, code automatically spaces them
    bool laser; //if false lasers destroyed on impact, else penetrate enemies

	float movespeed; //how quickly the player moves based on input

    int requiredexp; //required exp for next level

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

	//Series of accessors and mutators
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

	//adjusts multiple values and potentially levelup
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


//Handles player updating, game logic, drawing, etc...
class Player
{
private:
	//data storage class
    PlayerData data;
	//image for representing player
    SDL_Surface *shipimage;
	//Manages projectiles from the player
	ProjectileManager *ProjMngr;
	
	//For counting the frames between each shot
	//used to maintain rate of fire specified in player data class
	int framesSinceLastShot;

	//define position and velocity values
    float posx, posy;
    float velx, vely;
    
	//for checking input
    Uint8 *keystates;

public:
    //Specify image filename
    Player(string imagefn)
    {
		//load image position player to left side of screen in the middle
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
		//update inpute
    	keystates = SDL_GetKeyState( NULL );
    	
    	float velchange = data.getMoveSpeed();
    	float maxvel = data.getMoveSpeed();
    	
    	velx *= 0.95f;
    	vely *= 0.95f;
    	
		//Simply change velocity based on key input
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
		
		//increase counter
		framesSinceLastShot++;
		
		//If shoot key (in this case z) and framessinchlastshot at the rate of fire
		//create a bullet launch it
		if ( keystates[ SDLK_z ] && framesSinceLastShot >= data.getRateOfFire() )
		{
			int bposmult = 1;
			int xplus = 0;
			int yplus = 0;
			int distance = 0;
			int xdistance = -20;
			for ( int i = 0; i < data.getBulletCount(); i++ )
			{
				//For determining how to position multiple lasers at once
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
				
				//Add projectiles to the manager
				ProjMngr->AddProjectile( posx + 75 + xplus, posy + 18 + yplus, data.getLaser() );
				data.incExp( 1 ); //testing exp bar
			}

			//reset the frame counter
			framesSinceLastShot = 0;
		}
		
		//Update the projectile manager
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
	
	//Gets percentage of exp for current level so a box can  be drawn for the exp bar
	float getExpPercent()
	{
		return ( (float)data.getCurrentExp() / (float)data.getRequiredExp() ) * 100;
	}
};
