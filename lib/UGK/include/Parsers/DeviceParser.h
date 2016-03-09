/*	Class that loads the values of devices of game 

	Prefix CDevP_: Class Devices Parser

	@author Sonia Cardenas
	@version 2013-01-18
*/

#include <HTMLParser.h>
#ifndef FSTREAM_INITIALIZED //Se borra la macro inicializacion
#define FSTREAM_INITIALIZED //Se define la macro inicializacion
#include <fstream>
#endif

#define CDevP_DEBUG				//Class HTML Parser Debug activated
#define CDevP_MAXSTACK	  16	//Maximun deepness of the stack

/**   
   It defines the types of labels that can match the scanner during the scan that exist in the game
   @param enum CDevP_TagType which lists the types of labels to be analyzed
*/
typedef enum {
	DEVICE_D = MAXTAGS_D,
	NAME_D,
	NAME_DEVICE_D, //Name of the device
	MAXTAGS_DEV		//This tag is for management purpouses only. There is no correspondence to any string label in the HTML file
} CDevP_TagType;

/* Class that loads the values the device using game
*/
/**   
   Reading class is defined with derivation Level or class inheritance ILiteHTMLReaderEvents    
   Where is the constructor and destructor in addition to starting the analysis file
   @param CE_EXPLOSION_TYPE subType predefined variable for the types of explosion
*/
namespace UGK
{
	class UGK_API CDeviceReader : public CHTMLParser
	{

	private:
		std::ofstream	LogFile;
	
		void BeginParse(DWORD dwAppData, bool &bAbort); //Parsing begining
		void StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort); //Initial Tag
		void EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort); //Ending tag
		void Characters(const CString &rText, DWORD dwAppData, bool &bAbort); //Text containing information
		void Comment(const CString &rComment, DWORD dwAppData, bool &bAbort); //Comments
		void EndParse(DWORD dwAppData, bool bIsAborted); //Fin de  analisis
		void TagError(char *Text); //Errores de etiquetados
		void CDeviceReader::TagError(char *Tag, char *Group); //Se 
		void ErrorParser(char *msj) {LogFile << "Parser error: " << msj << std::endl;} // Mostrar error de analizador

	public:
		void Init(char *FileName);

		//Constructor and destructor
		CDeviceReader();
		~CDeviceReader();
	};
}