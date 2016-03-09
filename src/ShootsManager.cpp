/*/////////////////////////////////////////////////////////////////////////////////////
//
// Shoots manager
//

	@author Ramon Molla
	@version 2011-11

	Last modified: 2012-2013 by Vicente Perez
*/

#include "Global.h"
#include "ShootsManager.h"
#include "Quadtree.h"

///Shoots manager Initilization
void CShootsManager::Init() //Used when all the values are initialized by default
{
	RenderMode = CHAR_2D;
	nextShootVisible = true;

	msgUpd = new RTDESK_CMsg;
	msgUpd->Type = RTDESKMSG_BASIC_TYPE;
	msgUpd->Propietary = true;

}

///Generates a new shoot
void CShootsManager::NewShoot(CSH_SHOOT_TYPE SType, Vector &Pos, Vector &Spd)
{
	CShoot shootTmp;
	//If there are inactive shoots available, last one is picked
	if (ShootsInact.size() > 0)
	{
		shootTmp = ShootsInact.back();
		ShootsInact.pop_back();
	}

	shootTmp.Speed = Spd;
	shootTmp.Alive = true;
	shootTmp.Active = true;
	shootTmp.MoveTo(Pos);	
	shootTmp.ChangeRenderMode(RenderMode);
	shootTmp.SubtypeChange(SType);
	shootTmp.visible = nextShootVisible;

	if(SType == CSH_PLAYER || SType == CSH_PLAYER3D || SType == CSH_PLAYER3D_CHEVRON ||SType == CSH_AUXILIAR_LASER)
		shootTmp.Type = CHARS_PLAYER_SHOOT;

	msUpdShoot = abs(shootTmp.Size.v[YDIM]/shootTmp.Speed.v[YDIM]);
	if(msUpdShoot > CSIG_MIN_UPDATETIME_OBJECTS) msUpdShoot = CSIG_MIN_UPDATETIME_OBJECTS;
	shootTmp.msUpdMsg = msUpdShoot;
	ticksUpdShoot = Game.UpdateShootsTimer.ms2Ticks(msUpdShoot);
	shootTmp.ticksUpdMsg = ticksUpdShoot;

	ShootsAct.push_back(shootTmp);

	if(!nextShootVisible) nextShootVisible=true;
}

void CShootsManager::DiscreteNewShoot(CSH_SHOOT_TYPE SType, Vector &Pos, Vector &Spd)
{
	CShoot shootTmp;
	//If there are inactive shoots available, last one is picked
	if (ShootsInact.size() > 0)
	{
		shootTmp = ShootsInact.back();
		ShootsInact.pop_back();
	}
	
	shootTmp.Speed = Spd;
	shootTmp.Alive = true;
	shootTmp.Active = true;
	shootTmp.MoveTo(Pos);
	shootTmp.ChangeRenderMode(RenderMode);
	shootTmp.SubtypeChange(SType);
	shootTmp.visible = nextShootVisible;

	if(SType == CSH_PLAYER || SType == CSH_PLAYER3D || SType == CSH_PLAYER3D_CHEVRON ||SType == CSH_AUXILIAR_LASER)
		shootTmp.Type = CHARS_PLAYER_SHOOT;
	
	msUpdShoot = abs(shootTmp.Size.v[YDIM]/shootTmp.Speed.v[YDIM]);
	if(msUpdShoot > CSIG_MIN_UPDATETIME_OBJECTS) msUpdShoot = CSIG_MIN_UPDATETIME_OBJECTS;
	shootTmp.msUpdMsg = msUpdShoot;
	ticksUpdShoot = Game.UpdateShootsTimer.ms2Ticks(msUpdShoot);
	shootTmp.ticksUpdMsg = ticksUpdShoot;

	ShootsAct.push_back(shootTmp);

	#ifdef DEF_UPD_TIME
	EndAccCounting(TmrUpd);
	#endif
	#ifdef DEF_RTD_TIME
	Game.TimerManager.Timers[TmrRTDSKMM].InitCounting();
	#endif
	
	ShootsAct.back().SetMsgDispatcher(Game.RTDESK_Engine->MsgDispatcher);
	msg = GetMsgToFill(RTDESKMSG_BASIC_TYPE);
	msg->Type = RTDESKMSG_BASIC_TYPE;
	SendMsg(msg,&ShootsAct.back(),ticksUpdShoot);

	#ifdef DEF_RTD_TIME
	EndAccCounting(TmrRTDSKMM);
	#endif 
	#ifdef DEF_RTD_TIME
	Game.TimerManager.Timers[TmrUpd].InitCounting();
	#endif

	if(!nextShootVisible) nextShootVisible=true;
}

///Render shoots on the screen
void CShootsManager::Render(void)
{
	list<CShoot>::iterator i;

	//Draw active shoots
	for (i=ShootsAct.begin(); i != ShootsAct.end(); i++)
		if(i->Alive)
			i->Render();
}

/// Change the mode the shoots are rendered
void CShootsManager::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	RenderMode = Mode;

	list<CShoot>::iterator i;
	
	for (i=ShootsAct.begin(); i != ShootsAct.end(); i++)
		i->ChangeRenderMode(Mode);

	//Change the mode for inactive shoots too
	for (i=ShootsInact.begin(); i != ShootsInact.end(); i++)
		i->ChangeRenderMode(Mode);
}

/// Shoots list Maintenance
void CShootsManager::Maintenance()
{
	std::list<CShoot>::iterator Shoot;

	Shoot = ShootsAct.begin();
	while (Shoot != ShootsAct.end())
	{
		if (!Shoot->Active) //Inactive shoots are passed to the inactive list
		{
			ShootsInact.push_back(*Shoot);
			Shoot = ShootsAct.erase(Shoot);
		}
		else Shoot++;
	}

}

void CShootsManager::UpdateActiveShoots(void)
{
	#ifdef DEF_UPD_TIME
	Game.TimerManager.Timers[TmrUpd].InitCounting();
	#endif

	std::list<CShoot>::iterator Shoot;

	Shoot = ShootsAct.begin();
	while (Shoot != ShootsAct.end())
	{
		if (Shoot->Active)Shoot->Update();
		Shoot++;
	}

	#ifdef DEF_UPD_TIME
	EndAccCounting(TmrUpd);
	#endif
}

void CShootsManager::ReceiveMessage(RTDESK_CMsg *pMsg){
	Maintenance();	 
}


