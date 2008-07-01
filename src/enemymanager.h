class EnemyManager
{
private:
	LinkedList<Enemy> *EnemyList;
	
	void CleanUp()
	{
		if ( EnemyList->GetCount() > 1 )
		{
			for ( int i = 0; i < EnemyList->GetCount() - 1; i++ )
			{
				if ( EnemyList->GetNode( i )->variable->Destroy() )
					EnemyList->RemoveNode( i );
			}
		}
		else if ( EnemyList->GetCount() == 1 )
		{
			if ( EnemyList->GetNode( 0 )->variable->Destroy() )
				EnemyList->RemoveNode( 0 );
		}
	}
	
public:
	EnemyManager()
	{
		EnemyList = new LinkedList<Enemy>();
		//EnemyBullets = new ProjectileManager();
	}
	
	~EnemyManager()
	{
		delete EnemyList;
	}
	
	void AddEnemy( Enemy *e )
	{
		EnemyList->AddNode( e );
	}
	
	void AddEnemy( short t, float px, float py )
	{
		Enemy *e;
		e = new Enemy( t, px, py );
		
		EnemyList->AddNode( e );
	}
	
	void AddEnemy( short t, float px, float py, float vx, float vy )
	{
		Enemy *e;
		e = new Enemy( t, px, py, vx, vy );
		
		EnemyList->AddNode( e );
	}
	
	void Update()
	{
		for ( int i = 0; i <= EnemyList->GetCount() - 1; i++ )
		{
			EnemyList->GetNode( i )->variable->Update();
		}
		
		CleanUp();
	}
	
	void Draw( SDL_Surface *apply_s )
	{
		for ( int i = 0; i <= EnemyList->GetCount() - 1; i++ )
		{
			EnemyList->GetNode( i )->variable->Draw( apply_s );
		}
	}
	
	int getCount() { return EnemyList->GetCount(); }
};
