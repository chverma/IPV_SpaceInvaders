/**	Definition of the class Shoot

*	Prefix: CSHM_

*	@author Ramon Molla
*	@version 2011-11
*
*	Last modified: 2012-2013 by Vicente Perez
*/

#ifndef CSHM_SHOOTSMNG
#define CSHM_SHOOTSMNG

#include "Shoot.h"
#include "Quadtree.h"
#include <list>


/// Manages all the shoots available during the playing time
class CShootsManager: public CCharacter
{
	CHAR_RENDER_MODE RenderMode;	///The way the shoots are rendered on the screen

public:

	list<CShoot>	ShootsAct;		///List of active shoots
	list<CShoot>	ShootsInact;	///List of inactive shoots

	//RT-DESK
	double timeRTdeskMsg;						//Tiempo Mensaje RTDESK
	RTDESK_CMsg *msg;							//RTDESK Message Time
	RTDESK_CMsg *msgUpd;						//RTDESK Message Time
	double msUpdShoot;
	RTDESK_TIME ticksUpdShoot;

	bool nextShootVisible;

	//Methods
	void Init (void);

	CShootsManager(){Init();}												///Constructor of the class
	void NewShoot(CSH_SHOOT_TYPE SType, Vector &Pos, Vector &Spd);			///Generates a new shoot
	void DiscreteNewShoot(CSH_SHOOT_TYPE SType, Vector &Pos, Vector &Spd);	///Generates a new shoot

	void Maintenance(void);													///Lists shoot maintenance
	void UpdateActiveShoots(void);											///Loop to update all active shoots in Continuous Mode

	void ReceiveMessage(RTDESK_CMsg *pMsg);

	void Render				(void);		///<Shows all the shoots on the screen
	///Change the way all the shoots are going to be rendered on the screen
	void ChangeRenderMode	(CHAR_RENDER_MODE);
};

#endif