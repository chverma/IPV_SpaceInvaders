/** DEVICES READER
*	@author Sonia Cardenas
*	@version 2013-01-18
*/

#include "DeviceParser.h"
#include "Global.h"
#include <UGKPhysics.h>
#include <string.h>
#include <vector>
#include <string>
#include <UGKString.h>

GCHARS_CharacterType	CDevP_CharType; //se define la variables Tipo de Caracter
UGKPHY_CINEMATIC_ACTION	CDevP_CinematicAction;
unsigned				int NumberDevices;

/**   
   @fn CDevP_Tags
   @param MAXTAGS_DEV maximum number of tags
   @param CDevP_TAG_MAX_LONG the maximum length of the labels
   An array of allowed tags for the analyzer
*/
char CDevP_Tags[MAXTAGS_DEV][DHTML_TAG_MAX_LONG];

char CDevP_OwnTags[MAXTAGS_DEV][DHTML_TAG_MAX_LONG] = 
{
	"DEVICE",
	"NAME",
	"NAME"
};

/**   
   @fn TagError
   @param char Text: This is the text to be incorporated in the error message
   It displays the error message generated
*/
void CDeviceReader::TagError(char *Text)
{	char	msj[128];

	if (LogFile)
	{
		strcpy(msj, CP_ParserMsgs[LOG_PARSER_TAG_CHARACTER_TYPE]);
		strcat(msj, Text);
		strcat(msj, CP_ParserMsgs[LOG_PARSER_TAG_NOT_COMPATIBLE]);
		ErrorParser (msj);
	}
}

CDeviceReader::CDeviceReader()
{
	strcpy(Type,"InputDevices");	//Type of parser
	ResetState();
	StateStack.push(NIL_D);
	AssignTags (CDevP_Tags, CDevP_OwnTags, MAXTAGS_DEV);
}

CDeviceReader::~CDeviceReader()
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
unsigned int CDeviceReader::String2Tag (const char *Name)
{
switch (Name[0])
	{
	case 'B': if (0 == strcmp(Name, CDevP_Tags[BODY_D])) return BODY_D;
		break;
	case 'D': if (0 == strcmp(Name, CDevP_Tags[DEVICE_D])) return DEVICE_D;
		break;
	case 'H':	if (0 == strcmp(Name, CDevP_Tags[HTML_D])) return HTML_D;
				else if (0 == strcmp(Name, CDevP_Tags[HEAD_D])) return HEAD_D;
		break;
	case 'N': if (0 == strcmp(Name, CDevP_Tags[NAME_D])) return NAME_D;
		break;
	case 'T':	if (0 == strcmp(Name, CDevP_Tags[TITLE_D])) return TITLE_D;
				else 	if (0 == strcmp(Name, CDevP_Tags[TYPE_D])) return TYPE_D;
		break;
	case 'V': if (0 == strcmp(Name, CDevP_Tags[VERSION_D])) return VERSION_D;
		break;
	};
	return UNKNOWN_D;
};

/**   
   @fn Init
   Initialize Level
   @param char FileName : file name
   Number starts at 0 bunkers    
   Starts the default values ??at the beginning of each level in the game
*/
void CDeviceReader::Init(char *FileName)
{
	//BunkersManager.BunkersAmount = 0; // Initialization of the number of the bunkers
	LogFile.open(FileName);
	if (!LogFile)	//NAME the file
		exit(0);
	ResetState();
	StateStack.push(NIL_D);
};

/**   
   @fn BeginParse 
   @param DWORD dwAppData represents the open file
   @param bool bAbort is aborted if parser 
   This function starts the debugger parser
*/
void CDeviceReader::BeginParse(DWORD dwAppData, bool &bAbort)
{
	UNUSED_ALWAYS(dwAppData);
	bAbort = false;
#ifdef CDevP_DEBUG	//Class HTML Parser Debug activated
	LogFile << "Begining the initialization file parsing.\n";
#endif
}

/**   
   @fn StartTag
   @param CLiteHTMLTag pTag represents HTML tags
   @param DWORD dwAppData represents the open file
   @param bool bAbort represents whether the parser is aborted    
   This function defines the HTML tags permitted in the matrix of the analyzer and classified by categories or groups
*/
void CDeviceReader::StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
{
	CString TagString;
	unsigned int Tag;

	TagString = pTag->getTagName();

#ifdef CDevP_DEBUG	//Class HTML Parser Debug activated
	LogFile << CP_ParserMsgs[LOG_PARSER_START_TAG] << TagString << UGKS_NEW_LINE_STRING;
#endif

	Tag = String2Tag(LPCTSTR(TagString));

	UNUSED_ALWAYS(dwAppData);
	bAbort = false;

	switch(StateStack.top())
	{
	case NIL_D://Starting and ending states
				//if (HTML_D == Tag)
				//	//It is a HTML file. Change to HTML state
				//	StateStack.push(HTML_D);
				//else ErrorParser(CP_ParserMsgs[LOG_PARSER_NOT_HTML_TAG]);
				break;
	 case DEVICE_D:
		break;
	case HTML_D:	//HTML Tag
		break;
	default:;
	};

}

/**   
   @fn EndTag
   @param CLiteHTMLTag pTag represents HTML tags
   @param DWORD dwAppData represents the open file
   @param bool bAbort represents whether the parser is aborted    
   This function defines the end of a label acuardo permitted its category, If not shown an error message
*/
void CDeviceReader::EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
{
	CString TagString;
	unsigned int Tag;
	bool Error = false;
	char msj[128];

	TagString = pTag->getTagName();

	#ifdef CDevP_DEBUG	//Class HTML Parser Debug activated
		LogFile << "Etiqueta fin: " << pTag->getTagName() << std::endl;
	#endif
	//printf("Etiqueta fin: %s\n", pTag->getTagName());
	UNUSED_ALWAYS(dwAppData);
	bAbort = false;

	Tag = String2Tag(LPCTSTR(TagString));

	switch(StateStack.top())
	{case BODY_D:
	 case DEVICE_D:
	 case HEAD_D:		
	 case NAME_D:
	 case TITLE_D:		
 	 case TYPE_D:
	 case VERSION_D:
	 case HTML_D:	//The parser is at the very end of the initialization file
		 			if (StateStack.top() == Tag) StateStack.pop();	//Change to the previous state
					else Error = true;
		break;
	 case NAME_DEVICE_D: //Name of the character
	 case NIL_D:
	 case MAXTAGS_DEV:	//Closing a label when all the lables are closed is an error
					//strcpy_s(msj, TagString);
					//strcat_s(msj, CP_ParserMsgs[LOG_PARSER_END_TAG_MAXTAGS]);
					//ErrorParser(msj);
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
		strcat_s(msj, CP_ParserMsgs[LOG_PARSER_END_TAG_ERROR2]);
		if (StateStack.size()>=2)
		{
			unsigned int AuxState = StateStack.top();
			StateStack.pop();
			strcat_s(msj, CDevP_Tags[StateStack.top()]);
			StateStack.push(AuxState);
		}
		else strcat_s(msj, CDevP_Tags[StateStack.top()]);
		strcat_s(msj, CP_ParserMsgs[LOG_PARSER_END_TAG_ERROR2]);
		ErrorParser(msj);
	}
}

/**   
   @fn Characters
   @param CString &rText represents the text
   @param DWORD dwAppData represents the open file
   @param bool bAbort represents if the the parser is aborted
   This function defines the text for labels for the game   
   Otherwise it shows an error message
*/
void CDeviceReader::Characters(const CString &rText, DWORD dwAppData, bool &bAbort)
{

if (bAbort) return;

if (!isspace(rText[0]))
{
	#ifdef CDevP_DEBUG	//Class HTML Parser Debug activated
		LogFile << "Texto: " << rText << " " << std::endl;
	#endif

}
	if (!strncmp(rText,"*",1)) 
	{
		std::string str = rText; 
		Game.DevicesIO.push_back(str);
	}


	UNUSED_ALWAYS(dwAppData);
	bAbort = false;
	switch(StateStack.top())
	{
	/*	In this case, A new node for the Scene Graph is performed.
		Type, casting and dynamic memory allocation has to be done
	*/
	case BODY_D:///Generic treatment. Only for management purpouses. This parser is only sensitive to specific "names"
		LogFile << "BODY: "  << std::endl;
		break;
	case DEVICE_D:///Generic treatment. Only for management purpouses. This parser is only sensitive to specific "names"
		LogFile << "DEVICE: "  << std::endl;
		break;
	case HTML_D:   //HTML tag
		LogFile << "HTML: "  << std::endl;
		break;
	case HEAD_D:   //HTML tag
		LogFile << "HEAD: "  << std::endl;
		break;
	case NAME_D:///Generic treatment. Only for management purpouses. This parser is only sensitive to specific "names"
		LogFile << "NAME: "  << std::endl;
		break;
	case NAME_DEVICE_D:///Generic treatment. Only for management purpouses. This parser is only sensitive to specific "names"
		break;
	case TITLE_D:   //HTML tag
		LogFile << "TITLE: "  << std::endl;
		break;
	case TYPE_D:
		LogFile << "TYPE: "  << std::endl;
		break;
	case VERSION_D:
		LogFile << "VERSION: "  << std::endl;
		break;
	default:;		//Tags not supported are not managed, even if they have content associated
	};	//Switch
}

/**   
   @fn Comment
   @param CString &rComment represents the string of comments
   @param DWORD dwAppData represents the open file
   @param bool bAbort represents if the parser is aborted    
   This function defines Entries for tags
*/
void CDeviceReader::Comment(const CString &rComment, DWORD dwAppData, bool &bAbort)
{
	printf("Comentarios: %s\n", rComment);
	UNUSED_ALWAYS(dwAppData);
	bAbort = false;
}

/**   
   @fn EndParse
   @param DWORD dwAppData represents the open file
   @param bool bAbort represents if the parser is aborted  
   This function closes the file if the parser is aborted  
*/
void CDeviceReader::EndParse(DWORD dwAppData, bool bIsAborted)
{
	UNUSED_ALWAYS(dwAppData);
	if (bIsAborted) printf ("Se ha acabado la interpretación del fichero.\n");
};
