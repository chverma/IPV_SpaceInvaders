/****************************************
*	Initial Author:		Nathaniel Meyer	*
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
 *	state.h
 *	
 *	Refactored by Ramón Mollá <rmolla@dsic.upv.es>
 *
 *	HISTORY:
 *
 *	Adapted to be used by any kind of objects in any given program. It goes on using callbacks 
 *	but these calbacks have a new object parameter that provides the local information to execute 
 *	properly.
 *	The code have been doxygenated in order to imrpove the readability and support automatic documentation
 *	@version 11-2011
 *	Prefix: STT_
 */

#ifndef State_h
#define State_h

#include <string>
#include <vector>

#ifndef WINDOWS_LOADED
#define WINDOWS_LOADED
#include <windows.h>
#endif

#define STT_NO_VALID_STATE -1

/*
	State Class
		- A state consists of a set of specifications and transitions
		- A state can only do what its told to and nothing more
*/
typedef void* (*mFuncPtr) (LPSTR *args, void *Object);	//Object must casted to the one needed

/*
	eWhen
		- Actions to define within the state
*/
enum eWhen
{
	eOnEntry = 0,	/// Action taken on entering the state
	eDo,			/// Action taken from entering the state until leaving the state. 
					/// Action done by default if no Event is received
	eOnExit,		/// Action taken on leaving the state
	eOnEvent		/// Action taken based on the Event received
					/// Specify Event, Arguments, and Condition
};

/*
	eType
		- Specifies the action type
		- Only action is supported
*/
enum eType
{
	eAction = 0,	// Action is a function
					// Specify Name
	eSendEvent,		// Send an Event
					// - Specify Name, Arguments, and Target
	eDontUse		// Don't Use
};

class State
{
	private:

		std::string Name;	// Name of the state

//			LPSTR *mArgs;

	public:

		struct _transition
		{
			//State *cFrom;			// From State	NOT NEEDED
			State *cTo;				// To State

			std::string Event;		// Event ties to outgoing state
		};
		/*
			Specification
				- There are exactly 4 specifications as defined in eWhen
		*/
		struct _specification
		{
			mFuncPtr func;						// Pointer to the action
			std::string name,					// Name of the action
						Event;					// Event this specification is tied to
			int type;							// Type of action
		};	// 3 main actions + 4-mMaxEvents

		std::vector<_transition>Transition;
		std::vector<_specification>Specification;
		_specification SpecificationEntry;
		_specification SpecificationDo;
		_specification SpecificationExit;

		/*
			Constructor / Destructor
		*/
		State ();
		~State ();

		/*
			Clean
				- Cleans/Resets all data within the state
		*/
		void clean ();

		/*
			Transitions
				@param: Event
					- Name which identifies the proper action to take
				@param: args
					- If provided, it will be passed on to the action
				- Return true if switch is successful
		*/
		void addTransition	(const char *Event, State *cState);
		bool incoming		(const char *Event, LPSTR *args, void *Object);
		State *outgoing		(const char *Event, LPSTR *args, void *Object);

		/*
			addAction
				@param: when
					- Specify the action activation (0-4)
				@param: type
					- Specify the action type (0-1)
				@param: name
					- Name of the action to call
				@param: Event
					- Name of the Event to cause the action
				@param: funcPtr
					- Specify what function will be executed for this state
		*/
		//void addAction (int when, int type, char *name, void *funcPtr);	// Set default action
		// Fix from Meyer's code
		void addAction (eWhen when, eType type, const char *name, mFuncPtr funcPtr);	// Set default action
		//void addAction (int when, int type, char *name, char *Event, void *funcPtr);	// Set OnEvent action
		// Fix from Meyer's code
		void addAction (eWhen when, eType type, const char *name, const char *Event, mFuncPtr funcPtr);	// Set OnEvent action

		//Inline functions
		/*	Operator Methods */
		inline void setName (const char *name) {Name=std::string(name);}

		/*	Accessor Methods */
		inline const char		*getName ()			{return Name.c_str();}
		inline unsigned int		getEvents ()		{return Specification.size();}
		inline unsigned int		getTransitions ()	{return Transition.size();}
};


#endif