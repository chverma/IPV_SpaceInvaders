/*	Class that loads the values of a given level of game 

	Prefix CLaP_: Class Level Parser

	@author Ramon Molla
	@version 2007-09-11
*/

#include <HTMLParser.h>
#include <UGKLanguage.h>

#define CLaP_DEBUG				//Class HTML Parser Debug activated
#define CLaP_MSJ_MAX_LONG 128	//Maximun length of a msj for the user
#define LAN_MAX_TAGS 14	//Maximun length of a sensitive tag

typedef enum {
	LOG_LANGUAGE_PARSER_ERROR_RESOURCE,
	LOG_LANGUAGE_LANGUAGE,
	LOG_LANGUAGE_PARSER_TAG_LANG_ES,
	LOG_LANGUAGE_PARSER_TAG_LANG_EN,
	LOG_LANGUAGE_PARSER_TAG_LANG_CA,
	
	LOG_LANGUAGE_MAX_LOGMSG
} CLaP_LogMsg;

/**   
	* It defines the types of labels for the analysis
    * @param enum CLaP_TagType which lists the type of tags that the parser will match during the analysis
*/
typedef enum {
	L_CONTENT = MAXTAGS_D,
	L_ID,
	L_LANGUAGE,
	L_RESOURCE,
	L_MAXTAGS
} CLaP_Lan_TagType;

/**   
	* CLanguageReader class is defined with derivation or inheritance of the class ILiteHTMLReaderEvents
    * Where is loaded default values ??at the beginning of each level in the game
*/
namespace UGK
{
	class UGK_API  CLanguageReader : public CHTMLParser
	{

	private:
		void BeginParse(DWORD dwAppData, bool &bAbort); //Inicio del analisador
		void StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort); //Comienzo de las etiquetas
		void EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort); //terminacion de las etiquetas
		void Characters(const CString &rText, DWORD dwAppData, bool &bAbort); //Caracteres
		void Comment(const CString &rComment, DWORD dwAppData, bool &bAbort); //Comentarios de las etiquetas
		void EndParse(DWORD dwAppData, bool bIsAborted); //Fin del Analisador

		//Translates from a string to the token that the string represents
		unsigned int String2Tag (const char *Name);

		CLanguage *Language;

	public:

		CLanguageReader(char * versionP, char* tituloP);
		~CLanguageReader();

		void TagError(char *Text);
		inline void TagError(char *Tag, char *Group){if (LogFile)	TagNotAllowed(Tag, Group);}
		void Init(char *LogFile);
		inline void ErrorParser(char *msj) {LogFile << "Parser error: " << msj << std::endl;}
		inline void SetLanguage (CLanguage *CurrentLanguage) {Language = CurrentLanguage;};
	};
}