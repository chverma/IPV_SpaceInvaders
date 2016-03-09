/**	Class that loads the values by default in the very beginning of the game

*	Prefix FSM_: Class Initialization Parser

*	@author Jorge Izquierdo Ciges
*	@version 2007-09-11
*/

#include <Parsers\HTMLParser.h>
#include "State.h"
#ifndef FSTREAM_INITIALIZED
#define FSTREAM_INITIALIZED
#include <fstream>
#endif

#ifndef FSMPARSER_DEFINED /// Macro Parser is erased
#define FSMPARSER_DEFINED /// Macro Parser is defined

#define FSM_DEBUG	//Class HTML_FSM Parser Debug activated
#define FSM_TAG_MAX_LONG 40	//Maximun length of a sensitive tag

#define CSIG_MAX_LEVELS	99	// Max levels if Initialization.HTML_FSM is not correct

/** \typedef FSM_TagType

*	Types of tags the initialization parser can match during the parsing
*	They can be used also as states of of the stack machine (parser) during the parsing
*/
typedef enum {
	NIL_FSM,		///<Initial state. This tag is for management purpouses only. There is no correspondence to any string label in the HTML_FSM file
	HTML_FSM,		///<HTML_FSM tag
		HEAD_FSM,
			H_TITLE_FSM,
			H_VERSION_FSM,
			H_TYPE_FSM,
		BODY_FSM,
			STATE_INIT_FSM,
			STATE_CONT_FSM,
			VEC_STATE_FSM,
				STATE_NAME_FSM,
			VEC_TRANSITION_FSM,
				TRANSITION_FSM,
					T_NAME_FSM,
					T_ORIGIN_FSM,
					T_DESTINATION_FSM,
			VEC_ACTION_FSM,
				ON_ENTRY_FSM,
					ENTRY_TYPE_FSM,
					ENTRY_NAME_FSM,
					ENTRY_FUNC_FSM,
					ENTRY_ORIGIN_FSM,
				ON_DO_FSM,
					DO_TYPE_FSM,
					DO_NAME_FSM,
					DO_FUNC_FSM,
					DO_ORIGIN_FSM,
				ON_EXIT_FSM,
					EXIT_TYPE_FSM,
					EXIT_NAME_FSM,
					EXIT_FUNC_FSM,
					EXIT_ORIGIN_FSM,
				ON_EVENT_FSM,
					EVENT_TYPE_FSM,
					EVENT_NAME_FSM,
					EVENT_FUNC_FSM,
					EVENT_ORIGIN_FSM,
					EVENT_TRANSITION_FSM,//37
	UNKNOWN_FSM,
	MAXTAGS_FSM		///<This tag is for management purpouses only. There is no correspondence to any string label in the HTML_FSM file
} FSM_TagType;

typedef enum {
	FSM_MSG_FSM,
	FSM_MSG_FUNC_NOT_EXIST,
	FSM_MSG_MAX_LOGMSG
} CFSMP_LogMsg;

#define CFSMP_TAG_MAX_LONG 64	//Maximun length of a sensitive tag

/**	
	@class FSMReader
	* Define the Finite State Machine class reading with bypass or class inheritance ILiteHTMLReaderEvents
	* Where is the constructor and destructor of the class and are also actions for the parser based on FSM_TagType

*	@version 2007-09-11
*/
class FSMReader : public CHTMLParser
{
private:

	struct _function{
			mFuncPtr func;
			std::string name;
	};
	std::vector<_function> function_list;
	int searchPublicFunction(const char *name);

	//FSM states
	//TODO: Remove these following four attributes and change them by "FSM *AI;" and refactorize
	int n_states;
	int init_state;
	State *cStateList;	//FSM constructed by the FSM parser that is given to the AI Manager
	int it_state;

    /// Temporary variables are defined for the analyzer
	std::string temp_string;
	int temp_type;
	int temp_origin;
	int temp_destination;
	std::string temp_name_transition;
	std::string temp_name_function;

	std::ofstream	LogFile;
	
    void BeginParse(DWORD dwAppData, bool &bAbort); /// Begin parse
    void StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort); /// start tag
    void EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort); /// end the tag
    void Characters(const CString &rText, DWORD dwAppData, bool &bAbort); /// characters
    void Comment(const CString &rComment, DWORD dwAppData, bool &bAbort); /// comments
    void EndParse(DWORD dwAppData, bool bIsAborted); /// end parse
	void TagError(const char *Text); /// Etiquetas erroneas
	void ErrorParser(char *msj) {LogFile << "Parser error: " << msj << std::endl;} //mostrar error de analizado

	///Translates from a string to the token that the string represents
	unsigned int String2Tag (const char *Name);

public:

	void addPublicFunction	(const char *name, mFuncPtr funcPtr);

	void Init				(char *FileName);

	inline int		getFSMNumStates ()	{return n_states;};
	inline int		getFSMInitState ()	{return init_state;};
	inline State *	getFSMStatesVector(){return cStateList;};
	void			FSMReset();

	///Constructor
	FSMReader();
	///Destructor
	~FSMReader();

};

#endif
