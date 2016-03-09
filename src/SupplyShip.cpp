/** /////////////////////////////////////////////////////////////////////////////////////
//
// SupplyShip initialization
//
// These values are by default. They have to be overwritten in the initialization phase
// when reading the default values from the "initialization.html" file

	@author Ramon Molla
	@version 2011-10-11
*/
#include "Global.h"
#include "SupplyShip.h"

///Different name states for the FSMs that control the SS behaviour
char CSS_NameState[CSS_MAXSTATE][GCHAR_TAG_MAX_LONG ] = 
{
	"DEFAULT",
	"BORN",
	"LIVING",
	"DYING",
	"DEAD"
};

///Sensitive transition names
char CSS_NameTransition[CSS_MAXTRANSITION][GCHAR_TAG_MAX_LONG] = 
{
	"DEFAULT",
	"BORNING",
	"GETTING_ALIVE",
	"MOVING",
	"DISPLAYING",
	"HEALTHING",
	"BURST",
	"DIE",
	"FINISHED",
	"RESURRECTING"	
};

//Class behaviour
void CSupplyShip::Init()
{
	Health = MaxHealth	=	CSS_MAX_HEALTH;
	Hit_duration		=	CSS_MAX_HIT_DURATION;
	SubType				=	CSS_SUPPLY_SHIP;
	Type				=	CHARS_SUPPLYSHIP;
	Speed.Reset();
	Speed.v[XDIM]		=	0.002f;	//Units/ms

	Explosion.Init();
	Explosion.Life		=	CS_MAX_EXPLOSION_LIFE;

	Size.v[XDIM]			=	0.9f;
	Size.v[YDIM]			=	0.5f;
	Size.v[ZDIM]			=	0.0f;
	RenderMode			=	CHAR_2D;

	if (NULL != AI)
			delete AI;

#ifdef CHAR_USE_AABB
	InitializeAABB();
#endif

	MoveTo(0.0f, 4.0f, 0.05f);

	/*msgUpd = new cMsgSShip;
	msgUpd->Type = RTDESKMSG_SSHIP;
	msgUpd->msgSubType = TIPOUPD_SS;
	msgUpd->Propietary = true;*/
}

void CSupplyShip::AI_Init(SIAI_AI_TYPE AIType) 
{
	switch(AIType)
	{
		case SIAI_SUPPLYSHIP_DEFAULT:
			if (NULL != AI)
				delete AI;
			AI = AIManager.GetAI(SIAI_SUPPLYSHIP_DEFAULT);  ///<Asks the AIManager for the 'FSM' corresponding to the SupplyShip appropiately initialized.
		break;
		default:;
	}//Behaviour switch
}

/**
* Initializes the artificial intelligence of the SupplyShip.
*/
void CSupplyShip::AI_Init(void)
{
	if (NULL != AI)	///<If the structure is not empty
		delete AI;	///<cleans all the data structure.

	AI_Init(SIAI_SUPPLYSHIP_DEFAULT);
}

///Argument means the amount of miliseconds spent during the last 10 frames/game iterations
void CSupplyShip::AI_Healthing()
{
	
	if	(Health < MaxHealth)
			Health+=Game.RenderTemp*0.0002f;
	else Health = MaxHealth;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// SupplyShip movement
// @args[0] : SupplyShip moving
//
//
void CSupplyShip::AI_Move()
{
	PositionPrev=Position;
	//Argument means the amount of miliseconds spent during the last 10 frames/game iterations
	//UGKPHY_EULER integrator. No acceleration taken into account
	MoveRelTo(Speed.v[XDIM] * Game.SupplyShipTemp, 0.0f, 0.0f);
	if (Position.v[XDIM] < -CSS_MAX_X_SHIFT)			// Change movement direction
	{
		//Infinite acceleration
		if (0>Speed.v[XDIM]) Speed.v[XDIM] = -Speed.v[XDIM];
	}
	else if (Position.v[XDIM] > CSS_MAX_X_SHIFT)
		//Infinite acceleration
		if (0<Speed.v[XDIM]) Speed.v[XDIM] = -Speed.v[XDIM];

	#ifdef CHAR_USE_QUADTREE
	QuadtreeRoot::Instance()->getQuadtree()->ActualizeCharacter(this);
	#endif
}

///The supply ship is going to burst before being dead
void CSupplyShip::AI_Dye(void)
{
	///Changing internal attributes
	Alive = Active = false;
	Explosion.Life = float((rand()%100)+500);
	#ifdef CHAR_USE_QUADTREE
	QuadtreeRoot::Instance()->getQuadtree()->RemoveCharacter(this);
	#endif
}

///Nothinf at all has to be done. The supply ship is dead
void CSupplyShip::AI_Death()
{
}

//Physics
void CSupplyShip::Collided (CCharacter *CollidedChar)
{
	CSH_SHOOT_TYPE typeAux = ((CShoot*)CollidedChar)->SubType;

	switch (CollidedChar->Type)
	{
	case CHARS_PLAYER_SHOOT:
		if (CSH_PLAYER==typeAux || CSH_PLAYER3D==typeAux || CSH_PLAYER3D_CHEVRON==typeAux ||CSH_AUXILIAR_LASER==typeAux)
		{
			Health -= CollidedChar->Health;
			if (Health <= 0)
			{
				SoundsManager.Play(CGS_EXPLOSION,UGKSND_VOLUME_80);
				AI_Dye();
				Explosion.Init(this);
				Explosion.Active = true;
				Game.Score += (CS_MAXTYPE - Type) * 10;
				if(Game.Score > Game.HiScore) Game.HiScore=Game.Score;
			}

		}
	case CHARS_SHIP:
		break;
	}
}

//Rendering procedures

void CSupplyShip::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	RenderMode = Mode;
	Explosion.ChangeRenderMode(Mode);

	switch(Mode)
	{
	case CHAR_NO_RENDER:			//No render for this character: camera, collision objects,...
		break;
	case CHAR_2D:
		Size.v[XDIM]			=	0.9f;
		Size.v[YDIM]			=	0.5f;
		Size.v[ZDIM]			=	0.0f;
		break;
	case CHAR_LINES3D:
		break;
	case CHAR_3D:
		Size.v[XDIM]			=	0.9f;
		Size.v[YDIM]			=	0.5f;
		Size.v[ZDIM]			=	0.0f;

	#ifdef CHAR_USE_QUADTREE
		if(Mesh->Loaded)
			FitMeshIntoBoundingBox(*Mesh);
	#endif
		break;
	default: return;
	}
	
}

void CSupplyShip::Render()
{
	switch(RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character
		break;
	case CHAR_2D:
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		TexturesManager.Textures[IndTexture2D]->SetTexture();

		// ALPHA TEST + BLEND
		glAlphaFunc(GL_GREATER, 0.4f);								// for TGA alpha test
		glEnable(GL_ALPHA_TEST);									// for TGA alpha test
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// for anti-aliasing

		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (Position.v[XDIM] - .5, Position.v[YDIM] +.3);
			glTexCoord2f (0,0);	glVertex2f (Position.v[XDIM] - .5, Position.v[YDIM] -.3);
			glTexCoord2f (1,0);	glVertex2f (Position.v[XDIM] + .5, Position.v[YDIM] -.3);
			glTexCoord2f (1,1);	glVertex2f (Position.v[XDIM] + .5, Position.v[YDIM] +.3);
		glEnd ();

		// BACK TO NON ALPHA TEST + PREVIOUS BLEND
		glDisable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		break;
	case CHAR_LINES3D:
	case CHAR_3D:
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		Mesh->modelo.pos.x = Position.v[XDIM];
		Mesh->modelo.pos.y = Position.v[YDIM];
		Mesh->modelo.pos.z = Position.v[ZDIM];

		Mesh->modelo.rot.x =  90.0;

		Mesh->modelo.scaleY = .005;
		//Mesh->modelo.scale = 0.005;
		
		// SupplyShip touché
		if (Hit_duration < CSS_MAX_HIT_DURATION)
		{
			Hit_duration -= 10.0 * Game.RenderTemp;
			if (Hit_duration <= 0) Hit_duration = CSS_MAX_HIT_DURATION;

			Mesh->modelo.Draw();
		}
		
		// SupplyShip normal
		else
			Mesh->modelo.Draw();
		break;
	default: return;
	}
}

void CSupplyShip::Update(void){

	if (Alive)
	{
		Vector P, S;
		RTDESK_TIME nowTime, elapTime;

		nowTime = Game.TimerManager.Timers[TmrApp].TicksUntilNow();
		elapTime = nowTime - LastUpdTime;
		if(elapTime >= TicksToUpdateSupplyShip){

#ifndef DEF_CONTINUOUS_WITH_SIMULATE_TIME
			LastUpdTime = nowTime;

			//Next execution time calculation. (TicksToUpdateSupplyShip)
			double auxX= abs(Size.v[XDIM]/Speed.v[XDIM]);
			double auxY= abs(Size.v[YDIM]/Speed.v[YDIM]);
			double auxZ= abs(Size.v[ZDIM]/Speed.v[ZDIM]);	
			msUpdSShip = sqrt((auxX*auxX)+(auxY*auxY));//+(auxZ*auxZ));
			if(msUpdSShip > CSIG_MIN_UPDATETIME_OBJECTS) msUpdSShip = CSIG_MIN_UPDATETIME_OBJECTS;
			TicksToUpdateSupplyShip = Navy.Timer.ms2Ticks(msUpdSShip);

			//Shooting calculation
			if (( floor((rand()%100000/(1+Navy.ShootsFrequency))/msUpdSShip)) == 1) ///Has the Supply ship to fire?
			{
				P.Set(Position.v[XDIM],
					  Position.v[YDIM] - .3f,
					  .05f);
				S.Set(0.0f,
					  -CSS_SHOOT_SPEED,
					  0.0f);

				if(Navy.WithShoots)
					ShootsManager.NewShoot(CSH_SUPPLY_SHIP, P, S);
				
			}

			//Move the supply ship
			AI->outEvent(CSS_NameTransition[CSS_MOVING], NULL, (void*) this);	//v 2->2
#else
			bool SynWithRealTime = false;
			LastUpdTime += TicksToUpdateSupplyShip;

			//Next execution time calculation. (TicksToUpdateSupplyShip)
			double auxX= abs(Size.v[XDIM]/Speed.v[XDIM]);
			double auxY= abs(Size.v[YDIM]/Speed.v[YDIM]);
			double auxZ= abs(Size.v[ZDIM]/Speed.v[ZDIM]);	
			msUpdSShip = sqrt((auxX*auxX)+(auxY*auxY));//+(auxZ*auxZ));
			if(msUpdSShip > CSIG_MIN_UPDATETIME_OBJECTS) msUpdSShip = CSIG_MIN_UPDATETIME_OBJECTS;
			TicksToUpdateSupplyShip = Navy.Timer.ms2Ticks(msUpdSShip);

			//If next execution time is greater than real time then return. 
			//Else repeat until is synchronized with real time
			while(!SynWithRealTime){
				//Shooting calculation
				if (( floor((rand()%100000/(1+Navy.ShootsFrequency))/msUpdSShip)) == 1) ///Has the Supply ship to fire?
				{
					P.Set(Position.v[XDIM],
						  Position.v[YDIM] - .3f,
						  .05f);
					S.Set(0.0f,
						  -CSS_SHOOT_SPEED,
						  0.0f);

					if(Navy.WithShoots)
						ShootsManager.NewShoot(CSH_SUPPLY_SHIP, P, S);
					
				}

				//Move the supply ship
				AI->outEvent(CSS_NameTransition[CSS_MOVING], NULL, (void*) this);	//v 2->2
				
				nowTime = Game.TimerManager.Timers[TmrApp].TicksUntilNow();
				elapTime = nowTime - LastUpdTime;
				if(elapTime >= 0) 
					SynWithRealTime=true;
				else
					SynWithRealTime=false;
			}
#endif
		}
	}
}

void CSupplyShip::DiscreteUpdate(void){
	
	if (Alive && !endByTime && !endByFrame)
	{
		#ifdef DEF_UPD_TIME
		Game.TimerManager.Timers[TmrUpd].InitCounting();
		#endif

		//Shooting calculation
		if (( floor((rand()%100000/(1+Navy.ShootsFrequency))/msUpdSShip)) == 1) ///Has the Supply ship to fire?
		{
			Vector P, S;

			P.Set(Position.v[XDIM],
				  Position.v[YDIM] - .3f,
				  .05f);
			S.Set(0.0f,
				  -CSS_SHOOT_SPEED,
				  0.0f);

			if(Navy.WithShoots)
				DiscreteShootsManager.DiscreteNewShoot(CSH_SUPPLY_SHIP, P, S);
			
		}

		//Move the supply ship
		AI->outEvent(CSS_NameTransition[CSS_MOVING], NULL, (void*) this);	//v 2->2
	
		double auxX= abs(Size.v[XDIM]/Speed.v[XDIM]);
		double auxY= abs(Size.v[YDIM]/Speed.v[YDIM]);
		double auxZ= abs(Size.v[ZDIM]/Speed.v[ZDIM]);

		msUpdSShip = sqrt((auxX*auxX)+(auxY*auxY));//+(auxZ*auxZ));
		if(msUpdSShip > CSIG_MIN_UPDATETIME_OBJECTS) msUpdSShip = CSIG_MIN_UPDATETIME_OBJECTS;
		TicksToUpdateSupplyShip = Navy.Timer.ms2Ticks(msUpdSShip);

		#ifdef DEF_UPD_TIME
		EndAccCounting(TmrUpd);
		#endif
		#ifdef DEF_RTD_TIME
		Game.TimerManager.Timers[TmrRTDSKMM].InitCounting();
		#endif
	
		msgUpd =(cMsgSShip*) GetMsgToFill(RTDESKMSG_SSHIP);
		msgUpd->Type = RTDESKMSG_SSHIP;
		msgUpd->msgSubType = TIPOUPD_SS;
		SendSelfMsg(msgUpd,TicksToUpdateSupplyShip);

		#ifdef DEF_RTD_TIME
		EndAccCounting(TmrRTDSKMM);
		#endif
	}
}

void CSupplyShip::ReceiveMessage(RTDESK_CMsg *pMsg){
	
	switch (pMsg->Type)
	{
	case RTDESKMSG_SSHIP:
		cMsgSShip *auxMsg;
			auxMsg = (cMsgSShip*)pMsg;
			switch (auxMsg->msgSubType)
			{
			case TIPOUPD_SS:

				DiscreteUpdate();

				break;
			}
		break;
	case RTDESKMSG_BASIC_TYPE:

		DiscreteUpdate();

		break;
	}
	 

}


///Called everytime a time slot happens and its health has to be increased
void *CSS_FSM_Healthing	(LPSTR *args, CSupplyShip *SupplyShip)
{
	SupplyShip->AI_Healthing();
	return NULL;
}

///Called everytime a time slot happens and a moving has to be done
void *CSS_FSM_Move		(LPSTR *args, CSupplyShip *SupplyShip)
{
	
	SupplyShip->AI_Move();
	
	return NULL;
	
}

///Called when the supply ship is going to burst before dying
void *CSS_FSM_Dye	(LPSTR *args, CSupplyShip *SupplyShip)
{
	// SupplyShip dead
	SupplyShip->Explosion.Init(SupplyShip);
	SupplyShip->AI_Dye();
	Game.Score += (int)ceil((float)rand() / 100 );
	if(Game.Score > Game.HiScore) Game.HiScore=Game.Score;

	return NULL;
};

void *CSS_FSM_Death		(LPSTR *args, CSupplyShip *SupplyShip)
{
	SupplyShip->AI_Dye();
	return NULL;
}

///Called when the supply ship passed from death to unborn states
void *CSS_FSM_Unborning (LPSTR *args, CSupplyShip *SupplyShip)
{
	SupplyShip->AI_Init();
	return NULL;
}

void *CSS_display(LPSTR *args, CSupplyShip *SupplyShip)
{
	SupplyShip->Render();
	return NULL;
}
