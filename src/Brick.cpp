/**
*	Brick declarations

*	@author Ramon Molla
*	@version 2012-06
*/

#include <Global.h>
#include <GameSounds.h>
#include <GameCharacters.h>

void CBrick::Init()
{
	Alive				=	true;	// Every ship is alive
	Health = MaxHealth	=	CB_HEALTH_DEFAULT;
	Hit_duration		=	CB_MAX_MAX_HIT_DUR;
	Type				=	CHARS_BRICK;
	SubType				=	CB_DEFAULT;	

	Size.v[XDIM]			=	0.05f;	// Width
	Size.v[YDIM]			=	0.05f;	// Height
	Size.v[ZDIM]			=	0.0f;

	///Sounds initialization
	Sound = &SoundsManager.Sound[CGS_TOUCH];

#ifdef CHAR_USE_AABB
	///Since Brick PhysicMode is UGKPHY_PHYSICAL
	InitializeAABB();
#endif
}

///Character AI virtual methods rewritten
void CBrick::AI_Dye(void)
{
	Active = Alive = false;
}

///Virtual procedure overwritten
void CBrick::AI_Init(void)
{
}

///For physics
void CBrick::Collided (CCharacter *CollidedChar)
{	
	
	if (Active)
	{
		switch (CollidedChar->Type)
		{
		case CHARS_MISSIL:			//Any shoot from the ships, supplyship or circleship
		case CHARS_PLAYER_SHOOT:	//Any shoot from the player			
			SoundsManager.Play(CGS_TOUCH,UGKSND_LOW_VOLUME);
			AI_Dye();
			break;
		}
	}	//If brick active
}

void CBrick::Render(void)
{
	if (Active)
		switch(RenderMode)
		{
		case CHAR_NO_RENDER:			//No render for this character: camera, collision objects,...
			return;
		case CHAR_2D:
			glPushMatrix();
			glTranslatef (Position.v[XDIM], Position.v[YDIM], Position.v[ZDIM]);

			glBegin (GL_QUADS);					// callList essayé => Time.fps / 2. à voir plus en détails...
				glTexCoord2f (0,1);	glVertex3f (-0.05f, -0.05f, .05f);  // -.05 pour éviter le z-fighting
				glTexCoord2f (0,0);	glVertex3f (-0.05f,  0.05f, .05f);
				glTexCoord2f (1,0);	glVertex3f ( 0.05f,  0.05f, .05f);
				glTexCoord2f (1,1);	glVertex3f ( 0.05f, -0.05f, .05f);
			glEnd ();

			glPopMatrix();

			//Drawing red square
			/*glColor4f (1.0f, 0.0f, 0.0f, 1.0f);
			glBegin (GL_LINES);
				glVertex3f (Position.v[XDIM]-Size.v[XDIM], Position.v[YDIM]-Size.v[YDIM], .05f);
				glVertex3f (Position.v[XDIM]-Size.v[XDIM], Position.v[YDIM]+Size.v[YDIM], .05f);
				glVertex3f (Position.v[XDIM]+Size.v[XDIM], Position.v[YDIM]+Size.v[YDIM], .05f);
				glVertex3f (Position.v[XDIM]+Size.v[XDIM], Position.v[YDIM]-Size.v[YDIM], .05f);
			glEnd ();*/

			
			break;
		case CHAR_LINES3D:
		case CHAR_3D:
			// correct a pixel bug which display a Brick anywhere - needs to be fixed
			if (Position.v[ZDIM] != 0.05f || Position.v[YDIM] > -3.00f)
				Active = false;

			glBegin (GL_QUADS);
				glTexCoord2f (0,1);	glVertex3f (Position.v[XDIM]-0.05f, Position.v[YDIM]-0.05f, Position.v[ZDIM]+.05f);
				glTexCoord2f (0,0);	glVertex3f (Position.v[XDIM]-0.05f, Position.v[YDIM]+0.05f, Position.v[ZDIM]+.05f);
				glTexCoord2f (1,0);	glVertex3f (Position.v[XDIM]+0.05f, Position.v[YDIM]+0.05f, Position.v[ZDIM]+.05f);
				glTexCoord2f (1,1);	glVertex3f (Position.v[XDIM]+0.05f, Position.v[YDIM]-0.05f, Position.v[ZDIM]+.05f);
			glEnd ();
			break;
		default: return;
		}
}