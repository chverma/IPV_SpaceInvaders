/**	Definition of the class Player
	Class prefix CP_

	@author Ramon Molla
	@version 2011-09-11
*/

#ifndef CP_CLASS_PLAYER2
#define CP_CLASS_PLAYER2

#include "Laser.h"
#include "Shoot.h"
#include "Reactor.h"
#include <HRTimer.h>
#include <ExecutionMode.h>

#define CP_MAX_HEALTH2			 20	///<Maximun amount of health of the player by default
#define CP_MAX_MAX_HIT_DUR2		500	///<Maximun amount of time when the player is hit
#define CP_INFINITE_LIVES2		 -1 ///<Constant value for ethernal life
#define CP_DEFAULT_LIVES2		  3 ///<If not specified, by default, ther are three lives
#define CP_MAX_PLAYERS2			  1 ///<By default, if not specified, there may play at the same time only 4 players
#define CP_NO_PLAYER2		 -1	//
#define CP_DEFAULT_PLAYER2		  0	//
#define CP_MAX_SPEED2			0.01f	//Maximun X speed in units/ms
#define CP_MAX_ACCEL2			0.01f	//Maximun X acceleration in units/ms
#define CP_2D_SIZE_X2				0.6f	//Maximun X acceleration in units/ms
#define CP_2D_SIZE_Y2				0.4f	//Maximun X acceleration in units/ms
#define CP_3D_SIZE_X2				0.8f	//Maximun X acceleration in units/ms
#define CP_3D_SIZE_Y2				1.0f	//Maximun X acceleration in units/ms

#define CP_LASER_REL_POS2		0.05f	///<Left or right laser relative position respect to the palyer on the X axis

//Shooting
#define CP_SHOOT2D_SPEED2		0.008f
#define CP_SHOOT3D_SPEED2		0.017f

typedef enum {
	CP_LEFT_LASER2,					///<Code for left laser
	CP_RIGHT_LASER2,					///<Code for right laser
	CP_MAX_LASERS2					///<Amount of extra lasers the player has
} CP_PLAYER_LASER2;

typedef enum {
	CP_UNBORN2=0,	///<For management purpouses only
	CP_BORN2,		///<The character is just born but it is not still operative (living)
	CP_LIVING2,		///<Everything this character may do while it is alive 
	CP_DYING2,		///<The character has been touched so many times that its life has gone negative. So it has to burst before being died. Starts an explosion and dies
	CP_DEAD2,		///<The character is no operative. Reached after dying
	CP_MAXSTATE2		///<For management purpouses only
} CP_PLAYER_STATE2;


typedef enum {
	CP_MOVE_LEFT2,			///<The player wants to move to the left
	CP_MOVE_RIGHT2,			///<The player wants to move to the right
	CP_SHOOT2,				///<The player wants to fire a shoot
	CP_MAX_PLAYER_EVENTS2	// Only for management purpouses
} CP_PLAYER_EVENTS2;

class CPlayer2: public CCharacter
{
	//Attributes
public:
	unsigned short int	Lives;		///<Amount of lives a player can lose before finishing the game
	float		yi;			// le vaisseau est secoué (passage à game 3D)
	float		yi_speed;	// le vaisseau est secoué (vitesse)
	float		yi_cos;		// le vaisseau est secoué (variable de cosinus)
	float		zi;			// le vaisseau 'flotte' (coord z, pour mode 3D)
	float		zi_speed;	// le vaisseau 'flotte' (vitesse)
	float		Scrolling;	// Speed used in the scrolling, Control to rotate the Player
	CExplosion	Explosion;	///<When the ships loses its life, it burst. This is the particle system to perform the explosion

	//Shooting
	CSH_SHOOT_TYPE	ShootType;	///<TYPE of shooting power in a given moment. It depends on the bonus got during the playing
	float			ShootSpeed;	///<Initial speed of the current shoot

	CP_PLAYER_STATE State;

	//RT-DESK
	double timeRTdeskMsg;						//Tiempo Mensaje RTDESK
	RTDESK_CMsg *msg;							//RTDESK Message Time

	RTDESK_TIME TicksToUpdatePlayer;
	cHRTimer UpdatePlayerTimer;
	double msUpdPlayer;


	//Weapons
	CLaser		Laser	[CP_MAX_LASERS];	///<Extra lasers a player can have in a given moment
	CReactor	Reactor;					///<Reactors the player's ship have to perform the movement

	//Methods
	bool Init ();	//Used when all the values are initialized by default
					//when reading the global initialization game file
					//Returns true if the initialization of the attirbute has been correctly done

	CPlayer2();		//Constructor

	///Character AI virtual methods rewritten
	void AI_Init	(void);	///<Default Artificial Intelligence character initialization
	void AI_Dye		(void);
	void AI_Explode (void);
	void ActivateLaser	(CP_PLAYER_LASER Laser);
	void ActivateReactor(void);

	//Physics
	void Collided (CCharacter *CollidedChar);

	///Shows the player on the screen
	void Render				(void);
	///Change the way the player is going to be rendered on the screen
	void ChangeRenderMode	(CHAR_RENDER_MODE);
	void RenderExplosion	(void);
	void DiscreteUpdate		(void);
	void Update				(void);

	//Transformation methods
	void MoveTo		(float x, float y, float z);	///<Moves to the absolute coordinate x,y,z
	void MoveTo		(Vector &Move);					///<Moving to an absolute position. It does not matter where the character is
	void MoveRelTo	(float x, float y, float z);	///<Moves x,y,z units from the to the current position
	void MoveRelTo	(Vector &Move);					///<Moving relative to the character current Position

	//User interaction. One method for every player command sent to the player's avatar
	void ManageEvent(unsigned int);	///<Virtual method for managing events
	void UserRight	();
	void UserLeft	();
	void UserShoot	();
	void DiscreteUserShoot	();
	
	void ReceiveMessage(RTDESK_CMsg *pMsg);	
};

#endif