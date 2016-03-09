/*	Definition of the class Language

	Prephix = LANG_

	@author Ramon Molla
	@version 2011-11
*/

#ifndef LANG_LANGUAGE ///if macro LANG_LANGUAGE is not defined before in the game...
#define LANG_LANGUAGE ///macro LANG_LANGUAGE is defined

#ifndef VECTOR_LOADED
#define VECTOR_LOADED
#include <vector>
#endif

#include <UGK.h>

#define LANG_LONG_STRING 16		//space for storing an ISO 639 language identifier or the html file associated for it

/**   
	* It defines the types of errors for the language skills
    * @param enum lang_errors which: lists the types of errors of language skills
*/
typedef enum {
	LANG_NO_ERROR,		///No error has happened
	LANG_REPEATED,		///There is just an available language with the same name as the given one
	LANG_INEXISTENT,	///There is no langugae with the given name
	LANG_NO_LANGUAGE,	///There are no languages at all in the list of languages
	LANG_MAX_ERRORS		///For error management only
} LANG_ERRORS;

/**   
	* Language class is defined
    * Where you define the language file loaded from the language directory
    * The same containing text for the labels in the game depending on which option is chosen
    * In addition to the errors generated when loading
*/


	class CLanguage
	{
	private:
		char	File	[LANG_LONG_STRING],	///<File name where the language information resides. Typically "ISO 639-2 code"+".html". e.g. spa.html or eng.html
				Name	[LANG_LONG_STRING],	///<Name of the language in english
				ISO639	[LANG_LONG_STRING];	///<ISO 639-2 three chars code identifying the languge used. e.g. "spa", "eng"
	
		bool	Ready;
		std::vector<std::string>	TextResource;	///<Array that holds all possible messages available for the log files, msg boxes and HCI in general for this given language.

	public:

		//Methods
		CLanguage(){Ready = false; File[0] = Name[0] = ISO639[0] = '\0';}

		inline void SetFile	(char *f)		{strcpy(File, f);}		//Sets the folder f where the language files are
		inline void SetName	(char *n)		{strcpy(Name, n);}		//Sets the name n of the language
		inline void SetCode	(char *c)		{strcpy(ISO639, c);}	//Sets the code c of the language
		inline void SetSize (unsigned int S){TextResource.resize(S);};

		inline char *GetFile	() {return File;}
		inline char *GetName	() {return Name;}
		inline char *GetCode	() {return ISO639;}

		inline std::string	GetText	(const unsigned int TR) {return TextResource[TR];};
		void				SetText	(const unsigned int TR, const char *text);

		bool				Load	(char	*LogFile,char * versionP, char* tituloP);
		inline bool			IsReady () {return Ready;};

		inline void FillText(int id, std::string text){TextResource[id]=text;};
		inline std::vector<std::string> getTextResource(){return TextResource;};
	};

#endif
