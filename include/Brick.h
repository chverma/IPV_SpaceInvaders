/**	Definition of the class Brick

*	Prefix: CB_

*	@author Ramon Molla
*	@version 2011-08
*/

#ifndef CB_BRICK
#define CB_BRICK

//#include "Copyright.h"
#include <Character.h>

#define CB_HEALTH_DEFAULT	1.0f
#define CB_MAX_MAX_HIT_DUR	400

/**
*	\typedef CB_BRICK_STATE
*	Different states a brick can reach during the playing
*/
typedef enum {
	CB_UNBORN=0,	///<For management purpouses only
	CB_BORN,		///<The character is just born but it is not still operative (living)
	CB_LIVING,		///<Everything this character may do while it is alive 
	CB_DYING,		///<The character has been touched so many times that its life has gone negative. So it has to burst before being died. Starts an explosion and dies
	CB_DEAD,		///<The character is no operative. Reached after dying
	CB_MAXSTATE		///<For management purpouses only
} CB_BRICK_STATE;

/**
*	\typedef CB_BRICK_TYPE
*	Different types of bricks can be used by a bunker during the playing
*/
typedef enum {
	CB_NO_BRICK=0,		///<There is no brick 
	CB_DEFAULT,			///<Brick with default hardness
	CB_HARD,			///<Hard brick. It last more than default brick
	CB_INDESTRUCTIBLE,	///<This brick is never destroyed even by the most powerful weapons
	CB_MAXTYPE			///<For management purpouses only
} CB_BRICK_TYPE;

/**
*	\typedef CB_BRICK_SOUNDS
*	Different sounds a brick can perform during the playing: explode, touch a laser,...
*/
typedef enum {
	CB_NO_SOUND=0,
	CB_TOUCH,		///<Sound produced when a shoot hits the brick
	CB_MAX_SOUND	///<For management purpouses only
} CB_BRICK_SOUNDS;

/**
*	\class CBrick
*	The little bricks used to build the bunkers that defend the player against the enemy navy
*	The colour/material used may vary depending on its type
*/
class CBrick: public CCharacter
{
	
	
public:

	unsigned int Column;
	unsigned int Row;

	//Attributes
	CB_BRICK_TYPE SubType;

	//Methods
	void Init ();

	CBrick(){}	///<Constructor of the class

	///Character AI virtual methods rewritten
	void AI_Dye (void);	
	void AI_Init(void); ///<Virtual procedure overwritten

	//Physics
	void Collided (CCharacter *CollidedChar);

	void Render(void);	///<Shows the brick on the screen
	///Change the way the brick is going to be rendered on the screen
	inline void ChangeRenderMode(CHAR_RENDER_MODE Mode) {RenderMode = Mode;}
};

#endif