/****************************************
*	Author:		Nathaniel Meyer			*
*	E-Mail:		nath_meyer@hotmail.com	*
*	Website:	http://www.nutty.ca		*
*										*
*   You are free to use, redistribute,  *
*   and alter this file in anyway, so   *
*   long as credit is given where due.	*
****************************************/

/**
 *	PROJECT - Finite State Machine support for Deterministic Finite Automata
 *
 *	FSM.h
 *	
 *	Refactored by Raman Molla <rmolla@dsic.upv.es>
 *
 *	HISTORY:
 *
 *	Adapted to be used by any kind of objects in any given program. It goes on using callbacks 
 *	but these calbacks have a new object parameter that provides the local information to execute 
 *	properly.
 *	The code have been doxygenated in order to imrpove the readability and support automatic documentation
 *	@version 11-2011
 *	Prefix: FSM_
 */

#ifndef FSM_h /// Macro Finite State Machine is erased
#define FSM_h /// Macro Finite State Machine is defined

#include "State.h"


/**
*	FSM
*		- The FSM class controls a set of states
*		- All state transitions depend on how the states are configured
*		- Configure a set of states and then upload them to the FSM
*		@version 1.0 (Mar 06, 2004)
*		@author Gurmeet S. Kochar - Refactored by Ramón Mollá
*/
/**   
	* Define the Finite State Machine class
    * Where you define the constructor and destructor of the class
    * @param variable State cStateList to specify Deterministic Finite Automata transitions
    * @param variable State cCurrentState to specify  Finite State Machine transitions
    * @function Reset state transitions Releases
*/
class FSM
{
	private:

	public:

		State *cStateList;		///A pointer to the DFA
		State *cCurrentState;	///The state currently used. Every FSM points to the given active state at this very moment

		/*
			Constructor / Destructor
		*/
		FSM ();
		~FSM ();

		void Init();

		void Reset(void); ///Releases all the states, transitions and so on before setting to null all its data structures

		/*
			addStates
				@param: cState
					- Pointer to a list of states that the FSM will use
		*/
		void addStates (State *cState, unsigned int numStates);

		/*
			events
				@param: Event
					- Event that has occured
				@param: args
					- Set of arguments to pass into the next state (if any)
				@param: Object
					- The instatation of a class that has to change its state or perform anything
				- Returns true if successful
				- Incomming events are for starting the FSM
				- Outgoing events are also "incoming" when transitioning between states
		*/
		bool	inEvent	(char *Event, LPSTR *args, void *Object);
		bool	outEvent(char *Event, LPSTR *args, void *Object);

		void	SetState(unsigned int State);	///Sets the current state to the state indicated
		int		GetState(void);					///Returns the number of the current state

		/*
			Accessor Methods
		*/
		const char *getCurrentStateName ();
};

#endif
