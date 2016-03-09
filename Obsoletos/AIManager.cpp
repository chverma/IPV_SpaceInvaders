/** 
//
// AI declarations
//
	@author Ramon Molla
	@version 2012-01
*/

#include "AIManager.h"

void CAIManager::Init(){for (unsigned int i = 0; i < CAIM_MAX_AI; i++) AI[i] = NULL;}
/**
* Destroys all the behaviors of the game and their states lists.
*/
CAIManager::~CAIManager(){for (unsigned int i = 0; i < CAIM_MAX_AI; i++) if (NULL != AI[i]) delete AI[i];}

///Stablishes a new typefied predefined behaviour
void CAIManager::SetAI	(FSM *NewAI, CAIM_AI_TYPE AIType)
{
	if (NULL != AI[AIType]) 
		delete AI[AIType];
	AI[AIType] = NewAI;
}

/**
 * A copied 'FSM' is returned.
 * The 'cStateList' points to the one the AIManager has for this type of AI.
 * The 'cCurrentState' is the initial.
 */
FSM* CAIManager::GetAI	(CAIM_AI_TYPE AIType)
{
	FSM *result;

	if (NULL == AI[AIType]) 
			result = NULL;
	else	result = AI[AIType];  ///<Setting 'cStateList' and 'cCurrentState' the same than AI[AIType]
	return result;
}