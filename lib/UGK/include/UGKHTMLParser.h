/**	General Parser class that manages general functions of any given parser

*	Prefix CP_: Class Parser

*	@author Ramon Molla
*	@version 2012-09
*/

#ifndef CP_PARSER
#define CP_PARSER

#include <LiteHTMLReader.h>
#include <stack>

using namespace std;

#ifndef FSTREAM_INITIALIZED
#define FSTREAM_INITIALIZED
#include <fstream>
#endif

#define CP_DEBUG	//Class HTML Parser Debug activated
#define CP_MAX_MSJ_LENGTH	128
#define CP_MAX_TYPE_LENGTH	 32
#define CP_MSG_MAX_LONG 64	//Maximun length of a sensitive tag

typedef enum {
	LOG_PARSER_UNKNOWN,
	LOG_PARSER_TAG_CHARACTER_TYPE,
	LOG_PARSER_TAG_NOT_COMPATIBLE,
	
	LOG_PARSER_TAG,
	LOG_PARSER_ALLOWED,
	LOG_PARSER_NOT_ALLOWED,
	LOG_PARSER_GROUP,
	
	LOG_PARSER_BEGIN_PARSE,
	LOG_STARTING_INTERPRETATION,
	LOG_PARSER_START_TAG,
	LOG_PARSER_NOT_HTML_TAG,
	LOG_PARSER_NOT_HTML_TAG2,
	LOG_PARSER_ONLY_HEAD_OR_BODY,

	LOG_PARSER_END_TAG_END,
	LOG_PARSER_END_TAG_MAXTAGS,
	LOG_PARSER_END_TAG_UNK,
	LOG_PARSER_END_TAG_ERROR1,
	LOG_PARSER_END_TAG_ERROR2,
	LOG_PARSER_SEGMENT,

	LOG_PARSER_TEXT,
	LOG_PARSER_NOT_THIS_VG,
	LOG_PARSER_FILE_TYPE,
	LOG_PARSER_FILE_TYPE2,
	LOG_PARSER_FILE_VERSION,
	LOG_PARSER_FILE_VERSION2,
	 
	LOG_PARSER_COMMENT,

	LOG_PARSER_END_PARSE,
	
	LOG_PARSER_MAX_LOGMSG
} CP_LogMsg;

extern char CP_ParserMsgs[LOG_PARSER_MAX_LOGMSG][CP_MSG_MAX_LONG];

/**	
*	CHTMLParser

*	@author Ramon Molla
*	@version 2012-09
*/

class CHTMLParser : public ILiteHTMLReaderEvents
{

protected:
	//Game general attributes
	
	std::ofstream	LogFile;		//For reporting about the parsing for debugging
	stack<unsigned int> StateStack;	//Stack to store the different states while parsing a file
	char Type[CP_MAX_TYPE_LENGTH];	//Name of the parser created

	//Frees the stack so that it is completely empty
	inline void ResetState() {while (!StateStack.empty()) StateStack.pop();};

	virtual void TagError		(char *Text);
	void		 ErrorClosingTag(CString TagString); ///<Logs a parser error of not matching closing tags
	void		 ErrorParser	(char *msj) {LogFile << Type << " parser error: " << msj << std::endl;}

	///Translates from a string to the token that the string represents
	virtual unsigned int String2Tag (const char *Name);

public:
	char	*Version;	// Game VERSION
	char	*Title;		// Game TITLE

	///Constructor
	CHTMLParser		() {};

	///Destructor
	~CHTMLParser	(){if (NULL != LogFile) LogFile.close();};

	void Init		(char *FileName);
	void VersionMismatch			(const CString &V);
	void TagNotAllowed				(char* Tag, char* Group);
	inline void TagNotAllowed		(const char* Tag, char* Group){TagNotAllowed ((char*) Tag, Group);};
	void FileTypeMismatch			(char* FileType, char* ExpectedType);
	inline void FileTypeMismatch	(const char* FileType, char* ExpectedType){FileTypeMismatch((char*) FileType, ExpectedType);};

	/**   
	@fn TagError
	@param char Tag: this is the label that does not belong to the message
	@param char Group this is the name of the group classification label for the message
	It displays the message that the label does not belong
	*/
	inline void TagError(char *Tag, char *Group){if (LogFile) TagNotAllowed(Tag, Group);};
};

#endif