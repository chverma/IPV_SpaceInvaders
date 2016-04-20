/**
*	Bonus manager
*	@author Ramon Molla
*	@version 2011-11
*/

#include "Global.h"
#include "BonusManager.h"

/////////////////////////////////////////////////
//
// Bonus manager Initialisation 
//
//

///Used when all the values are initialized by default
void CBonusManager::Init ()				
{
	RenderMode		= CHAR_2D;
	for(unsigned int i=0; i<CBN_MAX_BONUSES; i++)
		Bonus[i].Init();

	msgUpd = new RTDESK_CMsg;
	msgUpd->Type = RTDESKMSG_BASIC_TYPE;
	msgUpd->Propietary = true;
}

///Renders all the bonuses available
void CBonusManager::Render(void)
{
	switch(RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character: camera, collision objects,...
		return;
	case CHAR_2D:
	case CHAR_LINES3D:
	case CHAR_3D:
		for (unsigned int k=0; k < CBN_MAX_BONUSES; k++){
			if(Bonus[k].Alive || Bonus[k].Active)
				Bonus[k].Render();
			if(Bonus[k].Explosion.Alive || Bonus[k].Explosion.Active){
				Bonus[k].Explosion.Update();
				//Bonus[k].Explosion.Render();
			}
		}
		break;
	default: return;
	}
}

///Transfers the event to all shoots available
void CBonusManager::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	unsigned int k;
	RenderMode = Mode;

	for (k=0; k < CBN_MAX_BONUSES; k++)
		Bonus[k].ChangeRenderMode(Mode);	
}

///Generates randomly the corresponding bonus to the game status. Only once a time
void CBonusManager::GenerateRandomBonus()
{
	// BONUS -> RANDOM BONUS CREATION
	if (CSIG_PLAYING == (CSIG_GAME_STATE) Game.AI->GetState())
		//It is allowed to produce a new bonus beacause the game is in play mode
		if (CHAR_2D == RenderMode)
		{
			//It is only allowed to produce a CBN_BONUS_3D Bonus to pass to 3D mode
			//Bonus to pass to 3d mode
			if (false == Bonus[CBN_BONUS_3D].Active)
				//There is no other CBN_BONUS_3D bonus active
				if (floor((rand()%10000)/Game.UpdateTemp) == 1)
					if(DiscreteSimulation) DiscreteGenerateBonus(CBN_BONUS_3D);
					else GenerateBonus(CBN_BONUS_3D);
				
		 }
		//Bonus to regenerate new lasers
		else if (!Player[CurrentPlayer].Laser[CP_LEFT_LASER].Alive || !Player[CurrentPlayer].Laser[CP_RIGHT_LASER].Alive)
		{
			if (false == Bonus[CBN_BONUS_LASER].Active) 
				if (floor((rand()%15000)/Game.UpdateTemp) == 1)
					if(DiscreteSimulation) DiscreteGenerateBonus(CBN_BONUS_LASER);
					else GenerateBonus(CBN_BONUS_LASER);
		}
		//Player has the two lasers (right and left)
		//So, a weapon improvement is allowed
		else if (CSH_PLAYER3D == Player[CurrentPlayer].ShootType)
				if (false == Bonus[CBN_BONUS_WEAPON].Active) 
					if (floor((rand()%300000)/Game.UpdateTemp) == 1)
						if(DiscreteSimulation) DiscreteGenerateBonus(CBN_BONUS_WEAPON);
						else GenerateBonus(CBN_BONUS_WEAPON);
}

///Shoots management for the Player and enemy navy
void CBonusManager::Maintenance()
{
	unsigned int i;

	GenerateRandomBonus();
	for (i=0;i<CBN_MAX_BONUSES;i++)
		if (Bonus[i].Alive)
			if (CollisionDetector.Collided(&Bonus[i], &Player[CurrentPlayer]))
			{
				Bonus[i].PlayerCollided();
				switch (Bonus[i].SubType)
				{
				case CBN_BONUS_3D:
					Scene.AngleStart.v[XDIM] = fmod(Scene.Angle.v[XDIM], 360);				
					Scene.AngleStart.v[YDIM] = fmod(Scene.Angle.v[YDIM], 360);
					Bonus[i].AI_Dye();
					Game.GameEvent(CSIG_CHANGING_2_3D);							//v 4->6
					break;
				case CBN_BONUS_LASER:
					if (!Player[CurrentPlayer].Laser[CP_LEFT_LASER].Alive || !Player[CurrentPlayer].Laser[CP_RIGHT_LASER].Alive){
						Player[CurrentPlayer].ActivateLaser(CP_LEFT_LASER);
						Player[CurrentPlayer].ActivateLaser(CP_RIGHT_LASER);
						Bonus[i].AI_Dye();
					}
					break;
				case CBN_BONUS_WEAPON:
					if (Player[CurrentPlayer].ShootType==CSH_PLAYER3D){
						Player[CurrentPlayer].ShootType = CSH_PLAYER3D_CHEVRON;
						Bonus[i].AI_Dye();
					}
					break;
				}
			}else if (CollisionDetector.Collided(&Bonus[i], &Player2[CurrentPlayer]))
			{
				Bonus[i].PlayerCollided();
				switch (Bonus[i].SubType)
				{
				case CBN_BONUS_3D:
					Scene.AngleStart.v[XDIM] = fmod(Scene.Angle.v[XDIM], 360);				
					Scene.AngleStart.v[YDIM] = fmod(Scene.Angle.v[YDIM], 360);
					Bonus[i].AI_Dye();
					Game.GameEvent(CSIG_CHANGING_2_3D);							//v 4->6
					break;
				case CBN_BONUS_LASER:
					if (!Player2[CurrentPlayer].Laser[CP_LEFT_LASER].Alive || !Player2[CurrentPlayer].Laser[CP_RIGHT_LASER].Alive){
						Player2[CurrentPlayer].ActivateLaser(CP_LEFT_LASER);
						Player2[CurrentPlayer].ActivateLaser(CP_RIGHT_LASER);
						Bonus[i].AI_Dye();
					}
					break;
				case CBN_BONUS_WEAPON:
					if (Player2[CurrentPlayer].ShootType==CSH_PLAYER3D){
						Player2[CurrentPlayer].ShootType = CSH_PLAYER3D_CHEVRON;
						Bonus[i].AI_Dye();
					}
					break;
				}
			}

}

void CBonusManager::UpdateActiveBonus(void){
	#ifdef DEF_UPD_TIME
	Game.TimerManager.Timers[TmrUpd].InitCounting();
	#endif

	for (int i=0;i<CBN_MAX_BONUSES;i++){
		if (Bonus[i].Alive)
			Bonus[i].Update();
	}

	#ifdef DEF_UPD_TIME
	EndAccCounting(TmrUpd);
	#endif
}

void CBonusManager::GenerateBonus(CBN_BONUS_TYPE BonusType)
{
	Bonus[BonusType].Type		= CHARS_BONUS;
	Bonus[BonusType].SubType	= BonusType;
	Bonus[BonusType].Active		= Bonus[BonusType].Alive = true;
	Bonus[BonusType].Position.v[YDIM] = CBM_BONUS_INITIAL_Y3D;

#ifdef CHAR_USE_AABB
	Bonus[BonusType].UpdateAABB();
#endif

	msUpdBonus = abs(Bonus[BonusType].Size.v[YDIM]/Bonus[BonusType].Speed.v[YDIM]);
	if(msUpdBonus > CSIG_MIN_UPDATETIME_OBJECTS) msUpdBonus = CSIG_MIN_UPDATETIME_OBJECTS;
	Bonus[BonusType].msUpdMsg = msUpdBonus;
	ticksUpdBonus = Game.UpdateShootsTimer.ms2Ticks(msUpdBonus);
	Bonus[BonusType].ticksUpdkMsg = ticksUpdBonus;
}

void CBonusManager::DiscreteGenerateBonus(CBN_BONUS_TYPE BonusType)
{
	Bonus[BonusType].Type		= CHARS_BONUS;
	Bonus[BonusType].SubType	= BonusType;
	Bonus[BonusType].Active		= Bonus[BonusType].Alive = true;
	Bonus[BonusType].Position.v[YDIM] = CBM_BONUS_INITIAL_Y3D;

#ifdef CHAR_USE_AABB
	Bonus[BonusType].UpdateAABB();
#endif

	msUpdBonus = abs(Bonus[BonusType].Size.v[YDIM]/Bonus[BonusType].Speed.v[YDIM]);
	if(msUpdBonus > CSIG_MIN_UPDATETIME_OBJECTS) msUpdBonus = CSIG_MIN_UPDATETIME_OBJECTS;
	Bonus[BonusType].msUpdMsg = msUpdBonus;
	ticksUpdBonus = Game.UpdateShootsTimer.ms2Ticks(msUpdBonus);
	Bonus[BonusType].ticksUpdkMsg = ticksUpdBonus;

	#ifdef DEF_RTD_TIME
	EndAccCounting(TmrUpd);
	#endif



	#ifdef DEF_RTD_TIME
	Game.TimerManager.Timers[TmrRTDSKMM].InitCounting();
	#endif

	Bonus[BonusType].SetMsgDispatcher(Game.RTDESK_Engine->MsgDispatcher);
	msg = GetMsgToFill(RTDESKMSG_BASIC_TYPE);
	msg->Type = RTDESKMSG_BASIC_TYPE;
	SendMsg(msg,&Bonus[BonusType],ticksUpdBonus);

	#ifdef DEF_RTD_TIME
	EndAccCounting(TmrRTDSKMM);
	#endif


	#ifdef DEF_RTD_TIME
	Game.TimerManager.Timers[TmrUpd].InitCounting();
	#endif
}

void CBonusManager::ReceiveMessage(RTDESK_CMsg *pMsg){
	//Maintenance();	 
}