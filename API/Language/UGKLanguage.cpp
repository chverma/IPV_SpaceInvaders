/*	Definition of the class Laguage
  General class to manage the current language

	Prefix: LANG_
	@author Ramon Molla
	@version 2014-05
*/

#include <UGKLanguageParser.h>
#include <UGKOS.h>

using namespace UGK;

bool CLanguage::Load(char	*LogFile,char * versionP, char* tituloP)
{
	//HTML Reader
	#ifdef	UGKOS_WINDOWS
	CFile			HTMLFile;		//File to parser in the default language
	#endif

	CLanguageReader	LanguageReader(versionP,tituloP);	//The parser used to understand the contents of the language file
	CLiteHTMLReader	HTMLReader;		//The real HTML reader

	//Starting language load

	LanguageReader.Init(LogFile);
	LanguageReader.SetLanguage(this);

	HTMLReader.setEventHandler(&LanguageReader);//Assigning the language parser to the generic reader

	#ifdef	UGKOS_WINDOWS
	if (HTMLFile.Open(_T(File), CFile::modeRead))
	{
		if (HTMLReader.ReadFile((HANDLE) HTMLFile.m_hFile)) 
			 Ready = true;
		else Ready = false;

		HTMLFile.Close();
	}
	#endif

	return Ready;
}

void CLanguage::SetText	(const unsigned int TR, const char *text) {
	char a = text[0];
}