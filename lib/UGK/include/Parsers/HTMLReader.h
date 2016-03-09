/**	General HTML Reader class that manages general functions of any given parser

*	Prefix CR_: Class Reader

*	@author Ramon Molla
*	@version 2014-11
*/

#ifndef CR_READER
#define CR_READER

#include <LiteHTMLReader.h>

using namespace std;

#ifndef FSTREAM_INITIALIZED
#define FSTREAM_INITIALIZED
#include <fstream>
#endif

#include <UGK.h>

/**	
*	CHTMLReader

*	@author Ramon Molla
*	@version 2014-11
*/

namespace UGK
{
	class UGK_API CHTMLReader : public CLiteHTMLReader
	{
		CFile			HTMLFile;		//File to parser in the initialization
		char*			HTMLFileName;

	public:

		inline void	SetFileName	(char * FileName)
		{
			if (HTMLFileName) delete HTMLFileName; 
			HTMLFileName = new char[strlen(FileName)+1]; //Take into account \0 null terminated string special char
			strcpy(HTMLFileName, FileName);
		}
		/**
		*	\fn void ParseFile (char * FileName)
		*	Opens a file and parses it depending on the parser loaded before invoking this method
		*	\param[in]  FileName The name of the file to parse
		*/
		inline void ParseFile	(char * FileName){SetFileName(FileName);ParseFile();};
		void		ParseFile	();

		CHTMLReader		() {HTMLFileName = NULL;};
		~CHTMLReader	() {if (NULL != HTMLFileName) delete HTMLFileName;};

	};
}
#endif
