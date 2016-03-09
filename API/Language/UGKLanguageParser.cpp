/*LANGUAGE READER
	@author Ramon Molla
	@version 2007-09-11
*/
#include <UGKLanguageParser.h>
#include "Global.h"
#include <UGKString.h>

//Sensitive string tags
char CLaP_Tags[LAN_MAX_TAGS][DHTML_TAG_MAX_LONG];

char CLaP_OwnTags[LAN_MAX_TAGS][DHTML_TAG_MAX_LONG] = 
{
	"CONTENT",
	"ID",
	"LANGUAGE",
	"RESOURCE"
};

char CLaP_LanguageParserMsgs[LOG_LANGUAGE_MAX_LOGMSG][CLaP_MSJ_MAX_LONG] = 
{
	"\". It is required a \"LANGUAGE\" file type.",
	"Language",
	" English ",
	" Spanish ",
	" Catalan "
};

unsigned int tempID=0;
char* version;
char* titulo;

CLanguageReader::CLanguageReader(char * versionP, char* tituloP)
{
	strcpy(Type,CLaP_LanguageParserMsgs[LOG_LANGUAGE_LANGUAGE]);	//Type of parser
	ResetState();
	StateStack.push(NIL_D);
	AssignTags (CLaP_Tags, CLaP_OwnTags, LAN_MAX_TAGS);
	version = versionP;
	titulo = tituloP;
}

CLanguageReader::~CLanguageReader()
{
	ResetState();
}

//Preface for the Tag analysis
void CLanguageReader::TagError(char *Text)
{	char	msj[CLaP_MSJ_MAX_LONG];

	if (LogFile)
	{
		strcpy(msj,CP_ParserMsgs[LOG_PARSER_TAG_CHARACTER_TYPE]);
		strcat(msj, Text);
		strcat(msj, CP_ParserMsgs[LOG_PARSER_TAG_NOT_COMPATIBLE]);
		ErrorParser (msj);
	}
}

//Translates from a string to the token that the string represents
unsigned int CLanguageReader::String2Tag (const char *Name)
{
	switch (Name[0])
	{
	case 'B': if (0 == strcmp(Name, CLaP_Tags[BODY_D]))		return BODY_D;
		break;
	case 'C': if (0 == strcmp(Name, CLaP_Tags[L_CONTENT]))	return L_CONTENT;	  
		break;
	case 'H': if ('E' == Name[1])							return HEAD_D;
			  else if (strcmp(Name, CLaP_Tags[HTML_D])==0)		return HTML_D;
			  else											return UNKNOWN_D;
			
		break;
	case 'I': if (0 == strcmp(Name, CLaP_Tags[L_ID]))			return L_ID;
		break;
	case 'L': if (0 == strcmp(Name, CLaP_Tags[L_LANGUAGE]))			return L_LANGUAGE;
		break;
	case 'R': if (0 == strcmp(Name, CLaP_Tags[L_RESOURCE]))	return L_RESOURCE;
		break;
	case 'T': if (strcmp(Name, CLaP_Tags[TITLE_D])==0)		return TITLE_D;
			  else if (strcmp(Name, CLaP_Tags[TYPE_D])==0)			return TYPE_D;
			  else										return UNKNOWN_D;
		break;
	case 'V': if (0 == strcmp(Name, CLaP_Tags[VERSION_D]))	return VERSION_D;
		break;
	};
	return UNKNOWN_D;
};

/* Class that loads the values by default in the very beginning of every level in the game

   LANGUAGE INITIALIZATION
*/

void CLanguageReader::Init(char *File)
{
	if (NULL != File)
	{
		LogFile.open(File);
		if (!LogFile)	//NAME the file
			exit(0);
	}
	ResetState();
	StateStack.push(NIL_D);
};

void CLanguageReader::BeginParse(DWORD dwAppData, bool &bAbort)
{
	UNUSED_ALWAYS(dwAppData);
	bAbort = false;

#ifdef CLaP_DEBUG	//Class HTML Parser Debug activated
	LogFile << "LANGUAGE PARSER: BEGIN PARSE.\n";
#endif
}

void CLanguageReader::StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
{
	CString TagString;
	unsigned int Tag, TopTag;

	TagString = pTag->getTagName();

#ifdef CLaP_DEBUG	//Class HTML Parser Debug activated
	LogFile << "LANGAGE PARSER: START TAG " << TagString << UGKS_NEW_LINE_STRING;
#endif

	UNUSED_ALWAYS(dwAppData);
	bAbort = false;

	Tag = String2Tag(LPCTSTR(TagString));
	TopTag = StateStack.top();
 	switch(TopTag)
	{case NIL_D://Starting and ending states
				if (HTML_D == Tag)
				{//It is a HTML file. Change to HTML state
					StateStack.push(HTML_D);
					//Do some initializations
				}
				else ErrorParser((char *) CP_ParserMsgs[LOG_PARSER_NOT_HTML_TAG]);
		break;
	 case BODY_D:	//HTML tag
					switch (Tag)
					{
						case L_CONTENT:
						case L_ID:
						case L_LANGUAGE:
						case L_RESOURCE: StateStack.push(Tag); //Change to the state specified by the Tag
							break;
						default: TagNotAllowed (TagString, CLaP_Tags[BODY_D]);
					}
		break;
	 case L_RESOURCE:
		 switch (Tag)
		 {
		 case L_ID:		
		 case L_CONTENT:
			 StateStack.push(Tag);
			 break;
		 default: ErrorParser((char *) CLaP_LanguageParserMsgs[LOG_LANGUAGE_PARSER_ERROR_RESOURCE]); 
		 }
		 break;
	 case HEAD_D:		//HTML tag
					switch (Tag)
					{
					case TYPE_D:
						//Rename Tag to the correct context
							Tag = TYPE_D;
					case TITLE_D:
					case VERSION_D:
					case L_LANGUAGE:
						StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					default: TagNotAllowed (TagString, CLaP_Tags[HEAD_D]);
					}
		break;
	case HTML_D:	//HTML Tag
					//The parser is at the very beginning of the iniitalization file
					switch (Tag)
					{
					case HEAD_D:		//A HEAD Tag is expected
					case BODY_D: 
						StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					default: 
							ErrorParser((char *) CP_ParserMsgs[LOG_PARSER_ONLY_HEAD_OR_BODY]);
							TagError(CLaP_Tags[Tag], CLaP_Tags[HTML_D]);
					}
		break;
	default:;
	};
}

/*void CLanguageReader::EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
{
	printf("Etiqueta fin: %s\n", pTag->getTagName());
	UNUSED_ALWAYS(dwAppData);
	bAbort = false;

}*/

void CLanguageReader::EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
{
	CString TagString;
	unsigned int Tag;
	bool Error = false;
	char msj[CLaP_MSJ_MAX_LONG];

	TagString = pTag->getTagName();

	#ifdef CLaP_DEBUG	//Class HTML Parser Debug activated
		LogFile << CP_ParserMsgs[LOG_PARSER_END_TAG_END] << pTag->getTagName() << std::endl;
	#endif
	//printf("Etiqueta fin: %s\n", pTag->getTagName());
	UNUSED_ALWAYS(dwAppData);
	bAbort = false;

	Tag = String2Tag(LPCTSTR(TagString));
	switch(StateStack.top())
	{
	 case BODY_D:
	 case L_CONTENT:
	 case HEAD_D:
	 case L_ID:
	 case L_LANGUAGE:
	 case L_RESOURCE:
	 case TITLE_D:
 	 case TYPE_D:
	 case VERSION_D:
 	 case HTML_D:	//The parser is at the very end of the initialization file					
		 			if (StateStack.top() == Tag) StateStack.pop();	//Change to the previous state
					else Error = true;
		break;
	 case NIL_D:
	 case L_MAXTAGS:	//Closing a label when all the lables are closed is an error
					strcpy_s(msj, TagString);
					strcat_s(msj, CP_ParserMsgs[LOG_PARSER_END_TAG_MAXTAGS]);
					ErrorParser(msj);
		break;
	 case UNKNOWN_D:	//This tag is for management purpouses only. There is no correspondence to any string label in the HTML file
					strcpy_s(msj, TagString);
					strcat_s(msj, CP_ParserMsgs[LOG_PARSER_END_TAG_UNK]);
					ErrorParser(msj);
		break;
	default:;
	};	//Switch

	if (Error)
	{	strcpy_s(msj, TagString);
		strcat_s(msj, CP_ParserMsgs[LOG_PARSER_END_TAG_ERROR1]);

		if (StateStack.size()>=2)
		{
			unsigned int AuxState = StateStack.top();
			StateStack.pop();
			strcat_s(msj, CLaP_Tags[StateStack.top()]);
			StateStack.push(AuxState);
		}
		else strcat_s(msj, CLaP_Tags[StateStack.top()]);
		strcat_s(msj, CP_ParserMsgs[LOG_PARSER_END_TAG_ERROR2]);
		ErrorParser(msj);
	}
}

/*void CLanguageReader::Characters(const CString &rText, DWORD dwAppData, bool &bAbort)
{
	printf("Texto: %s\n", rText);
	UNUSED_ALWAYS(dwAppData);
	bAbort = false;
}*/

void CLanguageReader::Characters(const CString &rText, DWORD dwAppData, bool &bAbort)
{
static int     intAux, msgBox;
string auxText;
unsigned int TopTag, TopTag2;

//LanguageManager.TextResource <- meto los valorea ahi

if (bAbort) return;

#ifdef CLaP_DEBUG	//Class HTML Parser Debug activated
LogFile <<CP_ParserMsgs[LOG_PARSER_TEXT] << rText << std::endl;
#endif

	UNUSED_ALWAYS(dwAppData);
	bAbort = false;

	TopTag = StateStack.top();
	TopTag2  = TopTag;

	#ifdef CLaP_DEBUG	//Class HTML Parser Debug activated
	LogFile << "TopTag: " << " "<< rText << " "<< TopTag << " "<< TopTag2 << " "<< std::endl;
	#endif
	switch(TopTag)
	{
	case L_ID: 
		intAux = atoi (rText); 
		if(intAux >= 0)	tempID=intAux;
		else{

			msgBox = MessageBox(NULL,
										TEXT(rText),
										TEXT("Error al recuperar el ID del RESOURCE"),
										MB_ICONASTERISK | MB_OKCANCEL );
		}

		break;
	case L_CONTENT:
//			LanguageManager.SetText(tempID,rText);
		auxText = string(rText);
		Language->FillText(intAux,auxText);
		break;


	case TITLE_D:	//HTML tag
		if (strcmp(rText, titulo))
		{
			ErrorParser ((char *) CP_ParserMsgs[LOG_PARSER_NOT_THIS_VG]);
			bAbort = true;
		}
		//MessageBox(NULL,rText,UGKS_EMPTY_STRING,0);////////////////////////////////////////////
		break;
	case TYPE_D:
		if (strcmp(rText, CLaP_LanguageParserMsgs[LOG_LANGUAGE_LANGUAGE]))
		{
			TagNotAllowed(rText, CLaP_LanguageParserMsgs[LOG_LANGUAGE_LANGUAGE]);
			bAbort = true;
		}
		else ///When found closing tag TYPE_FILE, this method is also called and an error could happen
			StateStack.pop();
			StateStack.push(TYPE_D);
		break;
	case VERSION_D:
		if (strcmp(rText, version))
		{
			VersionMismatch(rText);
			bAbort = true;
		}
		break;
	//default:;		//Tags not supported are not managed, even if they have content associated
	};	//Switch
}

void CLanguageReader::Comment(const CString &rComment, DWORD dwAppData, bool &bAbort)
{
	printf(CP_ParserMsgs[LOG_PARSER_COMMENT], rComment);
	UNUSED_ALWAYS(dwAppData);
	bAbort = false;
}

void CLanguageReader::EndParse(DWORD dwAppData, bool bIsAborted)
{
	UNUSED_ALWAYS(dwAppData);
	if (bIsAborted) printf (CP_ParserMsgs[LOG_PARSER_END_PARSE]);
};