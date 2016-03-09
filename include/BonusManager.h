/**	Definition of the class Bonus Manager

*	Prefix: CBM_

*	@author Ramon Molla
*	@version 2011-11
*/

#ifndef CBM_BONUSMANAGER
#define CBM_BONUSMANAGER

#include "Bonus.h"

#define CBM_BONUS_INITIAL_Y3D	CSIG_PG_CEILING+2.0f*CBN_HEIGTH_3D

class CBonusManager: public CCharacter
{
	//Atributes
public:

	///Amount of bonuses the manager can take care of
	CBonus	Bonus[CBN_MAX_BONUSES];
	///The way the bonuses are rendered on the screen
	CHAR_RENDER_MODE RenderMode;	

	RTDESK_TIME LastUpdTime;
	RTDESK_TIME ticksRTdeskMsg;

	RTDESK_CMsg *msg;							//RTDESK Message Time
	RTDESK_CMsg *msgUpd;							//RTDESK Message Time
	double msUpdBonus;
	RTDESK_TIME ticksUpdBonus;

	void Init();	///<Initialization by default method
	CBonusManager(){Init();};	///<Constructor of the class

	///Generates an event to all the bonuses active at a given moment in order to do something
	void Maintenance();
	void UpdateActiveBonus(void);

	void CBonusManager::GenerateRandomBonus(); ///<Generates randomly the corresponding bonus. Only once a time

	void GenerateBonus(CBN_BONUS_TYPE BonusType);	///<Generates a new bonus for the player
	void DiscreteGenerateBonus(CBN_BONUS_TYPE BonusType);	///<Generates a new bonus for the player

	void Render				(void);	///<Shows the bonuses on the screen
	
	void ChangeRenderMode	(CHAR_RENDER_MODE);///Change the way all the bonuses are going to be rendered on the screen

	void ReceiveMessage(RTDESK_CMsg *pMsg);
};

#endif