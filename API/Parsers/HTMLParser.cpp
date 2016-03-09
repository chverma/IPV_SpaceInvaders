/**	Declaration of all the parser methods to understand the initialization files
*	@author Ramon Mollá
*	@version 2012-09
*/

#include <Parsers\HTMLParser.h>
#include <UGKString.h>

DHTML_TagsArray CP_ParserTags = 
{
	UGKS_EMPTY_STRING,
	"BODY",		//HTML tag
	"HEAD",		//HTML tag
	"HTML",		//HTML tag
	"TITLE",		//HTML tag
	"TYPE",
	"",		//This tag is for management purpouses only. There is no correspondence to any string label in the HTML file
	"VERSION"
};

char CP_ParserMsgs[LOG_PARSER_MAX_LOGMSG][CP_MSG_MAX_LONG] = 
{
	"Unknown",			//This tag has no matching. It is for management purpouses only. 
	"Character Type (",
	") not compatible."

	"Tag ",
	" allowed in a ",
	" not allowed in a ",
	" group.",

	"Begining the file parsing.",
	"Starting the interpretation of the file.",
	"Start tag: ",
	"First tag was not an HTML tag.",
	" was not an HTML tag.",
	"Only the attribute HEAD or BODY may appear in a group HTML.",
	
	"End tag: ",
	" clossing tag out from the HTML section.",
	" clossing tag in an unknown state.",
	" clossing tag without correspondent opening tag.",
	" clossing tag without correspondent opening tag in a ",
	" segment.",

	"Text: ",
	"This file does not corresponds to this videogame.",
	"This file type is \"\n",
	"\". It is required a file of type \"\n",
	"This file versión is v",
	". This does not corresponds with this game version v",
	"Comments: %s\n",
	"End interpretation"
};


/**
*	\fn void AssignTags (DHTML_TagsArray ExtTags)
*	Copy internal Tags for parsing into the external tags of the corresponding parser
*	\param[out] Tags	The array destination
*	\param[in]  OwnTags	The array with the tags related to the current parser
*	\param[in]  Size	Amount of tags counting CP_ParserTags and current parser ones
*/
void CHTMLParser::AssignTags (DHTML_TagsArray Tags, DHTML_TagsArray OwnTags, unsigned int Size)
{
	unsigned int TagsIndex, OwnTagsIndex;
	
	for(TagsIndex = 0; TagsIndex<MAXTAGS_D;TagsIndex++) strcpy(Tags[TagsIndex] , CP_ParserTags[TagsIndex]);
	for(OwnTagsIndex = 0; TagsIndex<Size;OwnTagsIndex++, TagsIndex++) strcpy(Tags[TagsIndex] , OwnTags[OwnTagsIndex]);
}

/**
*	\fn void CHTMLParser::FileTypeMismatch		(char* FileType, char* ExpectedType)
*	Reports a parser error associated to a file type mismatch when parsing a file.
*	\param[in] FileType		Type of the file currently parsed
*	\param[in] ExpectedType	Expected type of the file 
*/
void CHTMLParser::FileTypeMismatch		(char* FileType, char* ExpectedType)
{
	char	msg[128];

	strcpy(msg, CP_ParserMsgs[LOG_PARSER_FILE_TYPE]);
	strcat(msg, FileType);
	strcat(msg, CP_ParserMsgs[LOG_PARSER_FILE_TYPE2]);
	strcat(msg,	ExpectedType);
	strcat(msg, "\".");
	ErrorParser (msg);
}

/**
*	\fn void CHTMLParser::VersionMismatch (const CString &V)
*	Reports a parser error associated to a version mismatch when parsing a file.
*	\param[in] V Current file version. Differentfrom current video game version
*/
void CHTMLParser::VersionMismatch (const CString &V)
{
	char msg[256];

	strcpy(msg, CP_ParserMsgs[LOG_PARSER_FILE_VERSION]);
	strcat(msg, V);
	strcat(msg, CP_ParserMsgs[LOG_PARSER_FILE_VERSION2]);
	strcat(msg, Version);
	strcat(msg, ".");
	ErrorParser (msg);
}

/**
*	\fn void CHTMLParser::TagNotAllowed (char* Tag, char* Group)
*	Sets a parser error associated to a given mismatch TAG and Group when parsing a file.
*	\param[in] Tag Tag to log
*	\param[in] Group mismatch to log
*/
void CHTMLParser::TagNotAllowed (char* Tag, char* Group)
{
	char msg[256];

	strcpy (msg, CP_ParserMsgs[LOG_PARSER_TAG]);
	strcat (msg, Tag );
	strcat (msg, CP_ParserMsgs[LOG_PARSER_NOT_ALLOWED]);
	strcat (msg, Group);
	strcat (msg, CP_ParserMsgs[LOG_PARSER_GROUP]);
	ErrorParser( msg);
}

/**
*	\fn void CHTMLParser::TagError(const char *Text) 
*	Virtual error manager that logs the associated error to a given TAG when parsing a file.
*	Given by default in case the user does not create it on a given parser
*	\param[in] Text Message to log
*/

void CHTMLParser::TagError(char *Text) {ErrorParser (Text);}

/**
*	\fn unsigned int CHTMLParser::String2Tag (const char *Name)
*	Converts a string to the token that the string represents
*	\param[in] Text to convert to tag
*/
unsigned int CHTMLParser::String2Tag (const char *Name)
{
	return 0;
}

/**
*	\fn void CHTMLParser::ErrorClosingTag(CString TagString)
*	Error Msj to log when a given TAG is not properly closed when parsing a file.
*	Typically this is when the closed tag does not match the current parser state 
*	\param[in] TagString Message to log
*/

void CHTMLParser::ErrorClosingTag(CString TagString) ///<Logs a parser error of not matching closing tags
{
	char msj[CP_MAX_MSJ_LENGTH];
	
	strcpy_s	(msj, TagString);
	strcat_s	(msj, CP_ParserMsgs[LOG_PARSER_END_TAG_ERROR1]);
	ErrorParser	(msj);
}

/**
*	\fn void CHTMLParser::Init(char *FileName)
*	Initializes the parser and sets the log file where to store the results of parsing a file.
*	\param[in] Text Message to log
*/

void CHTMLParser::Init(char *FileName)
{
	LogFile.open(FileName);
	if (LogFile)	//The file is opened
	{
		Version	= NULL;
		Title	= NULL;
	}
	else
		exit(0);	//Cannot open file. Finish program
};


