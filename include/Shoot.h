/**	Definition of the class Shoot

*	Prefix: CSH_

*	@author Ramon Molla
*	@version 2011-11
*/

#ifndef CSH_SHOOT
#define CSH_SHOOT

#include "Character.h"
#include "Quadtree.h"
#include "QuadtreeRoot.h"
/**
*	\typedef CSH_SHOOT_TYPE 
*	Different types of shoots that can be active at any moment while playing the game
*
*/

#define CSH_DEFAULT_SPEED	0.2F

typedef enum {
	CSH_DEFAULT,			///<For management purpouses only
	CSH_2D,					///<When the shoot is in 2D mode
	CSH_PLAYER,				///<Shoot sent by the player
	CSH_PLAYER3D,			///<Shoot sent by the player in 3D mode
	CSH_PLAYER3D_CHEVRON,	///<Shoot sent by the player
	CSH_AUXILIAR_LASER,		///<Shoot sent by the right and left laser auxiliar player ships
	CSH_SHIP,				///<Shoot sent by a enemy ship. This kind of shoot has to be always after all player's shootings
	CSH_SUPPLY_SHIP,		///<Shoot sent by a enemy Supply Ship. This kind of shoot has to be always after all player's shootings
	CSH_CIRCLE_SHIP,		///<Shoot sent by a enemy Circle Ship. This kind of shoot has to be always after all player's shootings
	CSH_BULLET,				///
	CSH_MAX_SHOOTS_TYPES	///<Not a real shoot. This is for accounting purpouses only
} CSH_SHOOT_TYPE;

/**
*	\class CShoot
*	Supports every shoot sent by any character in the game
*/
class CShoot: public CCharacter
{
	public:

	//RT-DESK
	double msUpdMsg;
	RTDESK_TIME ticksUpdMsg;						//Tiempo Mensaje RTDESK
	RTDESK_CMsg *msg;
	RTDESK_CMsg *msgUpd;							//RTDESK Message Time

	//Attributes
	CSH_SHOOT_TYPE	SubType;
	bool			visible;
	
	//particles2 particle2[MAX_PARTICLES2];// Shoot 3D

	//Methods
	void Init (void);

	CShoot(){Init();}								///<Constructor of the class
	void SubtypeChange(CSH_SHOOT_TYPE	SubType);	///<Any subtype change has to update AABB and size

	///Character AI virtual methods rewritten
	inline void AI_Dye (void) {QuadtreeRoot::Instance()->getQuadtree()->RemoveCharacter(this);Init();}
	void AI_Explode (void);

	//Physics
	void Collided (CCharacter *CollidedChar);

	void Render				(void);	///<Shows the shoot on the screen
	void Update				(void);
	void DiscreteUpdate		(void);
	///Change the way the shoot is going to be rendered on the screen
	void ChangeRenderMode	(CHAR_RENDER_MODE);

	void ReceiveMessage(RTDESK_CMsg *pMsg);

	~CShoot();
};

#endif