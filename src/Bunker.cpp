/**	Definition of the class Bonus

	Prefix: CBK_

	@author Ramon Molla
	@version 2011-07
*/

#include "Global.h"

char *CBNK_TypeNames[CBNK_MAXTYPE] =
{
	"BLANK",
	"HOME",
	"USER"
};

/// Bunker class default initialization
void CBunker::Init (void)
{
	Vector Pos;
	Pos.Reset();
	Size.v[XDIM]=CBNK_BRICK_WIDTH*CBNK_MAX_COLUMNS;
	Size.v[YDIM]=CBNK_BRICK_HEIGHT*CBNK_MAX_LINES;
	Init(CBNK_BLANK, Pos);

	FileAvailable = ColumnAvailable = 0;	/// Pointer to the current available file and column. For initialization purpouses

	///A bunker has no physical representation. Every brick it has is visible, but not the bunker
	PhysicMode	= UGKPHY_PHANTOM; ///So, no InitializeAABB() is required
	Type		= CHARS_BUNKER;
};

/**
*	Performs a complete line of bricks at the position Pos
*	\param line	The horizontal line of bricks to write on the screen
*	\param Pos	Position where to start the line of bricks for the bunker
*/
void CBunker::CreateBrickLine(unsigned int line, Vector &Pos)
{

	////////////////////////////////POR FINALIZAR
	
	for (unsigned int i = 0; i<CBNK_MAX_COLUMNS;i++)
	{
		//Brick[line][i].Init();
		Brick[line][i].Active = true;
		Brick[line][i].Column=i;
		Brick[line][i].Row=line;
		//Brick[line][i].MoveTo(Vector(
		//QuadtreeRoot::Instance()->getQuadtree()->AddCharacter(&Brick[line][i]);
	}

};

void CBunker::TranslateBunker	(Vector &Pos){

	Vector diff=Vector();

	Active=true;
	Alive=true;
	
	diff.v[XDIM]= Pos.v[XDIM]==FLT_MAX?0:Pos.v[XDIM]-Position.v[XDIM];
	diff.v[YDIM]= Pos.v[YDIM]==FLT_MAX?0:Pos.v[YDIM]-Position.v[YDIM];
	diff.v[ZDIM]= Pos.v[ZDIM]==FLT_MAX?0:Pos.v[ZDIM]-Position.v[ZDIM];
	

	Position.v[XDIM]= Pos.v[XDIM]==FLT_MAX?Position.v[XDIM]:Pos.v[XDIM];
	Position.v[YDIM]= Pos.v[YDIM]==FLT_MAX?Position.v[YDIM]:Pos.v[YDIM];
	Position.v[ZDIM]= Pos.v[ZDIM]==FLT_MAX?Position.v[ZDIM]:Pos.v[ZDIM];

	for(int row=0;row<CBNK_MAX_LINES;row++)
		for(int col=0;col<CBNK_MAX_COLUMNS;col++)
			Brick[row][col].Position += diff;
}

///Performs a default Bunker at position Pos
void CBunker:: Init(CBNK_BUNKER_TYPE Type, Vector &Pos)
{
	Vector BricksPosition;
	unsigned int row, col;

	
	// Every bunker is 14 bricks Width and 10 lines high


	//Put the auxiliar vector BricksPosition at the upper left corner
	BricksPosition.Set(Pos.v[XDIM]-(Size.v[XDIM]/2.f)+(CBNK_BRICK_WIDTH/2.f),
					   Pos.v[YDIM]+(Size.v[YDIM]/2.f)-(CBNK_BRICK_HEIGHT/2.f),
					   0.05f);

	//QuadtreeRoot::Instance()->getQuadtree()->AddCharacter(this);

	switch (Type)
	{
	case CBNK_BLANK:
	case CBNK_HOME:
	

		for(row=0;row<CBNK_MAX_LINES;row++){
			for(col=0;col<CBNK_MAX_COLUMNS;col++){
			
				Brick[row][col].Active = Brick[row][col].Alive = false;
				Brick[row][col].Health = 1.0f;
				Brick[row][col].Position.Set(BricksPosition.v[XDIM],
											 BricksPosition.v[YDIM],
											 0.05f);
				BricksPosition.v[XDIM] += CBNK_BRICK_WIDTH;
			}

			BricksPosition.v[XDIM] = Pos.v[XDIM]-(Size.v[XDIM]/2.f)+(CBNK_BRICK_WIDTH/2.f);
			BricksPosition.v[YDIM] -= CBNK_BRICK_HEIGHT;			
		}

		Position.Set(Pos.v[XDIM],
					 Pos.v[YDIM],
					 Pos.v[ZDIM]);

		Size.v[XDIM]=CBNK_BRICK_WIDTH*CBNK_MAX_COLUMNS;
		Size.v[YDIM]=CBNK_BRICK_HEIGHT*CBNK_MAX_LINES;

	}

	///A bunker has no physical representation. Every brick it has is visible, but not the bunker
	PhysicMode = UGKPHY_PHANTOM; ///So, no InitializeAABB() is required

};

void CBunker::Init(float x, float y, float z)
{
	Vector Pos;

	Pos.Set(x, y, z);

	Type = CHARS_BUNKER;

	Init (CBNK_HOME, Pos);
}

//////////////////////////////////////////////////////////////////////////////////////
//
/// Initialization of a bunker following a given pattern
//
//
void CBunker::Init(Vector &Pos, bool BrickMatrix[CBNK_MAX_LINES][CBNK_MAX_COLUMNS])
{
	int		row, col;
	Vector	P = Pos; 
	PositionPrev=Position;
	P.v[YDIM] += 0.1f;
	//QuadtreeRoot::Instance()->getQuadtree()->AddCharacter(this);


	for (row=0;row<CBNK_MAX_LINES;row++)
	{
		P.v[XDIM] = Pos.v[XDIM] - 0.02f;	//X reset
		P.v[YDIM] -= 0.01f;				//New Y position
		for(col=0;col<CBNK_MAX_COLUMNS;col++)
		{
			Brick[row][col].Active = BrickMatrix[row][col];
			Brick[row][col].MoveTo(P);
		} //Inner for
	}//External  for
	
	//A bunker has no physical representation. Every brick it has is visible, but not the bunker
	PhysicMode = UGKPHY_PHANTOM; //So, no InitializeAABB() is required
}

///Moving to an absolute position. It does not matter where the bunker is
void CBunker::MoveBricksTo (Vector &Move)
{
	/*
	int	row, col;
	Vector RelMove;

	RelMove.v[XDIM] = Move.v[XDIM] - Position.v[XDIM];
	RelMove.v[YDIM] = Move.v[YDIM] - Position.v[YDIM];
	RelMove.v[ZDIM] = Move.v[ZDIM] - Position.v[ZDIM];

	MoveTo(Move);

	for (row=0;row<CBNK_MAX_LINES;row++)
		for(col=0;col<CBNK_MAX_COLUMNS;col++)
			Brick[row][col].MoveRelTo(RelMove);
			*/

}

void CBunker::Render(void)
{
	unsigned int row, col;

	switch(RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character
		return;
	case CHAR_2D:
	case CHAR_LINES3D:
	case CHAR_3D:
		if (Alive)
		{
			glDisable(GL_BLEND);
			glEnable (GL_TEXTURE_2D);
			glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

			switch(RenderMode)
			{
			case CHAR_NO_RENDER:			//No render for this character
				break;
			case CHAR_2D:
			case CHAR_LINES3D:
				TexturesManager.Textures[CTM_BRICK]->SetTexture();
				break;
			case CHAR_3D:
				if (6 == Game.Level)	//ACHTUNG: This exception has to be performed in the Level6.html file, not in source code!!!
					TexturesManager.Textures[CTM_BRICK]->SetTexture();
				else
					TexturesManager.Textures[CTM_BRICK3D]->SetTexture();
				break;
			default: break;
			}

			
			
//			glColor4f (1.0f, 1.0f, 1.0f, 1.0f);


			Alive = false;
			for (row=0;row<CBNK_MAX_LINES;row++)
				for(col=0;col<CBNK_MAX_COLUMNS;col++)
				{
					Brick[row][col].Render();
					Alive = true;
				}


			/*glColor4f (1.0f, 0.0f, 1.0f, 1.0f);
			glBegin (GL_LINE_STRIP);
				glVertex3f (Position.v[XDIM]-(Size.v[XDIM]/2.f),	Position.v[YDIM]+(Size.v[YDIM]/2.f), .05f);
				glVertex3f (Position.v[XDIM]-(Size.v[XDIM]/2.f),	Position.v[YDIM]-(Size.v[YDIM]/2.f), .05f);
				glVertex3f (Position.v[XDIM]+(Size.v[XDIM]/2.f),	Position.v[YDIM]-(Size.v[YDIM]/2.f), .05f);
				glVertex3f (Position.v[XDIM]+(Size.v[XDIM]/2.f),	Position.v[YDIM]+(Size.v[YDIM]/2.f), .05f);
				glVertex3f (Position.v[XDIM]-(Size.v[XDIM]/2.f),	Position.v[YDIM]+(Size.v[YDIM]/2.f), .05f);

			glEnd ();
			*/


			glEnable(GL_BLEND);
		}
		break;
	default: return;
	}
}

void CBunker::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	unsigned int row, col;

	RenderMode = Mode;

	//Bunkers
	for (row=0;row<CBNK_MAX_LINES;row++)
		for(col=0;col<CBNK_MAX_COLUMNS;col++)
			if (Brick[row][col].Active)	Brick[row][col].ChangeRenderMode(RenderMode);
}

//Physics
void CBunker::Collided (CCharacter *CollidedChar)
{
	int row, col;

	if(CollidedChar->Type==CHARS_PLAYER_SHOOT){
	
		col=((CollidedChar->Position.v[XDIM]-(Position.v[XDIM]-(Size.v[XDIM]/2.f)))/CBNK_BRICK_WIDTH);
			
		for(row=CBNK_MAX_LINES-1;row>=0;row--){
		
			if(Brick[row][col].Alive){
				Brick[row][col].Type=CHARS_BRICK;
				CollidedChar->Collided(&(CCharacter)Brick[row][col]);
				Brick[row][col].Collided(CollidedChar);
				return;
			}
		
		}


	}else if(CollidedChar->Type==CHARS_MISSIL){
	
		col=((CollidedChar->Position.v[XDIM]-(Position.v[XDIM]-(Size.v[XDIM]/2.f)))/CBNK_BRICK_WIDTH);
			
		for(row=0;row<CBNK_MAX_LINES;row++){
		
			if(Brick[row][col].Alive){
				Brick[row][col].Type=CHARS_BRICK;
				CollidedChar->Collided(&(CCharacter)Brick[row][col]);
				Brick[row][col].Collided(CollidedChar);
				return;
			}
		
		}

	}
	else{
		//Detects which brick has been touched
		for (row=0;row<CBNK_MAX_LINES;row++)
			for (col=0;col<CBNK_MAX_COLUMNS;col++)
				if (Brick[row][col].Alive)
					if (CollisionDetector.Collided(&Brick[row][col], CollidedChar)) return ;
	}
	//CollidedChar->Alive=true;
}

#ifdef CHAR_USE_AABB
///Recalculates its AABB after traversing all the alive bricks
void CBunker::UpdateAABB()
{
	unsigned int file, col;

	for (file=0;file<CBNK_MAX_LINES;file++)
		for (col=0;col<CBNK_MAX_COLUMNS;col++)
			if (Brick[file][col].Alive)
				UpdateAABB(Brick[file][col]);

}

///Recalculates its AABB given a brick
void CBunker::UpdateAABB(CBrick Brick)
{
	unsigned int i;

	for (i=XDIM;i<UGKALG_DIMENSION;i++)
	{
		if (AABB[CHAR_BBMIN][i].Value > Brick.AABB[CHAR_BBMIN][i].Value)
			//Update min value on this dimension
			AABB[CHAR_BBMIN][i].Value = Brick.AABB[CHAR_BBMIN][i].Value;

		if (AABB[CHAR_BBMAX][i].Value < Brick.AABB[CHAR_BBMAX][i].Value)
			//Update max value on this dimension
			AABB[CHAR_BBMAX][i].Value = Brick.AABB[CHAR_BBMAX][i].Value;
	}
}
#endif