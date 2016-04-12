/*/////////////////////////////////////////////////////////////////////////////////////
//
// Player initialization or constructor
//
// These values are by default. They have to be overwritten in the initializatio phase
// when reading the default values from the "initialization.html" file

	@author Ramon Molla
	@version 2011-09-11

	*/
#include "Global.h"
#include "Player2.h"
#include <math.h>

#include <gl.h>
#include <glext.h>
#include "Bonus.h"

//Vector CP_REACTOR_REL_POS;

bool CPlayer2::Init ()	//Used when all the values are initialized by default
						//when reading the global initialization game file. 
						//Returns true if the initialization of the attirbute has been correctly done
{
	Type			=	CHARS_PLAYER2;
	Lives			=	CP_DEFAULT_LIVES;
	Health			=	MaxHealth = CP_MAX_HEALTH;
	Hit_duration	=	CP_MAX_MAX_HIT_DUR;

#ifdef CHAR_USE_AABB
	InitializeAABB();
#endif

	MoveTo(0.0f, -6.15f, 0.05f);	///This updates the AABB

	yi				=	0.00;
	yi_speed		=	0.01f;
	yi_cos			=	0;
	zi				=	0.00;
	zi_speed		=	0.01f;

	Size.v[XDIM]					= 0.6f;
	Size.v[YDIM]					= 0.4f;

	Speed.Reset();
	Acceleration.Reset();
	Speed.v[XDIM]		= CP_MAX_SPEED;
	Acceleration.v[XDIM] = CP_MAX_ACCEL;

	Alive = true;

	ShootType		=	CSH_PLAYER;
	RenderMode = CHAR_2D;

	ActivateReactor();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Player Initialisation 
//
//

CPlayer2::CPlayer2()
{
	Init();

	msg = new RTDESK_CMsg;
	msg->Type = RTDESKMSG_BASIC_TYPE;
	msg->Propietary = true;
}

//AI
void CPlayer2::AI_Init	(void)	///Default Artificial Intelligence character initialization
{
	State = CP_UNBORN;
}

void CPlayer2::AI_Dye (void)
{
		Active = Alive  = false;

}

void CPlayer2::AI_Explode (void)
{
	SoundsManager.Play(CGS_TOUCH,UGKSND_ULTRA_LOW_VOLUME);
	
	Explosion.Init(this);
	Explosion.Active=true;
}	

void CPlayer2::ActivateReactor()
{
	Reactor.Alive = Reactor.Active = true;
	Reactor.Position = Position;
}

void CPlayer2::ActivateLaser(CP_PLAYER_LASER LaserActivated)
{
	Laser[LaserActivated].Alive = Laser[LaserActivated].Active = true;
	Laser[LaserActivated].Explosion.Alive = false;
	
	if (CP_LEFT_LASER == LaserActivated)
		Laser[LaserActivated].Position.v[XDIM] = Position.v[XDIM] - Size.v[XDIM] - CP_LASER_REL_POS;
	else 
		Laser[LaserActivated].Position.v[XDIM] = Position.v[XDIM] + Size.v[XDIM] + CP_LASER_REL_POS;

	Laser[LaserActivated].Position.v[YDIM] = Position.v[YDIM];
	Laser[LaserActivated].Position.v[ZDIM] = Position.v[ZDIM];

	QuadtreeRoot::Instance()->getQuadtree()->ActualizeCharacter(&Laser[LaserActivated]);
}

//Physics
void CPlayer2::Collided (CCharacter *CollidedChar)
{
	switch (CollidedChar->Type)
	{
	case CHARS_SHIP:
	case CHARS_MISSIL:
		if (Alive && !Immortal && ((CShoot*)CollidedChar)->SubType!=CSH_PLAYER)
		{
			#ifdef DEBUG_MORTAL
			if (CSIG_PASSING_2_3D == Game.AI->GetState())	return;	// Stop checking more collisions. The player is protected while changing from mode 2D to mode 3D
			
			SoundsManager.Play(CGS_TOUCH,UGKSND_VOLUME_60);
			SoundsManager.SetPosition(CGS_TOUCH,NULL);

			if(CHAR_2D == RenderMode)
				Health  = 0;
			else
				Health -= CollidedChar->Health;

			Hit_duration--;

			if (Health <= 0)
			{			
				//... and everything has to explode, if exists
				if (Laser[CP_LEFT_LASER].Alive)
					Laser[CP_LEFT_LASER].AI_Explode();

				if (Laser[CP_RIGHT_LASER].Alive)
					Laser[CP_RIGHT_LASER].AI_Explode();
				
				SoundsManager.Play(CGS_EXPLOSION,UGKSND_VOLUME_80);
				Explosion.Init();
				Explosion.Active=true;
				AI_Dye();

				//Player loses its life...
				Game.GameEvent(CSIG_LIFE_LOST);	//v Controlado 4->7
			}
			#endif
		}
		break;
	case CHARS_BONUS:
		//These collisions are detected and managed by bonus manager when collided with the player.
		break;
	}
}

void CPlayer2::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	RenderMode = Mode;
	Explosion.ChangeRenderMode(Mode);
	Reactor.ChangeRenderMode(Mode);

	switch(RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character: camera, collision objects,...
		return;
	case CHAR_2D:
		Position.v[YDIM]			= CSIG_PLAYER_Y_POSITION;
		ShootType					= CSH_PLAYER;
		ShootSpeed					= CP_SHOOT2D_SPEED;

		//No laser support when in 2D mode
		Laser[CP_LEFT_LASER].Alive	= false;
		Laser[CP_RIGHT_LASER].Alive	= false;
		Laser[CP_LEFT_LASER].Active	= false;
		Laser[CP_RIGHT_LASER].Active= false;

		Reactor.Active = Reactor.Alive = false;

		Size.v[XDIM]					= CP_2D_SIZE_X;
		Size.v[YDIM]					= CP_2D_SIZE_Y;

		Speed.v[XDIM]				= CP_MAX_SPEED;	//There is no acceleration. Instant max speed when moving

		break;
	case CHAR_LINES3D:
	case CHAR_3D:
		//Position.v[YDIM]	= CSIG_PLAYER_Y_POSITION -.5f;
		MoveTo(Position.v[XDIM], CSIG_PLAYER_Y_POSITION -.5f, Position.v[ZDIM]);
		ShootType		= CSH_PLAYER3D;
		ShootSpeed		= CP_SHOOT3D_SPEED;

		Size.v[XDIM]		=	CP_3D_SIZE_X - Mesh->modelo.rot.z/250; // Mesh->rot.v[ZDIM] (de -48.xx à 48.xx) correspond au penchement du joueur donc sa taille projetée en 2D est plus faible (la gestion de collision est en 2D)
		Size.v[YDIM]		=	CP_3D_SIZE_Y;
		if(Mesh)
			if(Mesh->Loaded)
				FitMeshIntoBoundingBox(*Mesh);

		Laser[CP_LEFT_LASER].Size.v[XDIM] = Size.v[XDIM]/3;
		Laser[CP_LEFT_LASER].Size.v[YDIM] = Size.v[YDIM];
		Laser[CP_RIGHT_LASER].Size.v[XDIM] = Size.v[XDIM]/3;
		Laser[CP_RIGHT_LASER].Size.v[YDIM] = Size.v[YDIM];
		if(Laser[CP_LEFT_LASER].Mesh)
			if(Laser[CP_LEFT_LASER].Mesh->Loaded)
				Laser[CP_LEFT_LASER].FitMeshIntoBoundingBox(*Laser[CP_LEFT_LASER].Mesh);
		if(Laser[CP_RIGHT_LASER].Mesh)
			if(Laser[CP_RIGHT_LASER].Mesh->Loaded)
				Laser[CP_RIGHT_LASER].FitMeshIntoBoundingBox(*Laser[CP_RIGHT_LASER].Mesh);

		ActivateReactor();
		break;
	default: return;
	}

	Size.v[ZDIM]		=	0.0f;
#ifdef CHAR_USE_AABB
	UpdateAABB();
#endif
}

void CPlayer2::Render(void)
{
	// GAME 2D
	if (CHAR_2D == RenderMode)
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		TexturesManager.Textures[IndTexture2D]->SetTexture();

		// ALPHA TEST + BLEND
		glAlphaFunc(GL_GREATER, 0.3f);								// for TGA alpha test
		glEnable(GL_ALPHA_TEST);									// for TGA alpha test
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// for anti-aliasing

		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (Position.v[XDIM] - Size.v[XDIM]/2.f, Position.v[YDIM] + Size.v[YDIM]/2.f);
			glTexCoord2f (0,0);	glVertex2f (Position.v[XDIM] - Size.v[XDIM]/2.f, Position.v[YDIM] - Size.v[YDIM]/2.f);
			glTexCoord2f (1,0);	glVertex2f (Position.v[XDIM] + Size.v[XDIM]/2.f, Position.v[YDIM] - Size.v[YDIM]/2.f);
			glTexCoord2f (1,1);	glVertex2f (Position.v[XDIM] + Size.v[XDIM]/2.f, Position.v[YDIM] + Size.v[YDIM]/2.f);
		glEnd ();

		// BACK TO NON ALPHA TEST + PREVIOUS BLEND
		glDisable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	}
	else	// GAME 3D
	{

		// Player
		Mesh->modelo.rot.x =  -270.0f;
		Mesh->modelo.scaleY = 0.15f;
		Mesh->modelo.pos.x = Position.v[XDIM];

		if (CSIG_PASSING_2_3D != Game.AI->GetState())
			Mesh->modelo.pos.y = Position.v[YDIM];

		//Control to rotate Player
		Mesh->modelo.rot.z = -Scrolling * 11000;
		if(Mesh->modelo.rot.z > 50) Mesh->modelo.rot.z = 50;
		else if(Mesh->modelo.rot.z < -50) Mesh->modelo.rot.z = -50;  //[-50, 50]

		// Anti-aliasing (DEBUT)
		if (Application.Window.Antialiasing_active && CHAR_3D == RenderMode)
			glEnable(GL_MULTISAMPLE_ARB);

		// Player touched
		if (Hit_duration < 500)
		{
			Hit_duration -= 20 * Game.RenderTemp;
			if (Hit_duration <= 0)
				Hit_duration = 500;
			Mesh->modelo.Draw();	// vaisseau non texturé, blanc
		}
		
		// Normal player drawing
		else
			Mesh->modelo.Draw();

		// Anti-aliasing
		if (Application.Window.Antialiasing_active && CHAR_3D == RenderMode)
		glDisable(GL_MULTISAMPLE_ARB);

		if(Laser[CP_LEFT_LASER].Alive) Laser[CP_LEFT_LASER].Render();
		if(Laser[CP_RIGHT_LASER].Alive)Laser[CP_RIGHT_LASER].Render();

		//In 2D, no reactor has to be rendered
		if(CHAR_2D != RenderMode && Reactor.Active) Reactor.Render();
			
	}

	
}

///Geometric transformation methods
void CPlayer2::MoveTo		(float x, float y, float z)	///Moves to the absolute coordinate x,y,z
{
	Vector P;

	P.Set(x, y, z);

	MoveTo(P);
}

void CPlayer2::MoveTo		(Vector &Move)				///Moving to an absolute position. It does not matter where the character is

{
	Position = Move; 

#ifdef CHAR_USE_AABB
	///Updating the bounding box
	UpdateAABB ();
#endif
	

	//Left Laser position update
	if (Laser[CP_LEFT_LASER].Active){
		Laser[CP_LEFT_LASER].Position = Position;
		Laser[CP_LEFT_LASER].Position.v[XDIM] = Position.v[XDIM] - Size.v[XDIM] - CP_LASER_REL_POS;
		QuadtreeRoot::Instance()->getQuadtree()->ActualizeCharacter(&Laser[CP_LEFT_LASER]);
	}
	//Right Laser position update
	if (Laser[CP_RIGHT_LASER].Active){
		Laser[CP_RIGHT_LASER].Position = Position;
		Laser[CP_RIGHT_LASER].Position.v[XDIM] = Position.v[XDIM] + Size.v[XDIM] + CP_LASER_REL_POS;
		QuadtreeRoot::Instance()->getQuadtree()->ActualizeCharacter(&Laser[CP_RIGHT_LASER]);
	}
	
	Reactor.Position = Position;
}

void CPlayer2::MoveRelTo	(float x, float y, float z)	///Moves x,y,z units from the to the current position
{
	Vector P;

	P.Set(x, y, z);

	MoveRelTo(P);
}

void CPlayer2::MoveRelTo (Vector &Move)	///Moving relative to the character current Position
{
	///Adding relative movement
	Position						+= Move;

#ifdef CHAR_USE_AABB
	///Updating the bounding box
	UpdateAABB ();
#endif

	if (Laser[CP_LEFT_LASER].Active){
		Laser[CP_LEFT_LASER].Position = Position;
		Laser[CP_LEFT_LASER].Position.v[XDIM] = Position.v[XDIM] - Size.v[XDIM] - CP_LASER_REL_POS;
		QuadtreeRoot::Instance()->getQuadtree()->ActualizeCharacter(&Laser[CP_LEFT_LASER]);
	}
		
	if (Laser[CP_RIGHT_LASER].Active){
		Laser[CP_RIGHT_LASER].Position = Position;
		Laser[CP_RIGHT_LASER].Position.v[XDIM] = Position.v[XDIM] + Size.v[XDIM] + CP_LASER_REL_POS;
		QuadtreeRoot::Instance()->getQuadtree()->ActualizeCharacter(&Laser[CP_RIGHT_LASER]);
	}

	Reactor.Position = Position;
}

	//User interaction. One method for every player command sent to the player's avatar
void CPlayer2::ManageEvent	(unsigned int Event)
{
		switch (Event)
		{
		case CP_MOVE_LEFT:			///The player wants to move to the left
			UserLeft();
			break;
		case CP_MOVE_RIGHT:			///The player wants to move to the right
			UserRight();
			break;
		case CP_SHOOT:				///The player wants to fire a shoot
			if(!DiscreteSimulation) UserShoot();
			else DiscreteUserShoot();
			break;
		}
}

void CPlayer2::UserRight	()
{
	PositionPrev=Position;
	if (Alive)
	{
		//ACHTUNG: To do
		/*if (CHAR_3D == RenderMode)
		{
			Speed.v[XDIM] += Acceleration.v[XDIM] * msUpdPlayer;
			if (Speed.v[XDIM] > CP_MAX_SPEED)
				Speed.v[XDIM] = CP_MAX_SPEED;
			MoveRelTo(Speed.v[XDIM]*msUpdPlayer, 0.0f, 0.0f);
		}
		else*/ 
			MoveRelTo(Speed.v[XDIM]*msUpdPlayer, 0.0f, 0.0f);	///Speed is constant (in 2D mode

		if (Position.v[XDIM] > CSIG_PG_RIGHT)
			Position.v[XDIM] = CSIG_PG_RIGHT;

		//Control to rotate the Player
		Scrolling = Speed.v[XDIM];
	}
	QuadtreeRoot::Instance()->getQuadtree()->ActualizeCharacter(this);
}

void CPlayer2::UserLeft	()
{
	PositionPrev=Position;
	if (Alive)
	{
		//ACHTUNG: To do
		/*if (CHAR_3D == RenderMode)				
		{
			Speed.v[XDIM] -= Acceleration.v[XDIM] * msUpdPlayer;
			if (Speed.v[XDIM] < - CP_MAX_SPEED)
				Speed.v[XDIM] = - CP_MAX_SPEED;
			MoveRelTo(Speed.v[XDIM]*msUpdPlayer,  0.0f, 0.0f);
		}
		else */
			MoveRelTo(-Speed.v[XDIM]*msUpdPlayer,  0.0f, 0.0f); ///Speed is constant in 2D mode

		if (Position.v[XDIM] < CSIG_PG_LEFT)
			Position.v[XDIM] = CSIG_PG_LEFT;

		//Control to rotate the Player
		Scrolling = -Speed.v[XDIM];
	}
		
	QuadtreeRoot::Instance()->getQuadtree()->ActualizeCharacter(this);
}

void CPlayer2::Update(void){
	#ifdef DEF_UPD_TIME
	Game.TimerManager.Timers[TmrUpd].InitCounting();
	#endif

	RTDESK_TIME nowTime, elapTime;

	nowTime = Game.TimerManager.Timers[TmrApp].TicksUntilNow();
	elapTime = nowTime - LastUpdTime;
	if(elapTime >= TicksToUpdatePlayer){

#ifndef DEF_CONTINUOUS_WITH_SIMULATE_TIME
		LastUpdTime = nowTime;

		msUpdPlayer = abs(Size.v[YDIM]/Speed.v[YDIM]);
		if(msUpdPlayer > CSIG_MIN_UPDATETIME_OBJECTS) msUpdPlayer = CSIG_MIN_UPDATETIME_OBJECTS;
		TicksToUpdatePlayer = UpdatePlayerTimer.ms2Ticks(msUpdPlayer);

		//Control to rotate the Player
		if(Scrolling >  0.0001) Scrolling -= 0.001;
		if(Scrolling < -0.0001) Scrolling += 0.001;
#else
		bool SynWithRealTime = false;
		LastUpdTime += TicksToUpdatePlayer;

		msUpdPlayer = abs(Size.v[YDIM]/Speed.v[YDIM]);
		if(msUpdPlayer > CSIG_MIN_UPDATETIME_OBJECTS) msUpdPlayer = CSIG_MIN_UPDATETIME_OBJECTS;
		TicksToUpdatePlayer = UpdatePlayerTimer.ms2Ticks(msUpdPlayer);

		while(!SynWithRealTime){
			//Control to rotate the Player
			if(Scrolling >  0.0001) Scrolling -= 0.001;
			if(Scrolling < -0.0001) Scrolling += 0.001;

		nowTime = Game.TimerManager.Timers[TmrApp].TicksUntilNow();
		elapTime = nowTime - LastUpdTime;
		if(elapTime >= 0LL) 
				SynWithRealTime=true;
			else
				SynWithRealTime=false;
		}
#endif
	}

	#ifdef DEF_UPD_TIME
	EndAccCounting(TmrUpd);
	#endif
}

void CPlayer2::DiscreteUpdate(void){
	#ifdef DEF_UPD_TIME
	Game.TimerManager.Timers[TmrUpd].InitCounting();
	#endif

	//Control to rotate the Player
	if(Scrolling >  0.0001) Scrolling -= 0.001;
	if(Scrolling < -0.0001) Scrolling += 0.001;

	msUpdPlayer = abs(Size.v[YDIM]/Speed.v[YDIM]);
	if(msUpdPlayer > CSIG_MIN_UPDATETIME_OBJECTS) msUpdPlayer = CSIG_MIN_UPDATETIME_OBJECTS;
	TicksToUpdatePlayer = UpdatePlayerTimer.ms2Ticks(msUpdPlayer);

	#ifdef DEF_UPD_TIME
	EndAccCounting(TmrUpd);
	#endif

	if(!endByTime && !endByFrame){
		#ifdef DEF_RTD_TIME
		Game.TimerManager.Timers[TmrRTDSKMM].InitCounting();
		#endif

		SendSelfMsg(msg,TicksToUpdatePlayer);

		#ifdef DEF_RTD_TIME
		EndAccCounting(TmrRTDSKMM);
		#endif
	}
}

void CPlayer2::ReceiveMessage(RTDESK_CMsg *pMsg){
		
	DiscreteUpdate();

}

void CPlayer2::UserShoot	()
{
	Vector	P,	//Shoot initial Position
			S;	//Shoot initial Speed

	if (Alive != true) return;	///No gun is fired when the player is dead
		if (CHAR_2D == RenderMode)
		{
			SoundsManager.Play(CGS_SHOOT,UGKSND_LOW_VOLUME);

			S.v[XDIM] = S.v[ZDIM] = 0.0f;
			S.v[YDIM] = CP_SHOOT2D_SPEED;
		}
		else
		{
			SoundsManager.Play(CGS_SHOOT3D,UGKSND_LOW_VOLUME);
			
			S.v[XDIM] = S.v[ZDIM] = 0.0f;
			S.v[YDIM] = CP_SHOOT3D_SPEED;
		}

		///Generate a new player fire
		//x
			
		if (CHAR_2D == RenderMode){
			P.v[XDIM]	= Position.v[XDIM];
			P.v[YDIM]	= Position.v[YDIM] + .5f;
			P.v[ZDIM]	= Position.v[ZDIM];
		}
		else{
			P.v[XDIM]	= Position.v[XDIM];
			P.v[YDIM]	= Position.v[YDIM] + 1.5f;
			P.v[ZDIM]	= Mesh->modelo.pos.z;
		}
			
		ShootsManager.NewShoot(Player2[CurrentPlayer].ShootType, P, S);				//Auxiliar shoots with Bonus Weapon
		if(Player2[CurrentPlayer].ShootType == CSH_PLAYER3D_CHEVRON){
			P.v[XDIM] += 0.5;
			ShootsManager.nextShootVisible = true;
			ShootsManager.NewShoot(Player2[CurrentPlayer].ShootType, P, S);
			P.v[XDIM] -= 0.5;
			P.v[XDIM] -= 0.5;
			ShootsManager.nextShootVisible = true;
			ShootsManager.NewShoot(Player2[CurrentPlayer].ShootType, P, S);
		}

		if (Player2[CurrentPlayer].Laser[CP_LEFT_LASER].Alive)						//Left Laser shoot
		{
			P.v[XDIM]		= Position.v[XDIM] - .9f;
			P.v[YDIM]		= Position.v[YDIM] + .8f;
			P.v[ZDIM]		= Mesh->modelo.pos.z;

			S.v[XDIM] = S.v[ZDIM] = 0.0f;
			S.v[YDIM] = CP_SHOOT3D_SPEED;
			ShootsManager.NewShoot(CSH_AUXILIAR_LASER, P, S);
		}

		if (Player2[CurrentPlayer].Laser[CP_RIGHT_LASER].Alive)						//Right Laser shoot
		{
			P.v[XDIM]		= Position.v[XDIM] + .9f;
			P.v[YDIM]		= Position.v[YDIM] + .8f;
			P.v[ZDIM]		= Mesh->modelo.pos.z;

			S.v[XDIM] = S.v[ZDIM] = 0.0f;
			S.v[YDIM] = CP_SHOOT3D_SPEED;
			ShootsManager.NewShoot(CSH_AUXILIAR_LASER, P, S);
		}
}

void CPlayer2::DiscreteUserShoot	()
{
	Vector	P,	//Shoot initial Position
			S;	//Shoot initial Speed

	if (Alive != true) return;	///No gun is fired when the player is dead
		if (CHAR_2D == RenderMode)
		{
			SoundsManager.Play(CGS_SHOOT,UGKSND_LOW_VOLUME);

			S.v[XDIM] = S.v[ZDIM] = 0.0f;
			S.v[YDIM] = CP_SHOOT2D_SPEED;
		}
		else
		{
			SoundsManager.Play(CGS_SHOOT3D,UGKSND_LOW_VOLUME);
			
			S.v[XDIM] = S.v[ZDIM] = 0.0f;
			S.v[YDIM] = CP_SHOOT3D_SPEED;
		}

		///Generate a new player fire
		//x
			
		if (CHAR_2D == RenderMode){
			P.v[XDIM]	= Position.v[XDIM];
			P.v[YDIM]	= Position.v[YDIM] + .5f;
			P.v[ZDIM]	= Position.v[ZDIM];
		}
		else{
			P.v[XDIM]	= Position.v[XDIM];
			P.v[YDIM]	= Position.v[YDIM] + 1.5f;
			P.v[ZDIM]	= Mesh->modelo.pos.z;
		}
			
		DiscreteShootsManager.DiscreteNewShoot(Player2[CurrentPlayer].ShootType, P, S);
		if(Player2[CurrentPlayer].ShootType == CSH_PLAYER3D_CHEVRON){
			P.v[XDIM] += 0.5;
			DiscreteShootsManager.nextShootVisible = true;
			DiscreteShootsManager.DiscreteNewShoot(Player2[CurrentPlayer].ShootType, P, S);
			P.v[XDIM] -= 0.5;
			P.v[XDIM] -= 0.5;
			DiscreteShootsManager.nextShootVisible = true;
			DiscreteShootsManager.DiscreteNewShoot(Player2[CurrentPlayer].ShootType, P, S);
		}

		if (Player2[CurrentPlayer].Laser[CP_LEFT_LASER].Alive)						// le laser de gauche tire
		{
			P.v[XDIM]		= Position.v[XDIM] - .9f;
			P.v[YDIM]		= Position.v[YDIM] + .8f;
			P.v[ZDIM]		= Mesh->modelo.pos.z;

			S.v[XDIM] = S.v[ZDIM] = 0.0f;
			S.v[YDIM] = CP_SHOOT3D_SPEED;
			DiscreteShootsManager.DiscreteNewShoot(CSH_AUXILIAR_LASER, P, S);
		}

		if (Player2[CurrentPlayer].Laser[CP_RIGHT_LASER].Alive)						// le laser de droite tire
		{
			P.v[XDIM]		= Position.v[XDIM] + .9f;
			P.v[YDIM]		= Position.v[YDIM] + .8f;
			P.v[ZDIM]		= Mesh->modelo.pos.z;

			S.v[XDIM] = S.v[ZDIM] = 0.0f;
			S.v[YDIM] = CP_SHOOT3D_SPEED;
			DiscreteShootsManager.DiscreteNewShoot(CSH_AUXILIAR_LASER, P, S);
		}
}
