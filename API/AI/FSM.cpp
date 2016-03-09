/****************************************
*	Author:		Nathaniel Meyer			*
*	E-Mail:		nath_meyer@hotmail.com	*
*	Website:	http://www.nutty.ca		*
*										*
*   You are free to use, redistribute,  *
*   and alter this file in anyway, so   *
*   long as credit is given where due.	*
****************************************/

//Refactored by Ramón Mollá 2012-10

#include "FSM.h"
#include <Character.h>  

/**   
   * Finite State Machine Constructor
   * @ Param cStateList Default transitions are empty
   * @ Param cCurrentState Default transitions are empty
*/

void FSM::Init()
{
	cStateList		= NULL;
	cCurrentState	= NULL;
}

FSM::FSM ()
{
	Init();
}

/**
 * The destructor does not perform any action. It is responsibility of the AI Manager to destroy every behavior.
 */
FSM::~FSM ()
{

}

/**   
   * Finite State Machine Reset
   * @ Param cStateList se elimina las transiciones
*/
void FSM::Reset(void)
{
	delete [] cStateList;
	//Finally everything is as the beginning
	Init();
}
/**
	* Finite State Machine addStates
    * @param State *cStates status list of Finite State Machine transitions
    * If there is a list of transition resets the list and then redefine the list
    * With this new list of states Clones the name of the game actions and transitions
    * This function allows you to add transitions to Finite State Machine list
*/
void FSM::addStates (State *cStates, unsigned int numStates)
{
	/*cStateList = cStates;*/
	unsigned int i, j, k, maxEvents;//, maxTransitions;

	if (NULL != cStateList)
		Reset();           //Delete all states 

	cStateList = new State[numStates];

	for (i = 0; i < numStates; i++){
		//Clone name
		cStateList[i].setName(cStates[i].getName());

		// Clone actions
		cStateList[i].SpecificationEntry=cStates[i].SpecificationEntry;
		cStateList[i].SpecificationDo=cStates[i].SpecificationDo;
		cStateList[i].SpecificationExit=cStates[i].SpecificationExit;
		for (k = 0, maxEvents=cStates[i].getEvents(); k < maxEvents; k++) {

			cStateList[i].Specification.push_back(cStates[i].Specification[k]);
		}
	}


	// Clone transitions
	// This code comes after because states must be clonated. 
	for (i = 0; i < numStates; i++)
	{
    	for (j = 0; j < cStates[i].getTransitions(); j++) {
            //get the state destination for the current transition
			State aux=*cStates[i].Transition[j].cTo;
			//look for the clonated state with the same name
			bool found=false;
			int ind = -1;
			for (unsigned int k=0; k<numStates && !found; k++){
				found=(strcmp(aux.getName(),cStateList[k].getName())==0);
				if (found) ind=k;
			}
			//we have the index of the destination transition, now we create the transition
			if (found){
				cStateList[i].addTransition(cStates[i].Transition[j].Event.c_str(), &cStateList[ind]);
			}	
		}
	}

	cCurrentState = &cStateList[0];
}

/**
	* Extract event from character
    * Function Incomming events
	* @param Event: it is a event to be produced
    * @param LPSTR args: Set of arguments to pass to the next state (if any)
    * @param Object: The instatation of a class that has to change their status or performance of any
    * Incomming events are for starting the FSM
    * Return true if it is extracted from CurrentState 'Object' or otherwise False
*/
bool FSM::inEvent (char *Event, LPSTR *args, void *Object)
{
	State *st;

	try  
	{
		st = ((CCharacter*)Object)->AI->cCurrentState; ///<The cCurrentState is extracted from 'Object'
		if ( st != NULL )
		{
			return st->incoming(Event, args, Object);
		}
	}
	catch (...)
	{
		return false;
	}

	return false;
}
/**
 *	end a event from character
 */
 
 /**
	* Function Output the Event
    * @param Event:  Event which is to be produced
    * @param LPSTR: Set of arguments to pass to the next state (if any)
    * @param Object: The instatation of a class that has to change their status or performance of any
    * Return true if it is extracted from CurrentState 'Object' and creates a new current output for the event otherwise false
 */
bool FSM::outEvent (char *Event, LPSTR *args, void *Object)
{
	// Output the Event into the current state
	State *st; 
	try  
	{
		st = ((CCharacter*)Object)->AI->cCurrentState; ///<The cCurrentState is extracted from 'Object'
		if ( (cStateList != NULL) && (st != NULL) ) 
		{
			State *cTemp = st->outgoing(Event, args, Object); 
			if ( cTemp != NULL )
			{
				// Set the new output state as current and feed it the Event. 
				((CCharacter*)Object)->AI->cCurrentState = cTemp; 
				((CCharacter*)Object)->AI->cCurrentState->incoming(Event, args, Object);  

				return true;
			}
		}
	}
	catch(...){
		return false;
	}
	
	return false;
}

void FSM::SetState (unsigned int State) {cCurrentState = &cStateList[State];} //Set the list of states
/**   
   * Function Get the state from the list of states
   * @return STT_NO_VALID_STATE si CurrentState is no defined otherwise it returns the number of the list of states
*/
int FSM::GetState ()
{
	int i;

	if (NULL == cCurrentState)
		return STT_NO_VALID_STATE;
	
	for (i=0;cCurrentState != &cStateList[i];i++);
		return i;
}

/*
	Accessor Methods
*/
/**   
   * Accessor Methods
   * @return the name of CurrentState
*/
const char *FSM::getCurrentStateName ()
{
	return cCurrentState->getName();
}
