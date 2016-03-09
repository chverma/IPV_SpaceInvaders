/****************************************
*	Author:		Nathaniel Meyer			*
*	E-Mail:		nath_meyer@hotmail.com	*
*	Website:	http://www.nutty.ca		*
*										*
*   You are free to use, redistribute,  *
*   and alter this file in anyway, so   *
*   long as credit is given where due.	*
****************************************/
//Refactored by Ramón Mollá on 2011-08

#include <State.h>


/*
	Constructor / Destructor
*/
State::State ()
{
	clean();
}

State::~State ()
{
}

/*
	Clean
*/
void State::clean ()
{
	Name.clear();

	// Clean transitions
	Transition.clear();
	Specification.clear();
	
	SpecificationDo.func=0;
	SpecificationEntry.func=0;
	SpecificationExit.func=0;
}

/*
	Transitions
*/
void State::addTransition (const char *Event, State *cState)
{
	// Add a transition to the list
	_transition temp;
	temp.Event=std::string(Event);
	temp.cTo=cState;

	Transition.push_back(temp);
	
}

bool State::incoming (const char *Event, LPSTR *args, void *Object)
{

	for(unsigned int i=0, itrMax=Specification.size();i<itrMax;i++){

		if(Specification[i].Event.compare(std::string(Event))==0){
			if (Specification[i].func != NULL){
				Specification[i].func(args, Object);
			}
			return true;
		}
	}
	if (SpecificationEntry.func != NULL){
		SpecificationEntry.func(args, Object);
	}
	if (SpecificationDo.func != NULL){
		SpecificationDo.func(args, Object);
	}
	return true;
}

State* State::outgoing (const char *Event, LPSTR *args, void *Object){

	// Find the State to which this Event is tied to
	for(unsigned int i=0, itrTransMax=Transition.size();i<itrTransMax;i++){
		if(Transition[i].Event.compare(std::string(Event))==0){
			// Run the exit action
			if (SpecificationExit.func != NULL){
				SpecificationExit.func(args, Object);
			}
			return Transition[i].cTo;
		}
	}

	return NULL;
}

/*
	addAction
*/
//void State::addAction (int when, int type, char *name, void *funcPtr)
void State::addAction (eWhen when, eType type, const char *name, mFuncPtr funcPtr){
	switch(when){
		case eOnEntry:
			if ( type == eAction ){
				SpecificationEntry.name=std::string(name);
				SpecificationEntry.type=type;
				SpecificationEntry.func = funcPtr;
			}
			break;
		case eDo:
			if ( type == eAction ){
				SpecificationDo.name=std::string(name);
				SpecificationDo.type=type;
				SpecificationDo.func = funcPtr;
			}
			break;
		case eOnExit:
			if ( type == eAction ){
				SpecificationExit.name=std::string(name);
				SpecificationExit.type=type;
				SpecificationExit.func = funcPtr;
			}
			break;
	}

}

//void State::addAction (int when, int type, char *name, char *Event, void *funcPtr)
void State::addAction (eWhen when, eType type, const char *name, const char *Event, mFuncPtr funcPtr)
{
	if(when == eOnEvent){

		if(type == eAction){

			_specification temp;
			temp.name=std::string(name);
			temp.Event=Event;
			temp.type=type;
			temp.func=funcPtr;

			Specification.push_back(temp);
		}
	}
}