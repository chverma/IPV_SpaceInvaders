/** 
//
// AI declarations
//
	@author Ramon Molla
	@version 2012-01
*/

#include "AIManager.h"

void CAIManager::Init(){for (unsigned int i = 0; i < AI.size(); i++) AI[i] = NULL;}
/**
* Destroys all the behaviors of the game and their states lists.
*/
CAIManager::~CAIManager(){for (unsigned int i = 0; i < AI.size(); i++) if (NULL != AI[i]) delete AI[i]; AI.clear();}

///Stablishes a new typefied predefined behaviour
void CAIManager::SetAI	(FSM *NewAI, unsigned int AIType)
{
	if (AIType<AI.size())
	{
		if (NULL != AI[AIType])
			delete AI[AIType];
	}
	else AI.resize(AIType+1, NULL);

	AI[AIType] = NewAI;
}

/**
 * A copied 'FSM' is returned.
 * The 'cStateList' points to the one the AI Manager has for this type of AI.
 * The 'cCurrentState' is the initial.
 */
FSM* CAIManager::GetAI	(unsigned int AIType)
{
	FSM *result;

	if (AIType<AI.size())
			result = AI[AIType];  ///<Setting 'cStateList' and 'cCurrentState' the same than AI[AIType]
	else	result = NULL;

	return result;
}