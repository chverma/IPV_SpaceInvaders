/*/////////////////////////////////////////////////////////////////////////////////////
//
// Shoot 
//
// These values are by default. They have to be overwritten in the initializatio phase
// when reading the default values from the "initialization.html" file

	@author Ramon Molla
	@version 2011-09-11
*/
#include "Global.h"
#include "Shoot.h"
#include <UGKPhysics.h>
#include "Global.h"
#include <vector>

extern QuadtreeRoot quadtreeRoot;

/**   
	* @fn void CShoot::Init (void)
	* Used when all the values are initialized by default
	* when reading the global initialization game file.
 */
void CShoot::Init (void)
{
	SubType			=	CSH_DEFAULT;
	Type			=	CHARS_MISSIL;
	Active = Alive  =	false;	///The very first time, when created at the beginning of the game, this device is not available
	
	//Space position and AABB							
	Size.v[XDIM]		=	.05f;	///Update by default the AABB relative to local coordinates
	Size.v[YDIM]		=	0.3f;
	Size.v[ZDIM]		=	0.0f;
	RenderMode		=	CHAR_2D;
	visible			=	true;
#ifdef CHAR_USE_AABB
	InitializeAABB();
#endif
	MoveTo(0.0f, 0.0f, 0.05f);
	Speed.v[YDIM] = CSH_DEFAULT_SPEED;
	Health=100;

	msgUpd = new RTDESK_CMsg;
	msgUpd->Type = RTDESKMSG_BASIC_TYPE;
	msgUpd->Propietary = true;
}

/**   
	* @fn void CShoot::SubtypeChange(CSH_SHOOT_TYPE	ShootType)
	* Any subtype change changes AABB and size
    * @ Param [in] ShootType Type of shoot
*/
void CShoot::SubtypeChange(CSH_SHOOT_TYPE	ShootType)
{
	SubType = ShootType;

	if (CHAR_2D == RenderMode)
	{
		Size.v[XDIM] = 0.05f;
		Size.v[YDIM] = 0.3f;
		Size.v[ZDIM] = 0.0f;
	}
	else switch (SubType)
		{
			case CSH_PLAYER:
				Health = 4;	//Amount of power a player shoot may substract from the enemy
				Size.v[XDIM] = 0.1f; 
				Size.v[YDIM] = 0.25f;
				Size.v[ZDIM] = 0.0f;
				break;
			case CSH_PLAYER3D:
				Health = 20;	//Amount of power a player shoot on 3D mode may substract from the enemy
				Size.v[XDIM] = 0.05f; //5.05	.1
				Size.v[YDIM] = 0.3f;	//.3	.25
				Size.v[ZDIM] = 0.0f;
			break;
			case CSH_PLAYER3D_CHEVRON:
				Health = 50;	//Amount of power a player shoot on 3D mode Chevron may substract from the enemy
				Size.v[XDIM] = 0.5f; //5.05 
				Size.v[YDIM] = 0.3f;  //.3
				Size.v[ZDIM] = 0.0f;
			break;
			case CSH_AUXILIAR_LASER:
				Health = 30;	//Amount of power a auxiliar player laser shoot on 3D mode may substract from the enemy
				Size.v[XDIM] = 0.03f;
				Size.v[YDIM] = 0.8f;
				Size.v[ZDIM] = 0.0f;
				if(Player[CurrentPlayer].ShootType == CSH_PLAYER3D_CHEVRON){
					Health = 50;	//Amount of power a auxiliar player laser shoot on 3D mode may substract from the enemy
					Size.v[XDIM] = 0.1f;
					Size.v[YDIM] = 0.8f;
					Size.v[ZDIM] = 0.0f;
				}
			break;
			case CSH_SHIP:
				Health = 3;	//Amount of power an enemy shoot may substract from the player
				Size.v[XDIM] = .033f;
				Size.v[YDIM] = 0.3f;
				Size.v[ZDIM] = 0.0f;				
			break;
			case CSH_CIRCLE_SHIP:
				Health = 10;	//Amount of power an enemy shoot may substract from the player
				Size.v[XDIM] = .035f;
				Size.v[YDIM] = 0.3f;
				Size.v[ZDIM] = 0.0f;				
			break;
			case CSH_SUPPLY_SHIP:
				Health = 10;	//Amount of power an enemy shoot may substract from the player
				Size.v[XDIM] = .035f;
				Size.v[YDIM] = 0.3f;
				Size.v[ZDIM] = 0.0f;				
			break;
			default:;
		}
#ifdef CHAR_USE_AABB
	UpdateAABB();
#endif
}

/**   
	* @fn void CShoot::ChangeRenderMode	(CHAR_RENDER_MODE Mode)
	* Changes the way a Shoot is visualize
    * @ Param [in] Mode New render mode to visualize the shoot: 2D, 2.5D, 3D,... 
*/
void CShoot::ChangeRenderMode	(CHAR_RENDER_MODE Mode)
{
	RenderMode = Mode;
}

/**   
	* @fn void CShoot::Render()
	* Lets the object to be visualized depending on the render mode
*/
void CShoot::Render()
{
	float	top = 0;

	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	//glEnable(GL_BLEND);

//	glAlphaFunc(GL_GREATER, 0.3f);								// for TGA alpha test
//	glEnable(GL_ALPHA_TEST);									// for TGA alpha test
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// for anti-aliasing

	///Render the shoot on the screen
	if(visible){
		switch(SubType)
		{
		case CSH_PLAYER:	/// Player Shoot 2D Render Mode
			switch (RenderMode)
			{
			case CHAR_LINES3D:
			case CHAR_3D:
				break;
			case CHAR_2D:
				TexturesManager.Textures[CTM_PLAYER_SHOOT]->SetTexture();
				break;
			}
			break;
		case CSH_SHIP:		/// Ship shoot
			switch (RenderMode)
			{
			case CHAR_2D:
				top = 0.3;
				TexturesManager.Textures[CTM_SHIP_SHOOT]->SetTexture();
				break;
			case CHAR_LINES3D:
			case CHAR_3D:
				top = 0.6;
				TexturesManager.Textures[CTM_SHIP_SHOOT]->SetTexture();
				break;
			}
			break;
		case CSH_SUPPLY_SHIP:   //SupplyShip shoot
			switch (RenderMode)
			{
			case CHAR_2D:
				top = 0.3;
				TexturesManager.Textures[CTM_SUPPLYSHIP_SHOOT]->SetTexture();
				break;
			case CHAR_LINES3D:
			case CHAR_3D:
				top = 0.6;
				TexturesManager.Textures[CTM_SUPPLYSHIP_SHOOT]->SetTexture();
				break;
			}
			break;
		case CSH_PLAYER3D_CHEVRON:	// Shoot 3D "chevron"
			switch (RenderMode)
			{
			case CHAR_LINES3D:
				TexturesManager.Textures[CTM_PLAYER_SHOOT3D_DARK_BCKG]->SetTexture();
				break;
			case CHAR_3D:
				TexturesManager.Textures[CTM_PLAYER_SHOOT3D_DARK_BCKG]->SetTexture();
				break;
			case CHAR_2D:
				TexturesManager.Textures[CTM_PLAYER_SHOOT]->SetTexture();
				return;
			default: //No render at all
				return;
			}			
			break;
		case CSH_PLAYER3D:	// Shoot 3D in particles
			switch (RenderMode)
			{
				case CHAR_LINES3D:
					glColor4f (1.0f, 1.0f, 1.0f, 0.7f);
					break;
				case CHAR_3D:
					TexturesManager.Textures[CTM_PLAYER_SHOOT3D]->SetTexture();			
					break;
				case CHAR_2D:
					return;
				default: //No render at all
					return;
			}
			break;
		case CSH_AUXILIAR_LASER:            // Shoot of a player auxiliar right or left laser
			switch (RenderMode)
			{
			case CHAR_LINES3D:
				top = 0.6;
				TexturesManager.Textures[CTM_PLAYER_SHOOT3D]->SetTexture();
				break;
			case CHAR_3D:
				top = 0.6;
				TexturesManager.Textures[CTM_PLAYER_SHOOT3D]->SetTexture();
				break;
			case CHAR_2D:
				return;
			default: //No render at all
				return;
			}
			break;
		case CSH_CIRCLE_SHIP:            // Shoot of a player auxiliar right or left laser
			switch (RenderMode)
			{
			case CHAR_LINES3D:
			case CHAR_3D:
				top = 0.6;
				TexturesManager.Textures[CTM_SUPPLYSHIP_SHOOT]->SetTexture();
				break;
			case CHAR_2D:
				top = 0.3;
				TexturesManager.Textures[CTM_SUPPLYSHIP_SHOOT]->SetTexture();
				break;
			}
			break;
		
		}	// Switch
		glBegin (GL_QUADS);
		glTexCoord2f (0,1);	glVertex2f (Position.v[XDIM] - Size.v[XDIM], Position.v[YDIM] + Size.v[YDIM]);
		glTexCoord2f (0,0);	glVertex2f (Position.v[XDIM] - Size.v[XDIM], Position.v[YDIM] - Size.v[YDIM]);
		glTexCoord2f (1,0);	glVertex2f (Position.v[XDIM] + Size.v[XDIM], Position.v[YDIM] - Size.v[YDIM]);
		glTexCoord2f (1,1);	glVertex2f (Position.v[XDIM] + Size.v[XDIM], Position.v[YDIM] + Size.v[YDIM]);
		glEnd ();
	}

	//glDisable(GL_BLEND);
}	// Method


/**   
	* @fn void CShoot::AI_Explode (void)
	* 
*/
void CShoot::AI_Explode (void)
{
	SoundsManager.Play(CGS_TOUCH,UGKSND_LOW_VOLUME);
	SoundsManager.SetPosition(CGS_TOUCH,UGKSND_STRD_POSITION );
	SoundsManager.SetVolume(CGS_TOUCH, UGKSND_ULTRA_LOW_VOLUME);
	AI_Dye();
}	

/**   
	* @fn void CShoot::Collided (CCharacter *CollidedChar)
	* What to do when a collision has happened
    * @ Param [in] CollidedChar Character collided to the shoot
*/
void CShoot::Collided (CCharacter *CollidedChar)
{
	bool Burnup = false;

	if (Alive)
		switch (CollidedChar->Type)
		{
		case CHARS_SHIP:
		case CHARS_SUPPLYSHIP:
		case CHARS_CIRCLESHIP:
			Burnup = CSH_PLAYER == SubType || CSH_PLAYER3D == SubType || CSH_AUXILIAR_LASER == SubType || CSH_PLAYER3D_CHEVRON == SubType;
			break;
		case CHARS_PLAYER:
			Burnup = CSH_SHIP == SubType|| CSH_SUPPLY_SHIP == SubType || CSH_CIRCLE_SHIP == SubType;
			break;
		case CHARS_BRICK:
		//case CHARS_BUNKER:
			Burnup = true;
			break;
		}

	if (Burnup)
	{
		AI_Explode();
	}
}

/**   
	* @fn void CShoot::Update ()
	* What the character has to do on every time tick 
*/
void CShoot::Update ()	///What the character has to do on every time tick 
 {
	RTDESK_TIME nowTime, elapTime;

	nowTime = Game.TimerManager.Timers[TmrApp].TicksUntilNow();
	elapTime = nowTime - LastUpdTime;
	if(elapTime >= ticksUpdMsg){

#ifndef DEF_CONTINUOUS_WITH_SIMULATE_TIME
		LastUpdTime = nowTime;

		vector<CCharacter*> v= QuadtreeRoot::Instance()->getQuadtree()->GetNearestObjectsTo(Position.v[XDIM],Position.v[YDIM],1 );

		for(unsigned int i=0;i<v.size();i++){	 
			if( ((v.at(i)->Position.v[XDIM]-(v.at(i)->Size.v[XDIM]/2.f)	<	Position.v[XDIM]/*+(Size.v[XDIM]/2.f)*/	)&&
				(v.at(i)->Position.v[XDIM]+(v.at(i)->Size.v[XDIM]/2.f)	>	Position.v[XDIM]/*-(Size.v[XDIM]/2.f)*/	)&&
				(v.at(i)->Position.v[YDIM]-(v.at(i)->Size.v[YDIM]/2.f)	<	Position.v[YDIM]/*+(Size.v[YDIM]/2.f)*/	)&&
				(v.at(i)->Position.v[YDIM]+(v.at(i)->Size.v[YDIM]/2.f)	>	Position.v[YDIM]/*-(Size.v[YDIM]/2.f)*/	) 
				) && Active
				){
					if((SubType==CSH_PLAYER || SubType==CSH_PLAYER3D || SubType==CSH_PLAYER3D_CHEVRON || SubType==CSH_AUXILIAR_LASER) && CHARS_COLLISION_TABLE[CHARS_PLAYER_SHOOT][v.at(i)->Type])
					{
						v.at(i)->Collided(this);
						Collided(v.at(i));
					}else if(CHARS_COLLISION_TABLE[CHARS_MISSIL][v.at(i)->Type])
					{
						v.at(i)->Collided(this);
						Collided(v.at(i));
					}
			}
		}

		/*	 if(CollisionDetector.Collided(this,v.at(i))){
				v.at(i)->Collided(this);
				Collided(v.at(i));
				}
				*/
					/*return
			a.min.x > b.max.x || a.max.x < b.min.x ||
			a.min.y > b.max.y || a.max.y < b.min.y ||
			a.min.z > b.max.z || a.max.z < b.min.z;
			*/		

		switch (SubType)
		{
			case CSH_PLAYER:			///Shoot sent by the player
			case CSH_PLAYER3D:			///Shoot sent by the player in 3D mode
			case CSH_PLAYER3D_CHEVRON:	///Shoot sent by the player
			case CSH_AUXILIAR_LASER:		///Shoot sent by the right and left laser auxiliar player ships
				if (Position.v[YDIM] > CSIG_PG_CEILING/*4.5f*/){		// Frustrum collection. Player's shoot is too much far away from the camera
					AI_Dye();}
				break;
			case CSH_SHIP:
			case CSH_SUPPLY_SHIP:
			case CSH_CIRCLE_SHIP:
				if (Position.v[YDIM] < CSIG_PG_BOTTOM/*-6.5f*/)	// If the Shoot exits from the playground downwards ...
				{
					AI_Dye();
				}
				break;
			
		}	//Switch

		PositionPrev=Position;
		//Move upwards or downwards depending on the original speed. No change in speed since the battle is in space
		MoveRelTo(Speed * msUpdMsg);	// Integrates space delta from its speed

		QuadtreeRoot::Instance()->getQuadtree()->ActualizeCharacter(this);
#else
		bool SynWithRealTime = false;
		LastUpdTime += ticksUpdMsg;

		//If next execution time is greater than real time then return. 
		//Else repeat until is synchronized with real time
		while(!SynWithRealTime){
			vector<CCharacter*> v= QuadtreeRoot::Instance()->getQuadtree()->GetNearestObjectsTo(Position.v[XDIM],Position.v[YDIM],1 );

			for(unsigned int i=0;i<v.size();i++){	 
				if( ((v.at(i)->Position.v[XDIM]-(v.at(i)->Size.v[XDIM]/2.f)	<	Position.v[XDIM]/*+(Size.v[XDIM]/2.f)*/	)&&
					(v.at(i)->Position.v[XDIM]+(v.at(i)->Size.v[XDIM]/2.f)	>	Position.v[XDIM]/*-(Size.v[XDIM]/2.f)*/	)&&
					(v.at(i)->Position.v[YDIM]-(v.at(i)->Size.v[YDIM]/2.f)	<	Position.v[YDIM]/*+(Size.v[YDIM]/2.f)*/	)&&
					(v.at(i)->Position.v[YDIM]+(v.at(i)->Size.v[YDIM]/2.f)	>	Position.v[YDIM]/*-(Size.v[YDIM]/2.f)*/	) 
					) && Active
					){
						if((SubType==CSH_PLAYER || SubType==CSH_PLAYER3D || SubType==CSH_PLAYER3D_CHEVRON || SubType==CSH_AUXILIAR_LASER) && CHARS_COLLISION_TABLE[CHARS_PLAYER_SHOOT][v.at(i)->Type])
						{
							v.at(i)->Collided(this);
							Collided(v.at(i));
						}else if(CHARS_COLLISION_TABLE[CHARS_MISSIL][v.at(i)->Type])
						{
							v.at(i)->Collided(this);
							Collided(v.at(i));
						}
				}
			}

			switch (SubType)
			{
				case CSH_PLAYER:			///Shoot sent by the player
				case CSH_PLAYER3D:			///Shoot sent by the player in 3D mode
				case CSH_PLAYER3D_CHEVRON:	///Shoot sent by the player
				case CSH_AUXILIAR_LASER:		///Shoot sent by the right and left laser auxiliar player ships
					if (Position.v[YDIM] > CSIG_PG_CEILING/*4.5f*/){		// Frustrum collection. Player's shoot is too much far away from the camera
						AI_Dye();}
					break;
				case CSH_SHIP:
				case CSH_SUPPLY_SHIP:
				case CSH_CIRCLE_SHIP:
					if (Position.v[YDIM] < CSIG_PG_BOTTOM/*-6.5f*/)	// If the Shoot exits from the playground downwards ...
					{
						AI_Dye();
					}
					break;
			
			}	//Switch

			PositionPrev=Position;
			//Move upwards or downwards depending on the original speed. No change in speed since the battle is in space
			MoveRelTo(Speed * msUpdMsg);	// Integrates space delta from its speed

			QuadtreeRoot::Instance()->getQuadtree()->ActualizeCharacter(this);
		
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

void CShoot::DiscreteUpdate ()	///What the character has to do on every time tick 
 {
	#ifdef DEF_UPD_TIME
	Game.TimerManager.Timers[TmrUpd].InitCounting();
	#endif

	 vector<CCharacter*> v= QuadtreeRoot::Instance()->getQuadtree()->GetNearestObjectsTo(Position.v[XDIM],Position.v[YDIM],1 );

	 for(unsigned int i=0;i<v.size();i++){	 
		 if( ((v.at(i)->Position.v[XDIM]-(v.at(i)->Size.v[XDIM]/2.f)	<	Position.v[XDIM]/*+(Size.v[XDIM]/2.f)*/	)&&
			 (v.at(i)->Position.v[XDIM]+(v.at(i)->Size.v[XDIM]/2.f)	>	Position.v[XDIM]/*-(Size.v[XDIM]/2.f)*/	)&&
			 (v.at(i)->Position.v[YDIM]-(v.at(i)->Size.v[YDIM]/2.f)	<	Position.v[YDIM]/*+(Size.v[YDIM]/2.f)*/	)&&
			 (v.at(i)->Position.v[YDIM]+(v.at(i)->Size.v[YDIM]/2.f)	>	Position.v[YDIM]/*-(Size.v[YDIM]/2.f)*/	) 
			 ) && Active
			 ){
				 if((SubType==CSH_PLAYER || SubType==CSH_PLAYER3D || SubType==CSH_PLAYER3D_CHEVRON || SubType==CSH_AUXILIAR_LASER) 
					 && CHARS_COLLISION_TABLE[CHARS_PLAYER_SHOOT][v.at(i)->Type])
				 {
					v.at(i)->Collided(this);
					Collided(v.at(i));
				 }else if(CHARS_COLLISION_TABLE[CHARS_MISSIL][v.at(i)->Type])
				 {
					v.at(i)->Collided(this);
					Collided(v.at(i));
				 }
		 }
	 }

	switch (SubType)
	{
		case CSH_PLAYER:			///Shoot sent by the player
		case CSH_PLAYER3D:			///Shoot sent by the player in 3D mode
		case CSH_PLAYER3D_CHEVRON:	///Shoot sent by the player
		case CSH_AUXILIAR_LASER:		///Shoot sent by the right and left laser auxiliar player ships
			if (Position.v[YDIM] > CSIG_PG_CEILING/*4.5f*/){		// Frustrum collection. Player's shoot is too much far away from the camera
				AI_Dye();}
			break;
		case CSH_SHIP:
		case CSH_SUPPLY_SHIP:
		case CSH_CIRCLE_SHIP:
			if (Position.v[YDIM] < CSIG_PG_BOTTOM/*-6.5f*/)	// If the Shoot exits from the playground downwards ...
			{
				AI_Dye();
			}
			break;
			
	}	//Switch

	PositionPrev=Position;
	//Move upwards or downwards depending on the original speed. No change in speed since the battle is in space
	MoveRelTo(Speed * msUpdMsg);	// Integrates space delta from its speed

	QuadtreeRoot::Instance()->getQuadtree()->ActualizeCharacter(this);

	#ifdef DEF_UPD_TIME
	EndAccCounting(TmrUpd);
	#endif

	if(Active && !endByTime && !endByFrame){
		#ifdef DEF_RTD_TIME
		Game.TimerManager.Timers[TmrRTDSKMM].InitCounting();
		#endif

		SendSelfMsg(msgUpd, ticksUpdMsg);

		#ifdef DEF_RTD_TIME
		EndAccCounting(TmrRTDSKMM);
		#endif 
	}
}

void CShoot::ReceiveMessage(RTDESK_CMsg *pMsg){
	DiscreteUpdate();
}


CShoot::~CShoot(){

	while(QuadtreeRoot::Instance()->getQuadtree()->RemoveCharacter(this));

	//delete(msgUpd);

}