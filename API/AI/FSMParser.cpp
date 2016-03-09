/**	Declaration of all the parser methods to understand the initialization files
*	@author Ramon Molla
*	@version 2012-08
*/

#include "FSMParser.h"

/**   
   * Finite State Machine Labels
   * @ Param MAXTAGS_FSM maximun amount of tags
   * @ Param FSM_TAG_MAX_LONG maximun length of tags
   * Label matrix allowed for the analyzer
*/
char FSM_Tags[MAXTAGS_FSM][FSM_TAG_MAX_LONG] = 
{
	"\n",
	"HTML",
		"HEAD",		///<HTML_FSM tag
			"TITLE",
			"VERSION",
			"TYPE",
		"BODY",
			"STATE_INIT",
			"STATE_CONT",
			"VEC_STATE",
				"STATE_NAME",
			"VEC_TRANSITION",
				"TRANSITION",
					"T_NAME",
					"T_ORIGIN",
					"T_DESTINATION",
			"VEC_ACTION",
				"ON_ENTRY",
					"ENTRY_TYPE",
					"ENTRY_NAME",
					"ENTRY_FUNC",
					"ENTRY_ORIGIN",
				"ON_DO",
					"DO_TYPE",
					"DO_NAME",
					"DO_FUNC",
					"DO_ORIGIN",
				"ON_EXIT",
					"EXIT_TYPE",
					"EXIT_NAME",
					"EXIT_FUNC",
					"EXIT_ORIGIN",
				"ON_EVENT",
					"EVENT_TYPE",
					"EVENT_NAME",
					"EVENT_FUNC",
					"EVENT_ORIGIN",
					"EVENT_TRANSITION",
		"UNKNOWN_FSM"
};

char CFSMP_ParserTags[FSM_MSG_MAX_LOGMSG][CFSMP_TAG_MAX_LONG] = 
{
	"FSM",
	" error function does not exist"
};

FSMReader::FSMReader()
{
	strcpy(Type, CFSMP_ParserTags[FSM_MSG_FSM]);	//Type of parser
	ResetState();
	cStateList = NULL;
	FSMReset();
}

FSMReader::~FSMReader()
{
	if (NULL != LogFile)
		LogFile.close();
	ResetState();
}

/**   
   @fn String2Tag
   Translates a string to label
   @param char Name represents the string 
   According to the first letter of the string is compared with the list of allowed tags   
   Returns the type of label is the string if it exists in the matrix of the analyzer is returned otherwise unknown label
*/
void FSMReader::FSMReset()
{	n_states = init_state = it_state = 0;
	if (cStateList != NULL) delete [] cStateList;
	cStateList = NULL;
}

/**   
   * Function agregar una funcion publica
   * @ Param name nombre de la funcion tempora a crear
   * @ Param funcPtr la funcion que se agregara temporalmente a crear
   * Se crea temporal para el analizador
*/
void FSMReader::addPublicFunction(const char *name, mFuncPtr funcPtr){

	_function temp; //se iniciar la variable temp
	temp.name=string(name); //se le asigna el nombre 
	temp.func=funcPtr;      //Se le asigna una funcion

	function_list.push_back(temp); //se inserta al final de una lista el temporal
}

/**   
   * Function buscar funcion publica
   * @ Param name nombre de la funcion tempora
   * Retorna el numero del temporal si es que existe caso contrario devuelve un numero inexistente
*/
int FSMReader::searchPublicFunction(const char *name){

	for(unsigned int i=0; i<function_list.size();i++){
		if(!function_list[i].name.compare(std::string(name))){
			return i;
		}
	}
	return -1;
}
/**   
   * Function error etiqueta
   * @ Param char Text este es el texto a incorporar en el mensaje
   * Se muestra en pantalla el error generado
*/
void FSMReader::TagError(const char *Text)
{	char	msj[128];

	if (LogFile)
	{
		strcpy(msj, CP_ParserMsgs[LOG_PARSER_TAG_CHARACTER_TYPE]);
		strcat(msj, Text);
		strcat(msj, CP_ParserMsgs[LOG_PARSER_TAG_NOT_COMPATIBLE]);
		ErrorParser (msj);
	}
}
/**   
   * Traduce una cadena a etiqueta
   * @ Param char Name representa a la cadena
   * De acuerdo a la primera letra del cadena se compara con la lista de etiquetas permitidas
   * Retorna el tipo de etiqueta pertenece la cadena si es que existe, caso contrario se devuelve etiqueta desconocida
*/
unsigned int FSMReader::String2Tag (const char *Name)
{
	switch (Name[0]){
		case 'B':
			if(!strcmp(Name, FSM_Tags[BODY_FSM]))					return BODY_FSM; 
			break;
		case 'D':
			if(!strcmp(Name, FSM_Tags[DO_TYPE_FSM]))				return DO_TYPE_FSM; 
			if(!strcmp(Name, FSM_Tags[DO_NAME_FSM]))				return DO_NAME_FSM; 
			if(!strcmp(Name, FSM_Tags[DO_FUNC_FSM]))				return DO_FUNC_FSM; 
			if(!strcmp(Name, FSM_Tags[DO_ORIGIN_FSM]))				return DO_FUNC_FSM; 
			break;
		case 'E':
			if(!strcmp(Name, FSM_Tags[ENTRY_TYPE_FSM]))				return ENTRY_TYPE_FSM; 
			if(!strcmp(Name, FSM_Tags[ENTRY_NAME_FSM]))				return ENTRY_NAME_FSM; 
			if(!strcmp(Name, FSM_Tags[ENTRY_FUNC_FSM]))				return ENTRY_FUNC_FSM; 
			if(!strcmp(Name, FSM_Tags[ENTRY_ORIGIN_FSM]))			return ENTRY_ORIGIN_FSM;  

			if(!strcmp(Name, FSM_Tags[EVENT_TYPE_FSM]))				return EVENT_TYPE_FSM; 
			if(!strcmp(Name, FSM_Tags[EVENT_NAME_FSM]))				return EVENT_NAME_FSM; 
			if(!strcmp(Name, FSM_Tags[EVENT_FUNC_FSM]))				return EVENT_FUNC_FSM; 
			if(!strcmp(Name, FSM_Tags[EVENT_ORIGIN_FSM]))			return EVENT_ORIGIN_FSM;  
			if(!strcmp(Name, FSM_Tags[EVENT_TRANSITION_FSM]))		return EVENT_TRANSITION_FSM; 

			if(!strcmp(Name, FSM_Tags[EXIT_TYPE_FSM]))				return EXIT_TYPE_FSM;   
			if(!strcmp(Name, FSM_Tags[EXIT_NAME_FSM]))				return EXIT_NAME_FSM;   
			if(!strcmp(Name, FSM_Tags[EXIT_FUNC_FSM]))				return EXIT_FUNC_FSM;   
			if(!strcmp(Name, FSM_Tags[EXIT_ORIGIN_FSM]))			return EXIT_ORIGIN_FSM; 
			break;
		case 'H':
			if(!strcmp(Name, FSM_Tags[HTML_FSM]))					return HTML_FSM; 
			if(!strcmp(Name, FSM_Tags[HEAD_FSM]))					return HEAD_FSM; 
			break;
		case 'O':
			if(!strcmp(Name, FSM_Tags[ON_ENTRY_FSM]))				return ON_ENTRY_FSM; 
			if(!strcmp(Name, FSM_Tags[ON_DO_FSM]))					return ON_DO_FSM;
			if(!strcmp(Name, FSM_Tags[ON_EXIT_FSM]))				return ON_EXIT_FSM;
			if(!strcmp(Name, FSM_Tags[ON_EVENT_FSM]))				return ON_EVENT_FSM;
			break;
		case 'S':
			if(!strcmp(Name, FSM_Tags[STATE_INIT_FSM]))				return STATE_INIT_FSM;
			if(!strcmp(Name, FSM_Tags[STATE_CONT_FSM]))				return STATE_CONT_FSM;
			if(!strcmp(Name, FSM_Tags[STATE_NAME_FSM]))				return STATE_NAME_FSM;			
			break;
		case 'T':
			if(!strcmp(Name, FSM_Tags[TRANSITION_FSM]))				return TRANSITION_FSM;
			if(!strcmp(Name, FSM_Tags[T_NAME_FSM]))					return T_NAME_FSM;
			if(!strcmp(Name, FSM_Tags[T_ORIGIN_FSM]))				return T_ORIGIN_FSM;
			if(!strcmp(Name, FSM_Tags[T_DESTINATION_FSM]))			return T_DESTINATION_FSM;
			if(!strcmp(Name, FSM_Tags[H_TITLE_FSM]))				return H_TITLE_FSM;
			if(!strcmp(Name, FSM_Tags[H_TYPE_FSM]))					return H_TYPE_FSM;
			break;
		case 'V':
			if(!strcmp(Name, FSM_Tags[VEC_STATE_FSM]))				return VEC_STATE_FSM;
			if(!strcmp(Name, FSM_Tags[VEC_TRANSITION_FSM]))			return VEC_TRANSITION_FSM;
			if(!strcmp(Name, FSM_Tags[VEC_ACTION_FSM]))				return VEC_ACTION_FSM;
			if(!strcmp(Name, FSM_Tags[H_VERSION_FSM]))				return H_VERSION_FSM;
			break;

			//37
	};
	return UNKNOWN_FSM;
};


//GAME INITIALIZATION
void FSMReader::Init(char *FileName)
{
	LogFile.open(FileName);

	if (LogFile)	//NAME the file
		 ResetState();
	else exit(0);
};

/**   
   * @fn BeginParse
   * @param DWORD dwAppData: represents the open file
   * @param bool bAbort: represents if the parser is aborted
   * This function starts the debugger parser
*/
void  FSMReader::BeginParse(DWORD dwAppData, bool &bAbort)
{
	UNUSED_ALWAYS(dwAppData);
	bAbort = false;
#ifdef FSM_DEBUG	//Class HTML_FSM Parser Debug activated
	LogFile << CP_ParserMsgs[LOG_STARTING_INTERPRETATION] << "\n";
#endif
	ResetState();				//Stack reset
	StateStack.push(NIL_FSM);
}
// Funcion para iniciar las etiquetas
/**   
   * @fn StartTag
   *¨Function Start Tags
   * @param CLiteHTMLTag pTag: represents HTML's Tags
   * @param DWORD dwAppData: represents the open file
   * @param bool bAbort: represents if the parser is aborted
   * This function define allowed HTML tags and sorts them by category
*/
void FSMReader::StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
{
	CString TagString;
	unsigned int Tag;
	char msg[256];

	TagString = pTag->getTagName();

#ifdef FSM_DEBUG	//Class HTML_FSM Parser Debug activated
	LogFile << CP_ParserMsgs[LOG_PARSER_START_TAG] << TagString << "\n";
#endif

	UNUSED_ALWAYS(dwAppData);
	bAbort = false;

	Tag = String2Tag(LPCTSTR(TagString)); //revisar si la etiqueta es permitida y obtener el nombre de etiqueta
	// Comenzar una clasificacion y copiado de etiquetas segun su categoria
	switch(StateStack.top()){
		case NIL_FSM:		//Starting and ending states
			if (HTML_FSM == Tag){
				//It is a HTML_FSM file. Change to HTML_FSM state
				StateStack.push(HTML_FSM);
			}
			else{
				strcpy (msg, CP_ParserMsgs[LOG_PARSER_TAG]);
				strcat (msg, TagString );
				strcat (msg,  CP_ParserMsgs[LOG_PARSER_NOT_HTML_TAG2]);
				ErrorParser(msg);
			}
			break;

		case HTML_FSM:
			switch(Tag){
				case HEAD_FSM:
				case BODY_FSM:
					StateStack.push(Tag);
					break;
				default:
					TagNotAllowed (TagString, FSM_Tags[HTML_FSM]);
			}
			break;

		case HEAD_FSM:
			switch(Tag){
				case H_TITLE_FSM:
				case H_VERSION_FSM:
				case H_TYPE_FSM:
					StateStack.push(Tag);
					break;
				default:
					TagNotAllowed (TagString, FSM_Tags[HEAD_FSM]);
			}
			break;

		case BODY_FSM:
			switch(Tag){
				case STATE_INIT_FSM:
				case STATE_CONT_FSM:
				case VEC_STATE_FSM:
				case VEC_TRANSITION_FSM:
				case VEC_ACTION_FSM:
					StateStack.push(Tag);
					break;
				default:
					TagNotAllowed (TagString, FSM_Tags[BODY_FSM]);
			}
			break;

		case VEC_STATE_FSM:
			switch(Tag){
				case STATE_NAME_FSM:
					StateStack.push(Tag);
					break;
				default:
					TagNotAllowed (TagString, FSM_Tags[VEC_STATE_FSM]);
			}
			break;
		case VEC_TRANSITION_FSM:
			switch(Tag){
				case TRANSITION_FSM:
					StateStack.push(Tag);
					break;
				default:
					TagNotAllowed (TagString, FSM_Tags[VEC_TRANSITION_FSM]);
			}
			break;
		case VEC_ACTION_FSM:
			switch(Tag){
				case ON_ENTRY_FSM:
				case ON_DO_FSM:
				case ON_EXIT_FSM:
				case ON_EVENT_FSM:
					StateStack.push(Tag);
					break;
				default:
					TagNotAllowed (TagString, FSM_Tags[VEC_ACTION_FSM]);
			}
			break;
		case TRANSITION_FSM:
			switch(Tag){
				case T_NAME_FSM:
				case T_ORIGIN_FSM:
				case T_DESTINATION_FSM:
					StateStack.push(Tag);
					break;
				default:
					TagNotAllowed(TagString.GetString(), FSM_Tags[TRANSITION_FSM]);
			}
			break;
		case ON_ENTRY_FSM:
			switch(Tag){
				case ENTRY_TYPE_FSM:
				case ENTRY_NAME_FSM:
				case ENTRY_FUNC_FSM:
				case ENTRY_ORIGIN_FSM:
					StateStack.push(Tag);
					break;
				default:
					TagNotAllowed(TagString, FSM_Tags[ON_ENTRY_FSM]);
			}
			break;
		case ON_DO_FSM:
			switch(Tag){
				case DO_TYPE_FSM:
				case DO_NAME_FSM:
				case DO_FUNC_FSM:
				case DO_ORIGIN_FSM:
					StateStack.push(Tag);
					break;
				default:
					TagNotAllowed(TagString, FSM_Tags[ON_DO_FSM]);
			}
			break;
		case ON_EXIT_FSM:
			switch(Tag){
				case EXIT_TYPE_FSM:
				case EXIT_NAME_FSM:
				case EXIT_FUNC_FSM:
				case EXIT_ORIGIN_FSM:
					StateStack.push(Tag);
					break;
				default:
					TagNotAllowed(TagString, FSM_Tags[ON_EXIT_FSM]);
			}
			break;
		case ON_EVENT_FSM:
			switch(Tag){
				case EVENT_TYPE_FSM:
				case EVENT_NAME_FSM:
				case EVENT_FUNC_FSM:
				case EVENT_ORIGIN_FSM:
				case EVENT_TRANSITION_FSM:
					StateStack.push(Tag);
					break;
				default:
					TagNotAllowed(TagString, FSM_Tags[ON_EVENT_FSM]);
			}
			break;
	};
}

/**   
   * @fn EndTag
   * Function End Tag
   * @param CLiteHTMLTag pTag: represents HTML's Tags
   * @param DWORD dwAppData: representa the open file
   * @param bool bAbort: represents if the parse is aborted
   * This function define the end of a Allowed tag and sorts them by category
   * otherwise shows an error message
*/
void FSMReader::EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
{
	CString TagString;
	unsigned int Tag;
	bool Error = false;
	char msj[128];
	int temp_func=-1;

	TagString = pTag->getTagName();

#ifdef FSM_DEBUG	//Class HTML_FSM Parser Debug activated
	LogFile << "Ending Label: " << pTag->getTagName() << std::endl;
#endif

	UNUSED_ALWAYS(dwAppData);
	bAbort = false;

	Tag = String2Tag(LPCTSTR(TagString));
	switch(StateStack.top()){
		case TRANSITION_FSM:
			if (StateStack.top() == Tag){
				cStateList[temp_origin].addTransition(temp_string.c_str(), &cStateList[temp_destination]);
				StateStack.pop();
			}
			else Error = true;
			break;
		case ON_ENTRY_FSM:
			if (StateStack.top() == Tag){
				temp_func=searchPublicFunction(temp_name_function.c_str());
				if(temp_func==-1){
					strcpy (msj, temp_name_function.c_str());
					strcpy (msj, CFSMP_ParserTags[FSM_MSG_FUNC_NOT_EXIST]);
					ErrorParser( msj);
					Error=true;
				}
				else{
					cStateList[temp_origin].addAction(eOnEntry, (eType)temp_type, temp_string.c_str(), function_list[temp_func].func);
					StateStack.pop();
				}
			}
			else{
				Error = true;
			}
			break;
		case ON_DO_FSM:
			if (StateStack.top() == Tag){
				temp_func=searchPublicFunction(temp_name_function.c_str());
				if(temp_func==-1){
					strcpy (msj, temp_name_function.c_str());
					strcpy (msj, CFSMP_ParserTags[FSM_MSG_FUNC_NOT_EXIST]);
					ErrorParser( msj);
					Error=true;
				}
				else{
					cStateList[temp_origin].addAction(eDo, (eType)temp_type, temp_string.c_str(), function_list[temp_func].func);
					StateStack.pop();
				}
			}
			else Error = true;
			break;
		case ON_EXIT_FSM:
			if (StateStack.top() == Tag){
				temp_func=searchPublicFunction(temp_name_function.c_str());
				if(temp_func==-1){
					strcpy (msj, temp_name_function.c_str());
					strcpy (msj, CFSMP_ParserTags[FSM_MSG_FUNC_NOT_EXIST]);
					ErrorParser( msj);
					Error=true;
				}
				else{
					cStateList[temp_origin].addAction(eOnExit, (eType)temp_type, temp_string.c_str(), function_list[temp_func].func);
					StateStack.pop();
				}
			}
			else Error = true;
			break;
		case ON_EVENT_FSM:
			if (StateStack.top() == Tag){
				temp_func=searchPublicFunction(temp_name_function.c_str());
				if(temp_func==-1){
					strcpy (msj, temp_name_function.c_str());
					strcpy (msj, CFSMP_ParserTags[FSM_MSG_FUNC_NOT_EXIST]);
					ErrorParser( msj);
					Error=true;
				}
				else{
					cStateList[temp_origin].addAction(eOnEvent, (eType)temp_type, temp_string.c_str(), temp_name_transition.c_str(), function_list[temp_func].func);
					StateStack.pop();
				}
			}
			else Error = true;
			break;
		case HTML_FSM:
		case HEAD_FSM:
		case H_TITLE_FSM:
		case H_VERSION_FSM:
		case H_TYPE_FSM:
		case BODY_FSM:
		case STATE_INIT_FSM:
		case STATE_CONT_FSM:
		case VEC_STATE_FSM:
		case STATE_NAME_FSM:
		case VEC_TRANSITION_FSM:
		case T_NAME_FSM:
		case T_ORIGIN_FSM:
		case T_DESTINATION_FSM:
		case VEC_ACTION_FSM:
		case ENTRY_TYPE_FSM:
		case ENTRY_NAME_FSM:
		case ENTRY_FUNC_FSM:
		case ENTRY_ORIGIN_FSM:
		case DO_TYPE_FSM:
		case DO_NAME_FSM:
		case DO_FUNC_FSM:
		case DO_ORIGIN_FSM:
		case EXIT_TYPE_FSM:
		case EXIT_NAME_FSM:
		case EXIT_FUNC_FSM:
		case EXIT_ORIGIN_FSM:
		case EVENT_TYPE_FSM:
		case EVENT_NAME_FSM:
		case EVENT_FUNC_FSM:
		case EVENT_ORIGIN_FSM:
		case EVENT_TRANSITION_FSM:
			if (StateStack.top() == Tag) StateStack.pop();	//Change to the previous state
			else Error = true;
			break;
		case UNKNOWN_FSM:
			strcpy_s(msj, TagString);
			strcat_s(msj, CP_ParserMsgs[LOG_PARSER_END_TAG_UNK]);
			ErrorParser(msj);
			break;
		case NIL_FSM:
			break;
	};
	if (Error){
		strcpy_s(msj, TagString);
		strcat_s(msj, CP_ParserMsgs[LOG_PARSER_END_TAG_ERROR2]);
		if (StateStack.size()>=2)
		{
			unsigned int AuxState = StateStack.top();
			StateStack.pop();
			strcat_s(msj, FSM_Tags[StateStack.top()]);
			StateStack.push(AuxState);
		}
		else strcat_s(msj, FSM_Tags[StateStack.top()]);
		strcat_s(msj, CP_ParserMsgs[LOG_PARSER_END_TAG_ERROR2]);
		ErrorParser(msj);
	}
}
/**   
   * @fn Characters
   * Function of Characters
   * @param CString &rText: represents string
   * @param DWORD dwAppData: represents de open file
   * @param bool bAbort: represents if the parser is aborted
   * This function defines the texts of the game, 
   * otherwise shows an error message
*/
void FSMReader::Characters(const CString &rText, DWORD dwAppData, bool &bAbort)
{

#ifdef FSM_DEBUG	//Class HTML_FSM Parser Debug activated
LogFile << CP_ParserMsgs[LOG_PARSER_TEXT] << rText << std::endl;
#endif

	UNUSED_ALWAYS(dwAppData);
	bAbort = false;

	switch(StateStack.top()){
		//TEXT
		case H_TITLE_FSM:
			if (strcmp(rText, Title)){
				ErrorParser (CP_ParserMsgs[LOG_PARSER_NOT_THIS_VG]);
				bAbort = true;
			}
			break;
		case H_VERSION_FSM:
			if (strcmp(rText, Version)){
				VersionMismatch(rText);
				bAbort = true;
			}
			break;
		case H_TYPE_FSM:
			if (strcmp(rText, "FSM"))
			{
				FileTypeMismatch(rText, "FSM");
				bAbort = true;
			}
			break;
		case STATE_INIT_FSM:
			init_state = atoi(rText);
			break;
		case STATE_CONT_FSM:
			n_states = atoi(rText);
			cStateList=new State[n_states];
			break;
		case STATE_NAME_FSM:
			cStateList[it_state].setName(rText.GetString());
			it_state++;
			break;
			//PROPOSITO GENERAL
		case T_NAME_FSM:
		case ENTRY_NAME_FSM:
		case DO_NAME_FSM:
		case EXIT_NAME_FSM:
		case EVENT_NAME_FSM:
			temp_string=std::string(rText.GetString());
			break;
		case T_ORIGIN_FSM:
		case ENTRY_ORIGIN_FSM:
		case DO_ORIGIN_FSM:
		case EXIT_ORIGIN_FSM:
		case EVENT_ORIGIN_FSM:
			temp_origin=atoi(rText);
			break;
		case T_DESTINATION_FSM:
			temp_destination=atoi(rText);
			break;
		case ENTRY_TYPE_FSM:
		case DO_TYPE_FSM:
		case EXIT_TYPE_FSM:
		case EVENT_TYPE_FSM:
			temp_type=atoi(rText);
			break;
		case ENTRY_FUNC_FSM:
		case DO_FUNC_FSM:
		case EXIT_FUNC_FSM:
		case EVENT_FUNC_FSM:
			temp_name_function=std::string(rText.GetString());
			break;
		case EVENT_TRANSITION_FSM:
			temp_name_transition=std::string(rText.GetString());
			break;
	};
}

/**   
   * @fn Comment
   * Function Comments
   * @param CString &rComment: represents the comment string
   * @param DWORD dwAppData: represents the open file
   * @param bool bAbort: represents if the parser is aborted
   * This function defines the comments for the tags
*/
void FSMReader::Comment(const CString &rComment, DWORD dwAppData, bool &bAbort)
{
#ifdef FSM_DEBUG	//Class HTML_FSM Parser Debug activated
	LogFile << "Comments: " << rComment << std::endl;
#endif

	UNUSED_ALWAYS(dwAppData);
	bAbort = false;
}
/**   
   * @fn EndParse
   * Function to finish de parser
   * @param DWORD dwAppData: represents the open file
   * @param bool bAbort: represents if the parser is aborted
   * This function closes the file that abort the parser
*/
void FSMReader::EndParse(DWORD dwAppData, bool bIsAborted)
{
	UNUSED_ALWAYS(dwAppData);
#ifdef FSM_DEBUG	//Class HTML_FSM Parser Debug activated
	if (bIsAborted) LogFile << "Se ha acabado la interpretación del fichero.\n";
#endif
}
