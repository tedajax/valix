#include "projectile.h"

class ProjectileManager
{
private:
	LinkedList<Projectile> *ProjectileList;

	void CleanUp()
	{
		if ( ProjectileList->GetCount() > 1 )
		{
			for ( int i = 0; i < ProjectileList->GetCount() - 1; i++ )
			{
				if ( ProjectileList->GetNode( i )->variable->Destroy() )
					ProjectileList->RemoveNode( i );
			}
		}
		else if ( ProjectileList->GetCount() == 1 )
		{
			if ( ProjectileList->GetNode( 0 )->variable->Destroy() )
				ProjectileList->RemoveNode( 0 );
		}
	}

public:
	ProjectileManager()
	{
		ProjectileList = new LinkedList<Projectile>;
	}	
	
	~ProjectileManager()
	{
		ProjectileList->~LinkedList();
	}
	
	void AddProjectile( Projectile *p )
	{
		ProjectileList->AddNode( p );
	}
	
	void AddProjectile( int px, int py, bool l )
	{
		Projectile *newprojectile;
		newprojectile = new Projectile( px, py, l );
		ProjectileList->AddNode( newprojectile );
	}
	
	void Update()
	{
		for ( int i = 0; i <= ProjectileList->GetCount() - 1; i++ )
		{		
			ProjectileList->GetNode( i )->variable->Update();
		}
		
		CleanUp();
	}
	
	void Draw( SDL_Surface *apply_s )
	{
		for ( int i = 0; i <= ProjectileList->GetCount() - 1; i++ )
		{
			ProjectileList->GetNode( i )->variable->Draw( apply_s );
		}
	}
	
	int getCount() { return ProjectileList->GetCount(); }
};
