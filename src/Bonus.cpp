#include "Global.h"
#include <Textures\TexturesManager.h>
#include <UGKPhysics.h>

CBonus::CBonus()
{
	Init();
	//Since Bonus PhysicMode is UGKPHY_PHYSICAL... it has to initialize its AABB
#ifdef CHAR_USE_AABB
	InitializeAABB();
#endif
}

void CBonus::Init()
{
	Explosion.Life	= 4;
	Explosion.Alive	= false;
	Explosion.Active= false;
	
	Active = false;
	Alive  = false;

	LastUpdTime = 0LL;

	//Initial position
	Position.v[XDIM] = float (rand()%10) - 5.0f;
	Position.v[YDIM] = CBN_YPOS;
	Position.v[ZDIM] = CBN_ZPOS;

	//Initial size
	Size.v[XDIM] = CBN_WIDTH_2D;
	Size.v[YDIM] = CBN_HEIGTH_2D;
	Size.v[ZDIM] = Position.v[ZDIM];

	Speed.Reset();
	Speed.v[YDIM] = -0.003f;

	//Do not use Moveto method to avoid to be inserted in the collision detection facilities
	Shake1 = 1;
	Shake2 = 0;

	Alpha		=  CBN_MAX_ALPHA;
	FadingSpeed = -CBN_FADING_SPEED;

	Type	=	CHARS_BONUS;	///<Class of character is a Bonus
	SubType =	CBN_BONUS_3D;	///<When initializing the type of bonus, the first time is 3D

	msgUpd = new RTDESK_CMsg;
	msgUpd->Type = RTDESKMSG_BASIC_TYPE;
	msgUpd->Propietary = true;
}

void CBonus::Render(void)
{
	switch(RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character. Nothing to do
		return;
	case CHAR_2D:
		if (CBN_BONUS_3D != SubType)
		{
			//Not possible to render in 2D mode a non CBN_BONUS_3D Bonus
			//Bonus not allowed. Destroy bonus
			Alive = Active = false;
			return;
		}
		//Render Bonus to change from 2D mode to 3D. No other posibilities
		glPushMatrix();
		//glEnable(GL_BLEND);		
			TexturesManager.Textures[CTM_POWERUP]->SetTexture();
			glBegin (GL_QUADS);
				glTexCoord2f (0,1);	glVertex2f (Position.v[XDIM] - Size.v[XDIM]/2.f, Position.v[YDIM] + Size.v[YDIM]/2.f);
				glTexCoord2f (0,0);	glVertex2f (Position.v[XDIM] - Size.v[XDIM]/2.f, Position.v[YDIM] - Size.v[YDIM]/2.f);
				glTexCoord2f (1,0);	glVertex2f (Position.v[XDIM] + Size.v[XDIM]/2.f, Position.v[YDIM] - Size.v[YDIM]/2.f);
				glTexCoord2f (1,1);	glVertex2f (Position.v[XDIM] + Size.v[XDIM]/2.f, Position.v[YDIM] + Size.v[YDIM]/2.f);
			glEnd ();
		//glDisable(GL_BLEND);
		glPopMatrix();
		break;
	case CHAR_LINES3D:
		//Nothing to do at this version
		break;
	case CHAR_3D:
		switch (SubType)
		{
		case CBN_BONUS_3D:
			//Not possible to render in 3D mode a CBN_BONUS_3D Bonus
			//Bonus not allowed. Destroy bonus
			Alive = Active = false;
			return;
		case CBN_BONUS_LASER:
			TexturesManager.Textures[CTM_TEXT_BONUS]->SetTexture();
			break;
		case CBN_BONUS_WEAPON:
			TexturesManager.Textures[CTM_TEXT_WEAPON]->SetTexture();
		}

		//Fading in and out of the Bonus
		Alpha += FadingSpeed * msUpdMsg;
		if (Alpha <= 0)
			if (FadingSpeed <=0 ) FadingSpeed = -FadingSpeed;
		else if (Alpha >= CBN_MAX_ALPHA)
			if (FadingSpeed >=0 ) FadingSpeed = -FadingSpeed;

		//glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		
		glColor4f (1.0f, 1.0f, 1.0f, Alpha);

		glPushMatrix();
			glBegin (GL_QUADS);
				glTexCoord2f (0,1);	glVertex2f (Position.v[XDIM] - Size.v[XDIM]/2.f, Position.v[YDIM] + Size.v[YDIM]/2.f);
				glTexCoord2f (0,0);	glVertex2f (Position.v[XDIM] - Size.v[XDIM]/2.f, Position.v[YDIM] - Size.v[YDIM]/2.f);
				glTexCoord2f (1,0);	glVertex2f (Position.v[XDIM] + Size.v[XDIM]/2.f, Position.v[YDIM] - Size.v[YDIM]/2.f);
				glTexCoord2f (1,1);	glVertex2f (Position.v[XDIM] + Size.v[XDIM]/2.f, Position.v[YDIM] + Size.v[YDIM]/2.f);
			glEnd ();
		glPopMatrix();

		//glDisable(GL_BLEND);

		break;
	default: return;
	}
}

void CBonus::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	RenderMode = Mode;

	switch(Mode)
	{
	case CHAR_NO_RENDER:			//No render for this character: camera, collision objects,...
		break;
	case CHAR_2D:
		Size.v[XDIM] = CBN_WIDTH_2D;
		Size.v[YDIM] = CBN_HEIGTH_2D;
		break;
	case CHAR_LINES3D:
	case CHAR_3D:
		Size.v[XDIM] = CBN_WIDTH_3D;
		Size.v[YDIM] = CBN_HEIGTH_3D;
		break;
	default: return;
	}
	
}

void CBonus::Update (void)	///What the character has to do on every time tick 
{
	
	RTDESK_TIME nowTime, elapTime;

	nowTime = Game.TimerManager.Timers[TmrApp].TicksUntilNow();
	elapTime = nowTime - LastUpdTime;
	if(elapTime >= ticksUpdkMsg){

#ifndef DEF_CONTINUOUS_WITH_SIMULATE_TIME
		LastUpdTime = nowTime;
		//Move downwards one step
		Position.v[YDIM] += Speed.v[YDIM] * msUpdMsg;

		if (Position.v[YDIM] < CSIG_PG_BOTTOM)						//Out of game playground
			AI_Dye();

		#ifdef CHAR_USE_AABB
		UpdateAABB();
		#endif
#else
		bool SynWithRealTime = false;
		LastUpdTime += ticksUpdkMsg;
		while(!SynWithRealTime){
			//Move downwards one step
			Position.v[YDIM] += Speed.v[YDIM] * msUpdMsg;
			#ifdef CHAR_USE_AABB
			UpdateAABB();
			#endif

			nowTime = Game.TimerManager.Timers[TmrApp].TicksUntilNow();
			elapTime = nowTime - LastUpdTime;
			if(elapTime >= 0LL) 
				SynWithRealTime=true;
			else
				SynWithRealTime=false;
		}
#endif
	}
}

void CBonus::DiscreteUpdate (void)	///What the character has to do on every time tick 
{
	#ifdef DEF_UPD_TIME
	Game.TimerManager.Timers[TmrUpd].InitCounting();
	#endif

	//Move downwards one step
	Position.v[YDIM] += Speed.v[YDIM] * msUpdMsg;

	if (Position.v[YDIM] < CSIG_PG_BOTTOM)						//Out of game playground
		AI_Dye();

	#ifdef CHAR_USE_AABB
	UpdateAABB();
	#endif

	#ifdef DEF_UPD_TIME
	EndAccCounting(TmrUpd);
	#endif


	if(Alive && !endByTime && !endByFrame){
		#ifdef DEF_RTD_TIME
		Game.TimerManager.Timers[TmrRTDSKMM].InitCounting();
		#endif

		SendSelfMsg(msgUpd, ticksUpdkMsg);

		#ifdef DEF_RTD_TIME
		EndAccCounting(TmrRTDSKMM);
		#endif
	}
}

void CBonus::AI_Explode (void)	//What to do when exploding a Bonus
{
	Explosion.Init(this);
	Explosion.Alive = true;
	Explosion.Active = true;

	AI_Dye();
}

//Physics
void CBonus::Collided (CCharacter *CollidedChar){if (CHARS_PLAYER == CollidedChar->Type) PlayerCollided();}


void CBonus::PlayerCollided ()
{
	if (Alive && !Explosion.Alive)
		AI_Explode();

	switch(SubType)
	{
	case CBN_BONUS_3D:
		SoundsManager.Play(CGS_SHIELD,UGKSND_VOLUME_80);
		SoundsManager.SetPosition(CGS_SHIELD, UGKSND_STRD_POSITION);
		break;
	case CBN_BONUS_LASER:
	case CBN_BONUS_WEAPON:
		SoundsManager.Play(CGS_BONUS,UGKSND_MAX_VOLUME);
		SoundsManager.Play(CGS_BONUS,UGKSND_MAX_VOLUME);
		break;
	}

	AI_Dye();
}

void CBonus::ReceiveMessage(RTDESK_CMsg *pMsg){
	DiscreteUpdate();
}
