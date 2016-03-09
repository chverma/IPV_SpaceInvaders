/**	Definition of the class Bonus

*	Prefix: CBN_

*	HISTORY

*	@author Ramon Molla
*	@version 2011-02
*/

#ifndef CBN_BONUS
#define CBN_BONUS

#include <Particles\Explosion.h>

#define CBN_WIDTH_2D  0.8f
#define CBN_HEIGTH_2D 0.8f

#define CBN_WIDTH_3D  3.0f
#define CBN_HEIGTH_3D 1.0f

#define CBN_YPOS 0.05f
#define CBN_ZPOS 0.05f

#define CBN_MAX_ALPHA 0.9f
#define CBN_FADING_SPEED 0.0005f

typedef enum {
	CBN_BONUS_3D,
	CBN_BONUS_LASER,
	CBN_BONUS_WEAPON,
	CBN_MAX_BONUSES
} CBN_BONUS_TYPE;

/**
	@class CBonus: public CCharacter

	Support the Bonus class for upgrading the power or visualization 

	@author Ramon Molla
	@version 2011-02
*/
class CBonus: public CCharacter
{
	//Atributes
public:

	CBN_BONUS_TYPE	SubType;		///<Bonus subtype: 3D, laser adn enhaced weapon 
	float			Shake1, Shake2;	///<The scene is shaking

	///When the ships loses its life, it burst. This is the particle system to perform the explosion
	CExplosion		Explosion;

	float	Alpha,			///<For waving the vanishment of the bonus while falling downwards to the player
			FadingSpeed;	///<Velocity of Fading in and out the alpha channel of the texture for the bonus

	double msUpdMsg;
	RTDESK_TIME ticksUpdkMsg;						//Tiempo Mensaje RTDESK
	RTDESK_CMsg *msgUpd;							//RTDESK Message Time

	//Methods
	void Init (void);

	CBonus();	///<Constructor of the class

	void Update (void);
	void DiscreteUpdate (void);

	void AI_Explode (void);	///<What to do when exploding a Bonus

	//Physics
	void Collided (CCharacter *CollidedChar);
	void PlayerCollided ();

	void Render(void);	///<Shows the bonus on the screen
	///Change the way the bonus is going to be rendered on the screen
	void ChangeRenderMode(CHAR_RENDER_MODE);

	void ReceiveMessage(RTDESK_CMsg *pMsg);
};

#endif