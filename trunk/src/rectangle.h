class Rectangle
{
private:
	float xpos;
	float ypos;
	float width;
	float height;
public:
	Rectangle( float x, float y, float w, float h )
	{
		xpos = x;
		ypos = y;
		width = w;
		height = h;
	}
	
	bool PointInside( float px, float py )
	{
		if ( ( px <= xpos + width ) && ( px >= xpos ) )
			if ( ( py <= ypos + height ) && ( py >=  ypos ) )
				return true;
				
		return false;
	}
	
	float GetXPos() { return xpos; }
	float GetYPos() { return ypos; }
	float GetWidth() { return width; }
	float GetHeight() { return height; }
	
	bool Intersects( Rectangle *r )
	{
		float r1addx = xpos + width;
		float r1addy = ypos + height;
		float r2addx = r->GetXPos() + r->GetWidth();
		float r2addy = r->GetYPos() + r->GetHeight();
		
		if ( ( r2addx - r1addx < r1addx ) && ( r2addx - r1addx > -r1addx ) )
			if ( ( r2addy - r1addy < r1addy ) && ( r2addy - r1addy > -r1addy ) )
				return true;
				
		return false;
	}
};
