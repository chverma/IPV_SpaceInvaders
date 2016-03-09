/*Definition of the class game
  General class to support all the attribute general to all the ships, supply ships, general tactics,...
  Prefix: CN_

	@author Ramon Molla
	@version 2011-11
*/

#ifndef CN_CLASS_NAVY //se borra la macro de la nave
#define CN_CLASS_NAVY //se define la macro de la nave

#include "Game.h"
#include "Ship.h"
#include "SupplyShip.h"
#include "CircleShip.h"
#include <ExecutionMode.h>


#ifndef DEQUE_LOADED
#define DEQUE_LOADED
#include <deque>			// Uses Standard Template Library Double Ended Queue
using namespace std;
#endif

//Definitions for the Navy
#define CN_MAX_X_SHIFT		 10		///Initial value of the attribute ShipsBalancer
#define CN_SHIPS_MAX_SPEED	 0.001f	///Maximun speed for any given ship in any moment of the game in units/ms
#define CN_INITIAL_SPEED	-0.0005f	///Initial speed for any given ship at the begining of the game in units/ms
#define CN_INITIAL_ACC		 0.00001f	///Acceleration for any given ship every time the nay bounces in units/ms

#define CN_INITIAL_X_POS	 -7.0f	///Initial speed for any given ship at the begining of the game
#define CN_INITIAL_Y_POS	 2.0f	///Initial speed for any given ship at the begining of the game

#define CN_MAX_SHIPS		50		///Amount of Ships available during the same playing level
#define CN_MAX_ROWS			 5		///Amount of ship rows the navy can have
#define CN_MAX_COLUMNS		10		///Amount of ship columns the navy can have

#define CN_SHIP_X_SHIFT		1.0f
#define CN_SHIP_Y_SHIFT		0.8f

#define CN_DOWN_STEP		-0.3f

#define CN_LIMIT_CHANGE2PHOTO1	100	//When reached this limit, change to photogram1
#define CN_LIMIT_CHANGE2PHOTO2	 50	//When reached this limit, change to photogram2

#define CN_DEFAULT_INDEXES	-1		///Indexes of Textures and Meshes are initialized to -1.

#define CN_MAX_CIRCLESHIPS	10		///Amount of cicleship 

/**   
   * @class CNavy: public CCharacter
   * Defined the class Ship with inherit of the class CCharacter
   * And in this class it defined the variables for the ship, direction, shut, life, animation, suministries
   * Well as updates of the ship when you get items and bonus
   
	@author Ramon Molla
	@version 2011-11
*/

class CNavy: public CCharacter
{
public:
	//Attributes

	//Game general attributes
	char	*Version;	// Game VERSION
	char	*Title;		// Game TITLE

	//Ships Control
	///Max shift the navy may perform when not touching the sides
	///When positive, every ship moves to the right or left depending of the limit reached before
	///When reached the value CN_MAX_X_SHIFT, a change in the moving direction to the left is done
	///When a negative value is reached, a change in the moving direction to the right is done
	float			Bounce;
	///Navy moves to the right or to the left, downwards or upwards
	UGKPHY_DIRECTION	Direction;
	///Increases the amount of shootings simultaneously on the same screen
	float			ShootsFrequency;
	///Ships health level per level
	int				ShipLevDefaultHealth;
	///Ships health by default
	int				ShipDefaultHealth;

	bool			ReGenShips;			///<Resurrect ships

	CSound DescendingSound;

	//Supply ships control
	///Pointer to SupplyShip array
	unsigned int	CurrentSupplyShip;
	///When loading a level, this is the current ship being loaded right now
	///Belongs to the interval [0,CN_MAXSHIPS[
	unsigned int	CurrentShip;
	//
	unsigned int	CurrentCircleShip;
	///All the ships that belong to the navy
	deque<CShip*> Ship;
	///All the supply ships that belong to the navy
	deque<CSupplyShip*> SupplyShip;
	///All the circle ships that belong to the navy
	deque<CCircleShip*> CircleShip;

	//Position from which the ships are allocated.
	Vector StartShipsPos;

	unsigned int faseCambio;
	int ShipsMoveDown;
	int WithShoots;

	//RT-DESK
	double timeRTdeskMsg;						//Tiempo Mensaje RTDESK Render
	cMsgNavy	*msg;							//RTDESK Message Time

	RTDESK_TIME NextShip2Upd;
	RTDESK_TIME NextSShip2Upd;
	RTDESK_TIME NextCShip2Upd;
	
	cHRTimer Timer;

//methods

	///Performs all the geometric transformations on every ship alive in the navy
	void Update				(void);
	void UpdateShips		(void);
	void UpdateSupplyShips	(void);
	void UpdateCircleShips	(void);

	//void Collided(CCharacter *O);	//Virtual method that resolves the collision
	void SetMeshes (void);	///<Assigns a loaded mesh from the MeshesManager to every ship and supplyship 

	///Shows the navy on the screen
	void Render				(void);
	///Change the way the navy is going to render all the ships and supply ships on the screen
	void ChangeRenderMode	(CHAR_RENDER_MODE);

	void ReceiveMessage(RTDESK_CMsg *pMsg);


	void Init (void);
	void Deinitialize (void);

	//Constructor
	CNavy () {Init();};

	///Character AI virtual methods rewritten
	void GenerateDefaultShipAI			(void);	///Default Ship Artificial Intelligence initialization
	void GenerateDefaultSupplyShipAI	(void);	///Default SupplyShip Artificial Intelligence initialization
	void GenerateDefaultCircleShipAI	(void);
	void AI_Init	(void);	///Default Artificial Intelligence character initialization
	void AI_Dye		(void);

	///AI method
	void AI_Go2Play(void);
};

#endif
