/**	Definition of the class Artificial Intelligence Manager

*Prefix: CAIM_

*	@author Ramon Molla
*	@version 2011-11
*/

#ifndef CAIM_AIMANAGER
#define CAIM_AIMANAGER

#include "Copyright.h"
#include "FSM.h"

typedef enum {
	CAIM_GAME_DEFAULT,
	CAIM_SHIP_DEFAULT,
	CAIM_SUPPLYSHIP_DEFAULT,
	CAIM_BONUS_DEFAULT,
	CAIM_MAX_AI
} CAIM_AI_TYPE;

class CAIManager
{
	//Atributes

	///Character Artificial Intelligence supported by a Finite State Machine (FSM)
	///ACHTUNG: For future versions, use STL vector type in order to allow behaviour changes on the fly
	FSM	*AI[CAIM_MAX_AI];

public:
	//Methods
	void Init();
    ///Constructor of the class
	CAIManager	(){Init();};	

    ///Destructor of the class
	~CAIManager	();				

	void SetAI	(FSM *NewAI, CAIM_AI_TYPE AIType);	///<Generates a typefied predefined behaviour
	FSM *GetAI	(CAIM_AI_TYPE AIType);	///<Give a typefied predefined behaviour
};

#endif