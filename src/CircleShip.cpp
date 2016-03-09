/*
	@author Ramon Molla
	@version 2011-11
*/

#include <AI\FSMParser.h>
#include "SI_AI.h"

#include "Global.h"
#include "ShootsManager.h"
#include "CircleShip.h"
#include <SIGame.h>
#include <UGKPhysics.h>
#include <math.h>

//Global variables
extern char					CSIG_NameTransition[CSIG_MAXTRANSITION][GCHAR_TAG_MAX_LONG]; 

//////////////////////////////////////////////////////////////////////////////////////
//
// Ship initialization
//
// These values are by default. They have to be overwritten in the initialization phase
// when reading the default values from the "initialization.html" file

char *CCS_StateName[CCS_MAXSTATE] =
{
	"DEFAULT",
	"BORN",
	"LIVING",
	"DYING",
	"DEAD"
};

///Sensitive transition names
char CCS_NameTransition[CCS_MAXTRANSITION][GCHAR_TAG_MAX_LONG] = 
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
void CCircleShip::Init()
{
	Health = MaxHealth	=	CCS_MAX_HEALTH;
	Hit_duration		=	CCS_MAX_MAX_HIT_DUR;
	SubType				=	CCS_BOTHTYPE;
	Type				=	CHARS_CIRCLESHIP;
	Speed.Reset();
	Speed.v[XDIM]		=	0.002f;	//Units/ms

	Explosion.Init();  //Se inicia la explocion por defecto deshabilitado   

	Size.Set(0.9f,
			 0.5f,
			 0.0f);
	RenderMode			=	CHAR_2D;
	//InitializeAABB();

	Radio=2.0f;
	Center.Set(0.0f,
			   4.0f,
			   0.05f);
	Degrees=0;
	Velocity=0.01;
	ChgDir = false;

	if (NULL != AI) delete AI;

	/*msgUpd = new cMsgCShip;
	msgUpd->Type = RTDESKMSG_CSHIP;
	msgUpd->msgSubType = TIPOUPD_CS;
	msgUpd->Propietary = true;*/
}

void CCircleShip::AI_Init(SIAI_AI_TYPE AIType) 
{
	switch(AIType)
	{
		case SIAI_CIRCLESHIP_DEFAULT:
			if (NULL != AI)
				delete AI;
			AI = AIManager.GetAI(SIAI_CIRCLESHIP_DEFAULT);  ///<Asks the AIManager for the 'FSM' corresponding to the SupplyShip appropiately initialized.
		break;
		default:;
	}//Behaviour switch
}

/**
* Initializes the artificial intelligence of the SupplyShip.
*/
void CCircleShip::AI_Init(void)
{
	if (NULL != AI)	///<If the structure is not empty
		delete AI;	///<cleans all the data structure.

	AI_Init(SIAI_CIRCLESHIP_DEFAULT); 

}

///Argument means the amount of miliseconds spent during the last 10 frames/game iterations
void CCircleShip::AI_Healthing()
{
	
	if	(Health < MaxHealth){
		Game.TimerManager.Timers[TmrApp].EndCounting();
		Health+=Game.TimerManager.Timers[TmrApp].GetLastPeriodms() *0.0002f;
	}
	else Health = MaxHealth;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// CircleShip movement
// @args[0] : CircleShip moving
//
//
void CCircleShip::AI_Move()
{

	//Argument means the amount of miliseconds spent during the last 10 frames/game iterations
	//UGKPHY_EULER integrator. No acceleration taken into account
	//CirclesMovement

	PositionPrev=Position;

	if(ChgDir){
		//recalculate angle
		Degrees-=Velocity*Game.CircleShipTemp;
	}else{
		//recalculate angle
		Degrees+=Velocity*Game.CircleShipTemp;
	}

	//Move
	MoveTo((sin(Degrees)*Radio)+Center.v[XDIM],(cos(Degrees)*Radio)+Center.v[YDIM],-0.000000001);
	//Choose if it must send a change turn event
	if(Degrees>2*PI){
			ChgDir = true;
			Degrees=0;
	}else if(Degrees<-2*PI){
			ChgDir = false;
			Degrees=0;
	}

#ifdef CHAR_USE_QUADTREE
	QuadtreeRoot::Instance()->getQuadtree()->ActualizeCharacter(this);
#endif

}

///The circle ship is going to burst before being dead
void CCircleShip::AI_Dye(void)
{
	///Changing internal attributes
	Alive = Active = false;
	Explosion.Life = float((rand()%100)+500);
	
#ifdef CHAR_USE_QUADTREE

	QuadtreeRoot::Instance()->getQuadtree()->RemoveCharacter(this);
#endif
}

///Nothinf at all has to be done. The circle ship is dead
void CCircleShip::AI_Death()
{
}

//Physics
void CCircleShip::Collided (CCharacter *CollidedChar)
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
		break;
	case CHARS_SHIP:
		break;
	}
}

//Rendering procedures

void CCircleShip::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	RenderMode = Mode;
	Explosion.ChangeRenderMode(Mode);

	switch(Mode)
	{
	case CHAR_NO_RENDER:			//No render for this character: camera, collision objects,...
		break;
	case CHAR_2D:
		Size.Set(0.9f,
				 0.5f,
				 0.0f);
		break;
	case CHAR_LINES3D:
		break;
	case CHAR_3D:
		Size.Set(0.9f,
				 0.5f,
				 0.0f);
		if (Mesh)
			if(Mesh->Loaded = true)
				FitMeshIntoBoundingBox(*Mesh);
		break;
	default: return;
	}
}

void CCircleShip::Render()
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

		Mesh->modelo.rot.x = 90.0;

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

///Called everytime a time slot happens and its health has to be increased
void *CCS_FSM_Healthing	(LPSTR *args, CCircleShip *CircleShip)
{
	CircleShip->AI_Healthing();
	return NULL;
}

///Called everytime a time slot happens and a moving has to be done
void *CCS_FSM_Move		(LPSTR *args, CCircleShip *CircleShip)
{
	CircleShip->AI_Move();
	return NULL;
}

///Called when the circle ship is going to burst before dying
void *CCS_FSM_Dye	(LPSTR *args, CCircleShip *CircleShip)
{
	// CircleShip dead
	CircleShip->Explosion.Init(CircleShip);
	CircleShip->AI_Dye();
	Game.Score += (int)ceil((float)rand() / 100 );
	if(Game.Score > Game.HiScore) Game.HiScore=Game.Score;
	
	return NULL;
};

void CCircleShip::Update(void){

	if(Alive)
	{
		Vector P, S;
		
		RTDESK_TIME nowTime, elapTime;

		nowTime = Game.TimerManager.Timers[TmrApp].TicksUntilNow();
		elapTime = nowTime - LastUpdTime;
		if(elapTime >= TicksToUpdateCircleShip){

#ifndef DEF_CONTINUOUS_WITH_SIMULATE_TIME
			LastUpdTime = nowTime;

			//Next execution time calculation.(TicksToUpdateCircleShip)
			double auxX= abs(Size.v[XDIM]/Speed.v[XDIM]);
			double auxY= abs(Size.v[YDIM]/Speed.v[YDIM]);
			double auxZ= abs(Size.v[ZDIM]/Speed.v[ZDIM]);
			msUpdCShip = sqrt((auxX*auxX)+(auxY*auxY));//+(auxZ*auxZ));
			if(msUpdCShip > CSIG_MIN_UPDATETIME_OBJECTS) msUpdCShip = CSIG_MIN_UPDATETIME_OBJECTS;
			TicksToUpdateCircleShip = Navy.Timer.ms2Ticks(msUpdCShip);

			//Shooting calculation
			if (( floor((rand()%100000/(1+Navy.ShootsFrequency))/msUpdCShip)) == 1) ///Has the Supply ship to fire?
			{
				P.Set(Position.v[XDIM],
					  Position.v[YDIM] - .3f,
					  .05f);
				S.Set(0.0f,
					  -CCS_SHOOT_SPEED,
					  0.0f);

				if(Navy.WithShoots)
					ShootsManager.NewShoot(CSH_CIRCLE_SHIP, P, S);
			}
		
			//Move the circle ship
			AI->outEvent(CCS_NameTransition[CCS_MOVING], NULL, (void*) this);	//v 2->2
#else
			bool SynWithRealTime = false;
			LastUpdTime += TicksToUpdateCircleShip;

			//Next execution time calculation.(TicksToUpdateCircleShip)
			double auxX= abs(Size.v[XDIM]/Speed.v[XDIM]);
			double auxY= abs(Size.v[YDIM]/Speed.v[YDIM]);
			double auxZ= abs(Size.v[ZDIM]/Speed.v[ZDIM]);
			msUpdCShip = sqrt((auxX*auxX)+(auxY*auxY));//+(auxZ*auxZ));
			if(msUpdCShip > CSIG_MIN_UPDATETIME_OBJECTS) msUpdCShip = CSIG_MIN_UPDATETIME_OBJECTS;
			TicksToUpdateCircleShip = Navy.Timer.ms2Ticks(msUpdCShip);
			Navy.Acceleration;

			//If next execution time is greater than real time then return. 
			//Else repeat until is synchronized with real time
			while(!SynWithRealTime){
				//Shooting calculation
				if (( floor((rand()%100000/(1+Navy.ShootsFrequency))/msUpdCShip)) == 1) ///Has the Supply ship to fire?
				{
					P.Set(Position.v[XDIM],
						  Position.v[YDIM] - .3f,
						  .05f);
					S.Set(0.0f,
						  -CCS_SHOOT_SPEED,
						  0.0f);

					if(Navy.WithShoots)
						ShootsManager.NewShoot(CSH_CIRCLE_SHIP, P, S);
					
				}
		
				//Move the circle ship
				AI->outEvent(CCS_NameTransition[CCS_MOVING], NULL, (void*) this);	//v 2->2
			
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

void CCircleShip::DiscreteUpdate(void){
	
	if(Alive && !endByTime && !endByFrame)
	{
		#ifdef DEF_UPD_TIME
		Game.TimerManager.Timers[TmrUpd].InitCounting();
		#endif

		//Shooting calculation
		if (( floor((rand()%100000/(1+Navy.ShootsFrequency))/msUpdCShip)) == 1) ///Has the Supply ship to fire?
		{
			Vector P, S;
			P.Set(Position.v[XDIM],
				  Position.v[YDIM] - .3f,
				  .05f);
			S.Set(0.0f,
				  -CCS_SHOOT_SPEED,
				  0.0f);

			if(Navy.WithShoots)
				DiscreteShootsManager.DiscreteNewShoot(CSH_CIRCLE_SHIP, P, S);
			
		}
		
		//Move the circle ship
		AI->outEvent(CCS_NameTransition[CCS_MOVING], NULL, (void*) this);	//v 2->2

		double auxX= abs(Size.v[XDIM]/Speed.v[XDIM]);
		double auxY= abs(Size.v[YDIM]/Speed.v[YDIM]);
		double auxZ= abs(Size.v[ZDIM]/Speed.v[ZDIM]);

		msUpdCShip = sqrt((auxX*auxX)+(auxY*auxY));//+(auxZ*auxZ));
		if(msUpdCShip > CSIG_MIN_UPDATETIME_OBJECTS) msUpdCShip = CSIG_MIN_UPDATETIME_OBJECTS;
		TicksToUpdateCircleShip = Navy.Timer.ms2Ticks(msUpdCShip);

		#ifdef DEF_UPD_TIME
		EndAccCounting(TmrUpd);
		#endif

		#ifdef DEF_RTD_TIME
		Game.TimerManager.Timers[TmrRTDSKMM].InitCounting();
		#endif

		msgUpd =(cMsgCShip*) GetMsgToFill(RTDESKMSG_CSHIP);
		msgUpd->Type = RTDESKMSG_CSHIP;
		msgUpd->msgSubType = TIPOUPD_CS;
		SendSelfMsg(msgUpd,TicksToUpdateCircleShip);

		#ifdef DEF_RTD_TIME
		EndAccCounting(TmrRTDSKMM);
		#endif
	}
}

void CCircleShip::ReceiveMessage(RTDESK_CMsg *pMsg){

	switch (pMsg->Type)
	{
	case RTDESKMSG_CSHIP:
		cMsgCShip *auxMsg;
			auxMsg = (cMsgCShip*)pMsg;
			switch (auxMsg->msgSubType)
			{
			case TIPOUPD_CS:

				DiscreteUpdate();

				break;
			}
		break;
	case RTDESKMSG_BASIC_TYPE:

		DiscreteUpdate();

		break;
	}

}

void *CCS_FSM_Death		(LPSTR *args, CCircleShip *CircleShip)
{
	CircleShip->AI_Dye();
	return NULL;
}

///Called when the circle ship passed from death to unborn states
void *CCS_FSM_Unborning (LPSTR *args, CCircleShip *CircleShip)
{
	CircleShip->AI_Init();
	return NULL;
}

void *CCS_display(LPSTR *args, CCircleShip *CircleShip)
{
	CircleShip->Render();
	return NULL;
}
