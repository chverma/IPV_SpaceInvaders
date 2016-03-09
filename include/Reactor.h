/**	Definition of the class Player
	Class prefix CR_

	@author Ramon Molla
	@version 2011-09-11
*/

#ifndef CR_CLASS_REACTOR
#define CR_CLASS_REACTOR

#include <Character.h>

#define CR_MAX_HEALTH			 20	///Maximun amount of health of the player by default
#define CR_MAX_MAX_HIT_DUR		500	///Maximun amount of time when the player is hit

class CReactor: public CCharacter
{
	//Attributes
	float	alpha;
	bool	alpha_down;
	float	ln,			// particules externes des lasers + grandes pour le mode 3D_LINES
			ln2;		// particules internes des réacteurs + petites pour le mode *non* 3D_LINES

public:
	//Methods
	CReactor();		//Constructor

	///Shows the reactor on the screen
	void Render				(void);
	///Change the way the reactor is going to be rendered on the screen
	void ChangeRenderMode	(CHAR_RENDER_MODE);
};

#endif