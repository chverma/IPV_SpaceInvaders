/**	General Parser class that manages general functions of any given parser

*	Prefix CP_: Class Parser

*	@author Ramon Molla
*	@version 2012-09
*/

#ifndef CP_PARSER
#define CP_PARSER

#include <LiteHTMLReader.h>
#include <stack>
#include <DefaultHTMLLabels.h>
#include <map>
#include <string>

using namespace std;

#ifndef FSTREAM_INITIALIZED
#define FSTREAM_INITIALIZED
#include <fstream>
#endif

#include <UGK.h>

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

#define CP_LexemeType char*
#define CP_TokenType int

UGK_API extern DHTML_TagsArrayType CP_ParserTags;
UGK_API extern char CP_ParserMsgs[LOG_PARSER_MAX_LOGMSG][CP_MSG_MAX_LONG];

UGK_API typedef map<string,CP_TokenType> LexemeDictionaryType;

#define CP_NO_LEXEME -1

/**	
*	CHTMLParser

*	@author Ramon Molla
*	@version 2012-09
*/

namespace UGK
{
	class UGK_API CHTMLParser : public ILiteHTMLReaderEvents
	{

	protected:
		//Game general attributes
	
		std::ofstream	LogFile;		//For reporting about the parsing for debugging
		stack<unsigned int> StateStack;	//Stack to store the different states while parsing a file
		char Type[CP_MAX_TYPE_LENGTH];	//Name of the parser created
		LexemeDictionaryType LexemeDictionary;
		LexemeDictionaryType::iterator	CurrentToken;

		//Frees the stack so that it is completely empty
		inline void ResetState() {while (!StateStack.empty()) StateStack.pop();};

		/**
		*	\fn void CHTMLParser::TagError(const char *Text) 
		*	Virtual error manager that logs the associated error to a given TAG when parsing a file.
		*	Given by default in case the user does not create it on a given parser
		*	\param[in] Text Message to log
		*/
		void		TagError		(char *Text) {ErrorParser (Text);};
		void		ErrorClosingTag(CString TagString); ///<Logs a parser error of not matching closing tags
		void		ErrorParser	(char *msj) {LogFile << Type << " parser error: " << msj << std::endl;}

		///Translates from a string to the token that the string represents
		CP_TokenType String2Tag (CP_LexemeType Lexeme);

		///Given a token, this method converts it into its lexeme (string) associated to it
		CP_LexemeType Tag2String (CP_TokenType Token);

		virtual void BeginParse(DWORD dwAppData, bool &bAbort)
		{
			UNUSED_ALWAYS(dwAppData);
			bAbort = false;
		}

		virtual void StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
		{
			UNUSED_ALWAYS(pTag);
			UNUSED_ALWAYS(dwAppData);
			bAbort = false;
		}

		virtual void EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
		{
			UNUSED_ALWAYS(pTag);
			UNUSED_ALWAYS(dwAppData);
			bAbort = false;
		}
	
		virtual void Characters(const CString &rText, DWORD dwAppData, bool &bAbort)
		{
			UNUSED_ALWAYS(rText);
			UNUSED_ALWAYS(dwAppData);
			bAbort = false;
		}
	
		virtual void Comment(const CString &rComment, DWORD dwAppData, bool &bAbort)
		{
			UNUSED_ALWAYS(rComment);
			UNUSED_ALWAYS(dwAppData);
			bAbort = false;
		}

		virtual void EndParse(DWORD dwAppData, bool bIsAborted)
		{
			UNUSED_ALWAYS(dwAppData);
			UNUSED_ALWAYS(bIsAborted);
		}

	public:
		char	*Version;	// Game VERSION
		char	*Title;		// Game TITLE

		///Constructor
		CHTMLParser	();
		///Destructor
		~CHTMLParser	();

		void Init						(char *LogFileName);
		void VersionMismatch			(const CString &V);
	
		void		TagNotAllowed		(CP_LexemeType Tag, char* Group);
		inline void TagNotAllowed		(const CP_LexemeType Tag, char* Group){TagNotAllowed ((char*) Tag, Group);};
		void		FileTypeMismatch	(char* FileType, char* ExpectedType);
		inline void FileTypeMismatch	(const char* FileType, char* ExpectedType){FileTypeMismatch((char*) FileType, ExpectedType);};

		/**
		*	\fn void AssignTags DHTML_TagsArrayType Tags, DHTML_TagsArray OwnTags, unsigned int Size)
		*	Copy internal Tags for parsing into the external tags of the corresponding parser
		*	\param[out] Tags	The array destination
		*	\param[in]  OwnTags	The array with the tags related to the current parser
		*	\param[in]  Size	Amount of tags counting CP_ParserTags and current parser ones
		*/
		inline void AssignTags (DHTML_TagsArrayType Tags, CP_TokenType HTMLSize, CP_LexemeType OwnTags[], CP_TokenType OwnSize)
		{
			int TagsIndex, OwnTagsIndex;
	
			for(TagsIndex = 0; TagsIndex<HTMLSize;TagsIndex++) LexemeDictionary[Tags[TagsIndex]] = TagsIndex;
			for(OwnTagsIndex = 0; OwnTagsIndex<OwnSize-HTMLSize;OwnTagsIndex++, TagsIndex++) LexemeDictionary[OwnTags[OwnTagsIndex]] = TagsIndex;
		}

		/**
		*	\fn void CHTMLParser::WriteFile(HANDLE hFile)
		*	\param[in] hFile Handle to the file to open and parse
		*/
		virtual void WriteFile(HANDLE hFile){};

		/**   
		@fn TagError
		@param char Tag: this is the label that does not belong to the message
		@param char Group this is the name of the group classification label for the message
		It displays the message that the label does not belong
		*/
		inline void TagError(char *Tag, char *Group){if (LogFile) TagNotAllowed(Tag, Group);};

		char * GetMsg(CP_LogMsg Msg) {return CP_ParserMsgs[Msg];}
	};
}
#endif