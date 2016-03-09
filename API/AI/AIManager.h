/**	Definition of the class Artificial Intelligence Manager

*Prefix: CAIM_

*	@author Ramon Molla
*	@version 2011-11
*/

#ifndef CAIM_AIMANAGER
#define CAIM_AIMANAGER

#include "FSM.h"

class CAIManager
{
	//Atributes

	///Character Artificial Intelligence supported by a Finite State Machine (FSM)
	///ACHTUNG: For future versions, use STL vector type in order to allow behaviour changes on the fly
	std::vector<FSM	*>AI;

public:
	//Methods
	void Init();
    ///Constructor of the class
	CAIManager	(){Init();};	

    ///Destructor of the class
	~CAIManager	();				

	///Reserves space for the whole amount of FSM to hold
	inline void SetAmountAI (unsigned int A) {AI.resize(A);};
	void SetAI	(FSM *NewAI, unsigned int AIType);	///<Generates a typefied predefined behaviour
	FSM *GetAI	(unsigned int AIType);	///<Give a typefied predefined behaviour
};

#endif