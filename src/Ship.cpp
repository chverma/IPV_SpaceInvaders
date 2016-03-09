/*
	@author Ramon Molla
	@version 2011-11
*/

#include <AI\FSMParser.h>
#include "SI_AI.h"

#include "Global.h"
#include "ShootsManager.h"
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

char *CS_StateName[CS_MAXSTATE] =
{
	"UNBORN",
	"BORN",
	"LIVING",
	"BURSTING",
	"DEAD"
};

///Sensitive transition names
char CS_NameTransition[CS_MAXTRANSITION][GCHAR_TAG_MAX_LONG] = 
{
	"DEFAULT",
	"BORNING",
	"GROWING",
	"MOVING",
	"DISPLAYING",
	"BURST",
	"REBORN",
	"DYING"
};

void CShip::Init()
{
	Alive				=	true;	// Every ship is alive
	Health = MaxHealth	=	CS_MAX_HEALTH;
	Hit_duration		=	CS_MAX_MAX_HIT_DUR;
	Type				=	CHARS_SHIP;
	SubType				=	CS_HIGH1;
	Speed.Reset();
	Speed.v[XDIM]		=	0.015f;	//Units/ms

	Size.Set(0.5f,	// Width
			 0.2f,	// Height
			 0.0f);
	RenderMode			=	CHAR_2D;
#ifdef CHAR_USE_AABB
	InitializeAABB();
#endif
	MoveTo(0.0f, 4.0f, 0.05f);

	zi					=	0.0f;
	zi_speed			=	(float)(rand()%8+3) * 0.001f;
	Explosion.Init();
	Explosion.Life		=	CS_MAX_EXPLOSION_LIFE;

	faseActual = false;

	ratio = sqrt((Size.v[XDIM])*(Size.v[XDIM]) + (Size.v[YDIM])*(Size.v[YDIM]));// + (Size.v[ZDIM])*(Size.v[ZDIM]));
	ratio *= 0.5f;
}

CShip::CShip()
{
	AI = NULL;

	Init();

	/*msgUpd = new cMsgShip;
	msgUpd->Type = RTDESKMSG_SHIP;
	msgUpd->msgSubType = TIPOUPD;
	msgUpd->Propietary = true;*/
}

/**
* Destroys the 'Ship' object.
* The actions for destroying the Ship are performed through to the base class 'CCharacter'.
*/
CShip::~CShip()
{

};

void CShip::AI_Move(void)
{
	if(faseActual){
		Speed.v[XDIM]*=-1;
		if(Navy.ShipsMoveDown)
			MoveRelTo(0.0f, CN_DOWN_STEP, 0.0f);	// Descending a bit down to the player

		#ifdef CHAR_USE_QUADTREE
		QuadtreeRoot::Instance()->getQuadtree()->RemoveCharacter(this);
		#endif
		faseActual = false;	
	}

	PositionPrev=Position;
	
	if(BouncingMode){
		MoveRelTo(Speed.v[XDIM]*msUpdShip , Speed.v[YDIM]*msUpdShip, 0.0f);	///Move the ship in X and Y dimension
		if (Position.v[XDIM] > CSIG_PG_RIGHT){
			Speed.v[XDIM]*=-1;
			Position.v[XDIM] = CSIG_PG_RIGHT-ratio;
		}
		if (Position.v[XDIM] < CSIG_PG_LEFT){
			Speed.v[XDIM]*=-1;
			Position.v[XDIM] = CSIG_PG_LEFT+ratio;
		}
		if (Position.v[YDIM] > CSIG_PG_SHIPS_CEILING){
			Speed.v[YDIM]*=-1;
			Position.v[YDIM] = CSIG_PG_SHIPS_CEILING-ratio;
		}
		if (Position.v[YDIM] < CSIG_PG_SHIPS_BOTTOM){
			Speed.v[YDIM]*=-1;
			Position.v[YDIM] = CSIG_PG_SHIPS_BOTTOM+ratio;
		}
	}
	else{
		MoveRelTo(Speed.v[XDIM]*msUpdShip , 0.0f, 0.0f);	///Move the ship in X and Y dimension
		if ((Position.v[XDIM] > CSIG_PG_RIGHT) || (Position.v[XDIM] < CSIG_PG_LEFT)){
			Navy.faseCambio = true;
			SoundsManager.Play(CGS_DESCEND,UGKSND_VOLUME_60);
		}
	}

	

	#ifdef CHAR_USE_QUADTREE
	QuadtreeRoot::Instance()->getQuadtree()->ActualizeCharacter(this);
	#endif
}

void CShip::AI_Dye(void)
{
	Alive = Active = false;

	#ifdef CHAR_USE_QUADTREE
	QuadtreeRoot::Instance()->getQuadtree()->RemoveCharacter(this);
	#endif
}

/**
* Initializes the artificial intelligence of the Ship.
*/
void CShip::AI_Init(SIAI_AI_TYPE AIType) 
{
	switch(AIType)
	{
		case SIAI_SHIP_DEFAULT:
			if (NULL != AI)
				delete AI;
			AI = AIManager.GetAI(SIAI_SHIP_DEFAULT);  ///<Asks the AIManager for the 'FSM' corresponding to the SupplyShip appropiately initialized.
		break;
		default:;
	}//Behaviour switch
}

void CShip::AI_Init(void)
{

	if (NULL != AI)	///<If the structure is not empty
		delete AI;	///<cleans all the data structure.

	AI_Init(SIAI_SHIP_DEFAULT); 
}

///Physics methods
void CShip::Collided (CCharacter *CollidedChar)
{
	switch (CollidedChar->Type)
	{
	case CHARS_PLAYER_SHOOT:
		if (CSH_PLAYER == ((CShoot*)CollidedChar)->SubType ||
			CSH_PLAYER3D == ((CShoot*)CollidedChar)->SubType ||
			CSH_AUXILIAR_LASER == ((CShoot*)CollidedChar)->SubType ||
			CSH_PLAYER3D_CHEVRON == ((CShoot*)CollidedChar)->SubType)
		{
			
			Health -= CollidedChar->Health;

			Hit_duration--;

			if (Health <= 0)
			{
				SoundsManager.Play(CGS_EXPLOSION,UGKSND_VOLUME_80);
				Explosion.Init(this);
				Explosion.Active = true;
				//AI->outEvent(CS_NameTransition[CS_BURST], NULL, (void*) this); //v 2->3

				Game.Score += (CS_MAXTYPE - Type) * 10;
				if(Game.Score > Game.HiScore) Game.HiScore=Game.Score;

				//AI->outEvent(CS_NameTransition[CS_DIE], NULL, (void*) this); //v 3->4
				AI_Dye();
				//AI->outEvent(CS_NameTransition[CS_REBORN], NULL, (void*) this); //v 3->1
			}
			
			break;
		}
	case CHARS_PLAYER:
			AI_Dye();
			Explosion.Init(this);
			Game.Score += (CS_MAXTYPE - Type) * 10;
			if(Game.Score > Game.HiScore) Game.HiScore=Game.Score;

		break;
	case CHARS_SHIP:
		if(BouncingMode){
			CShip *other;
			other = (CShip*) CollidedChar;
		
			GLfloat dx = Position.v[XDIM] - other->Position.v[XDIM];
			GLfloat dy = Position.v[YDIM] - other->Position.v[YDIM];
			//GLfloat dz = Position.v[ZDIM] - other->Position.v[ZDIM];

			GLfloat distToBall = dx*dx + dy*dy;// + dz*dz;

			if(distToBall <= (ratio + other->ratio)*(ratio + other->ratio)){
				GLfloat normal[3];
				normal[0] = other->Position.v[XDIM] - Position.v[XDIM];
				normal[1] = other->Position.v[YDIM] - Position.v[YDIM];
				//normal[2] = other->Position.v[ZDIM] - Position.v[ZDIM];

				GLfloat aux[3];
				aux[0] = Speed.v[XDIM] - other->Speed.v[XDIM];
				aux[1] = Speed.v[YDIM] - other->Speed.v[YDIM];
				//aux[2] = Speed.v[ZDIM] - other->Speed.v[ZDIM];

				GLfloat paralelo[3];
				GLfloat dotProd = aux[0]*normal[0] + aux[1]*normal[1];// + aux[2]*normal[2];
				GLfloat length = normal[0]*normal[0] + normal[1]*normal[1];// + normal[2]*normal[2];
				GLfloat dot = dotProd/length;
				paralelo[0]=normal[0]*dot;
				paralelo[1]=normal[1]*dot;
				//paralelo[2]=normal[2]*dot;

				GLfloat perpen[3];
				perpen[0] = aux[0] - paralelo[0];
				perpen[1] = aux[1] - paralelo[1];
				//perpen[2] = aux[2] - paralelo[2];

				Speed.v[XDIM] = perpen[0] + other->Speed.v[XDIM];
				Speed.v[YDIM] = perpen[1] + other->Speed.v[YDIM];
				//Speed.v[ZDIM] = perpen[2] + other->Speed.v[ZDIM];
				other->Speed.v[XDIM] = paralelo[0] + other->Speed.v[XDIM];
				other->Speed.v[YDIM] = paralelo[1] + other->Speed.v[YDIM];
				//other->Speed.v[ZDIM] = paralelo[2] + other->Speed.v[ZDIM];

			} 
			
		}
		break;
	}

}

//Displays the ship while hit
void CShip::DisplayHit(CMesh3d &model)
{
	Game.TimerManager.Timers[TmrApp].EndCounting();
	Hit_duration -= 10 * Game.TimerManager.Timers[TmrApp].GetLastPeriodms();
	if (Hit_duration <= 0)
		Hit_duration = 500;

	model.modelo.Draw();	// ship not textured, white
}

//Renders a ships in 3D or 2D way
void CShip::Render ()
{
	switch(RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character
		break;
	case CHAR_2D:
		glPushMatrix();
		//glEnable(GL_BLEND);
		AnimationsManager.Animations[IndAnimation2D]->UpdateAnimation();
		AnimationsManager.Animations[IndAnimation2D]->SetCurrentPhotogram();
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
	case CHAR_3D:

		// The ship 'floats'
		zi += zi_speed * msUpdShip / 15;

		switch(SubType)
		{
			case CS_HIGH1:
				Color3D.r = Color3D.g = Color3D.b = .5f;
				break;
			case CS_HIGH2:
				Color3D.r = Color3D.g = Color3D.b = .6f;
				break;
			case CS_HIGH3:
				Color3D.r = Color3D.g = Color3D.b = .7f;
				break;
			case CS_MIDDLE:
				Color3D.r = Color3D.g = Color3D.b = .85f;
				break;
			case CS_LOW:
			default:
				Color3D.r = Color3D.g = Color3D.b =  1.0f;
		}

		Color3D.a = 1.0f;
		
		Mesh->modelo.rot.x = 90.0f;
		Mesh->modelo.rot.y = 0.0f;
		Mesh->modelo.rot.z = 0.0f;

		Mesh->modelo.pos.x = Position.v[XDIM];
		Mesh->modelo.pos.y = Position.v[YDIM];
		Mesh->modelo.pos.z = cosf(zi * 4) / 15;			// Width : from -1/var to 1/var

		Mesh->modelo.scaleY = .0013;

		// if ship is being touched...
		if (Hit_duration < 500)
		{
			Hit_duration -= 10 * Game.RenderTemp;
			if (Hit_duration <= 0)
				Hit_duration = 500;

			Mesh->modelo.Draw();						// ship not textured, white
		}
		else
		{
			glColor4f (Color3D.r, Color3D.g, Color3D.b, Color3D.a);
			Mesh->modelo.Draw();
		}

		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);
		glPushMatrix();
		glTranslatef(Position.v[XDIM], Position.v[YDIM], Position.v[ZDIM]);
		glColor4f(0.8f, 0.8f, 0.0f, 0.5f);
		glutSolidSphere(ratio,16,16);
		glPopMatrix();
		glDisable(GL_BLEND);*/
	
		break;
	default: return;
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Displays a ship 3DS
//
//
void CShip::Render(CMesh3d &model)
{
	//model.modelo.scale = 0.0015f;

	model.modelo.rot.x = 90.0f;
	model.modelo.rot.y = 0.0f;
	model.modelo.rot.z = 0.0f;

	model.modelo.pos.x = Position.v[XDIM];
	model.modelo.pos.y = Position.v[YDIM];
	model.modelo.pos.z = cosf(zi * 4) / 15;			// amplitude : de -1/var à 1/var

	// affiche le ship touché
	if (Hit_duration < 500)
		DisplayHit(model);
	else
		model.modelo.Draw();
}

void CShip::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	RenderMode = Mode;
	Explosion.ChangeRenderMode(Mode);

	switch(RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character
		break;
	case CHAR_2D:
		break;
	case CHAR_LINES3D:
		break;
	case CHAR_3D:
		Size.Set(0.4f,	// Width
				 0.5f,	// Height
				 0.0f);

		ratio = sqrt((Size.v[XDIM])*(Size.v[XDIM]) + (Size.v[YDIM])*(Size.v[YDIM]));// + (Size.v[ZDIM])*(Size.v[ZDIM]));
		ratio *= 0.5f;

		if (Mesh)
			if(Mesh->Loaded = true)
				FitMeshIntoBoundingBox(*Mesh);
		break;
	default: return;
	}
}

void CShip::Update(void){
		
	if (Alive)					///If the Ship is alive
	{
		
		RTDESK_TIME nowTime, elapTime;

		nowTime = Game.TimerManager.Timers[TmrApp].TicksUntilNow();
		elapTime = nowTime - LastUpdTime;
		if(elapTime >= TicksToUpdateShip){

#ifndef DEF_CONTINUOUS_WITH_SIMULATE_TIME
			LastUpdTime = nowTime;

			//Next execution time calculation.
			double auxX= abs(Size.v[XDIM]/Speed.v[XDIM]);
			double auxY= abs(Size.v[YDIM]/Speed.v[YDIM]);
			double auxZ= abs(Size.v[ZDIM]/Speed.v[ZDIM]);
			msUpdShip = sqrt((auxX*auxX)+(auxY*auxY));//+(auxZ*auxZ));
			if(msUpdShip > CSIG_MIN_UPDATETIME_OBJECTS) msUpdShip = CSIG_MIN_UPDATETIME_OBJECTS;
			TicksToUpdateShip = Navy.Timer.ms2Ticks(msUpdShip);

			//Shooting calculation
			if (( floor((rand()%100000/(1+Navy.ShootsFrequency))/msUpdShip)) == 1) ///Has the Supply ship to fire?
				{
					Vector P, S;

					P.Set(Position.v[XDIM],
						  Position.v[YDIM] - .3f,
						  .05f);
					S.Set(0.0f,
						  -CS_SHOOT_SPEED,
						  0.0f);

					if(Navy.WithShoots)
						ShootsManager.NewShoot(CSH_SHIP, P, S);
					
				}

			AI->outEvent(CS_NameTransition[CS_MOVING], NULL, (void*) this); //v 2->2

			if(BouncingMode){
				for(unsigned int i=numId+1;i<Navy.Ship.size();i++)
					if(Navy.Ship[i]->Alive) Collided(Navy.Ship[i]);
			}
#else
			bool SynWithRealTime = false;
			LastUpdTime += TicksToUpdateShip;

			//Next execution time calculation.
			double auxX= abs(Size.v[XDIM]/Speed.v[XDIM]);
			double auxY= abs(Size.v[YDIM]/Speed.v[YDIM]);
			double auxZ= abs(Size.v[ZDIM]/Speed.v[ZDIM]);
			msUpdShip = sqrt((auxX*auxX)+(auxY*auxY));//+(auxZ*auxZ));
			if(msUpdShip > CSIG_MIN_UPDATETIME_OBJECTS) msUpdShip = CSIG_MIN_UPDATETIME_OBJECTS;
			TicksToUpdateShip = Navy.Timer.ms2Ticks(msUpdShip);

			//If next execution time is greater than real time then return. 
			//Else repeat until is synchronized with real time
			while(!SynWithRealTime){
				//Shooting calculation
				if (( floor((rand()%100000/(1+Navy.ShootsFrequency))/msUpdShip)) == 1) ///Has the Supply ship to fire?
					{
						Vector P, S;

						P.Set(Position.v[XDIM],
							  Position.v[YDIM] - .3f,
							  .05f);
						S.Set(0.0f,
							  -CS_SHOOT_SPEED,
							  0.0f);

						if(Navy.WithShoots)
							ShootsManager.NewShoot(CSH_SHIP, P, S);
						
					}

				AI->outEvent(CS_NameTransition[CS_MOVING], NULL, (void*) this); //v 2->2

				if(BouncingMode){
					for(unsigned int i=numId+1;i<Navy.Ship.size();i++)
						if(Navy.Ship[i]->Alive) Collided(Navy.Ship[i]);
				}
			
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

void CShip::DiscreteUpdate(void){
	
		
	if (Alive && !endByTime && !endByFrame)					///If the Ship is alive
	{
		#ifdef DEF_UPD_TIME
		Game.TimerManager.Timers[TmrUpd].InitCounting();
		#endif

		if(faseActual){
			Speed.v[XDIM]*=-1;
			if(Navy.ShipsMoveDown)
				MoveRelTo(0.0f, CN_DOWN_STEP, 0.0f);	// Descending a bit down to the player
			
			#ifdef CHAR_USE_QUADTREE
			QuadtreeRoot::Instance()->getQuadtree()->RemoveCharacter(this);
			#endif
			faseActual = false;	
		}

		//Shooting calculation
		if (( floor((rand()%100000/(1+Navy.ShootsFrequency))/msUpdShip)) == 1) ///Has the Supply ship to fire?
		{
			Vector P, S;

			P.Set(Position.v[XDIM],
				  Position.v[YDIM] - .3f,
				  .05f);
			S.Set(0.0f,
				  -CS_SHOOT_SPEED,
				  0.0f);

			if(Navy.WithShoots)
				DiscreteShootsManager.DiscreteNewShoot(CSH_SHIP, P, S);
			
		}

		PositionPrev=Position;
		
		if(BouncingMode){
			for(unsigned int i=numId+1;i<Navy.Ship.size();i++)
					if(Navy.Ship[i]->Alive) Collided(Navy.Ship[i]);

			MoveRelTo(Speed.v[XDIM]*msUpdShip , Speed.v[YDIM]*msUpdShip, 0.0f);	///Move the ship in X dimension

			if (Position.v[XDIM] > CSIG_PG_RIGHT){
				Speed.v[XDIM]*=-1;
				Position.v[XDIM] = CSIG_PG_RIGHT-ratio;
			}
			if (Position.v[XDIM] < CSIG_PG_LEFT){
				Speed.v[XDIM]*=-1;
				Position.v[XDIM] = CSIG_PG_LEFT+ratio;
			}
			if (Position.v[YDIM] > CSIG_PG_SHIPS_CEILING){
				Speed.v[YDIM]*=-1;
				Position.v[YDIM] = CSIG_PG_SHIPS_CEILING-ratio;
			}
			if (Position.v[YDIM] < CSIG_PG_SHIPS_BOTTOM){
				Speed.v[YDIM]*=-1;
				Position.v[YDIM] = CSIG_PG_SHIPS_BOTTOM+ratio;
			}
		}
		else{
			MoveRelTo(Speed.v[XDIM]*msUpdShip , 0.0f, 0.0f);	///Move the ship in X dimension		
			if (Position.v[XDIM] > CSIG_PG_RIGHT || Position.v[XDIM] < CSIG_PG_LEFT){
				#ifdef DEF_UPD_TIME
				EndAccCounting(TmrUpd);
				#endif
				#ifdef DEF_RTD_TIME
				Game.TimerManager.Timers[TmrRTDSKMM].InitCounting();
				#endif

				cMsgNavy *msg2Navy = (cMsgNavy*) GetMsgToFill(RTDESKMSG_NAVY);
				msg2Navy->msgSubType = TIPOTURNALL;
				SendMsg(msg2Navy,&Navy,TicksToUpdateShip/4);

				#ifdef DEF_RTD_TIME
				EndAccCounting(TmrRTDSKMM);
				#endif
				#ifdef DEF_RTD_TIME
				Game.TimerManager.Timers[TmrUpd].InitCounting();
				#endif
			}
		}	

		#ifdef CHAR_USE_QUADTREE
		QuadtreeRoot::Instance()->getQuadtree()->ActualizeCharacter(this);
		#endif
	
		double auxX= abs(Size.v[XDIM]/Speed.v[XDIM]);
		double auxY= abs(Size.v[YDIM]/Speed.v[YDIM]);
		double auxZ= abs(Size.v[ZDIM]/Speed.v[ZDIM]);

		msUpdShip = sqrt((auxX*auxX)+(auxY*auxY));//+(auxZ*auxZ));
		if(msUpdShip > CSIG_MIN_UPDATETIME_OBJECTS) msUpdShip = CSIG_MIN_UPDATETIME_OBJECTS;
		TicksToUpdateShip = Navy.Timer.ms2Ticks(msUpdShip);

		#ifdef DEF_UPD_TIME
		EndAccCounting(TmrUpd);
		#endif
		#ifdef DEF_RTD_TIME
		Game.TimerManager.Timers[TmrRTDSKMM].InitCounting();
		#endif

		msgUpd = (cMsgShip*) GetMsgToFill(RTDESKMSG_SHIP);
		msgUpd->Type = RTDESKMSG_SHIP;
		msgUpd->msgSubType = TIPOUPD;
		SendSelfMsg(msgUpd,TicksToUpdateShip);

		#ifdef DEF_RTD_TIME
		EndAccCounting(TmrRTDSKMM);
		#endif
	}
}

void CShip::ReceiveMessage(RTDESK_CMsg *pMsg){
		
	switch (pMsg->Type)
	{
	case RTDESKMSG_SHIP:
		cMsgShip *auxMsg;
			auxMsg = (cMsgShip*)pMsg;
			switch (auxMsg->msgSubType)
			{
			case TIPOUPD:

				DiscreteUpdate();

				break;
			case TIPOTURN:
				faseActual = true;
				break;
			}
		break;
	case RTDESKMSG_BASIC_TYPE:

		DiscreteUpdate();

		break;
	}
}

void *init_ship	(LPSTR *args, CShip *Ship)
{
	///This is the room for may be whatever additional actions to perform this current ship initialization
	return NULL;
}

void *CS_Move	(LPSTR *args, CShip *Ship)
{
	Ship->AI_Move();
	return NULL;
}

void *CS_Display	(LPSTR *args, CShip *Ship)
{
	Ship->Render();
	return NULL;
}

void *CS_Bursting	(LPSTR *args, CShip *Ship)
{
	Ship->Explosion.Render();
	return NULL;
}

void *CS_Kill	(LPSTR *args, CShip *Ship)
{
	delete Ship;
	return NULL;
}

