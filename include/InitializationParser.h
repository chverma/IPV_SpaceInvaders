/**	Class that loads the values by default in the very beginning of the game

*	Prefix CIP_: Class Initialization Parser

*	@author Ramon Molla
*	@version 2007-09-11
*/

#include <Parsers\HTMLParser.h>
#ifndef FSTREAM_INITIALIZED
#define FSTREAM_INITIALIZED
#include <fstream>
#endif

#define CIP_DEBUG	//Class HTML Parser Debug activated
#define CIP_TAG_MAX_LONG 20	//Maximun length of a sensitive tag
#define CIP_MSG_MAX_LONG 64	//Maximun length of a msg

#define CSIG_MAX_LEVELS	99	// Max levels if Initialization.html is not correct

/** \typedef CIP_TagType

*	Types of tags the initialization parser can match during the parsing
*	They can be used also as states of of the stack machine (parser) during the parsing
*/
typedef enum {
	NIL,					///<Initial state. This tag is for management purpouses only. There is no correspondence to any string label in the HTML file
	ACCELERATION,
	AI,
	ANIMATION2D,
	BODY,					///<HTML tag
	CENTER,
	CHARACTER,				///<default values for a given character of the game
	DIR,					///<Specifies the folders where to find resources: images, languages,...
	EPS,					///<Energy Per Shoot
	EXPLOSION,
	FILE_TAG,
	FILE_TEXTURE,
	GEOMETRY,				///<Folder where to find the music and sounds
	HAVE,					///<Recursive definition in a character. A character that has mor characters inside. Not inherited but held
	HEAD,					///<HTML tag
	HEALTH,					///<How strong is the character. How much has to be hurt before dieing
	HTML,					///<HTML tag
	LANGUAGE,				///<Folder where to find the languages for the interface
	LINE,
	LINE_BUNKER,
	LINE_NAVY,
	LIVES,					///<Amount of lives the Player has still before finishing the game
	MESH,					///<3DS geometry associated to a given character //V Eliminar y cambiar por MESH_CHARACTER
	MESH_CHARACTER,			//V sustituye a MESH ///<3DS geometry associated to a given character
	MUSIC,					///<Folder where to find the music and sounds
	NAME,
	NAME_CHARACTER,
	NAME_SCORE,
	NAME_TEXTURE,
	NAME_ANI2D_CHAR,
	PERIODE_ANI2D_CHAR,
	RADIO,
	SCORE,
	SCORES,
	SHOOT,					///<Kind of shooting
	SPEED,					///<Character speed
	SPS,					///<Shoots Per Second
	SOUNDS,
	TEXTURE,
	TEXTURE2D,
	TEXTURE3D,
	TEXTURE2D_CHARACTER,
	TEXTU2D_ANI2D_CHAR,
	TEXTURE3D_CHARACTER,
	TITLE,					///<HTML tag
	TYPE,
	TYPE_CHARACTER,
	TYPE_ANIMA2D_CHAR,
	UNKNOWN,				///<This tag is for management purpouses only. There is no correspondence to any string label in the HTML file
	VERSION,
	VELOCITY,
	X,						///<First of the three space coordinates that may be used for coding the speed, position,...
	Y,						///<Second of the three space coordinates that may be used for coding the speed, position,...
	Z,						///<Third of the three space coordinates that may be used for coding the speed, position,...
	SPEEDX,					///<X speed of the character described
	SPEEDY,					///<Y speed of the character described
	SPEEDZ,					///<Z speed of the character described
	MAXTAGS					///<This tag is for management purpouses only. There is no correspondence to any string label in the HTML file
} CIP_TagType;

typedef enum {
	CIP_MIN_LIVES_NOT_DEF,
	CIP_MAX_LIVES_NOT_DEF,
	CIP_LOWER_THAN_MIN,
	CIP_HIGHR_THAN_MAX,
	CIP_AMOUNT_ALLOWED,
	CIP_VALUE_CUT,
	CIP_MINIMUM,
	CIP_MAXIMUM,
	CIP_INITIALIZATION,
	CIP_SPEED_N,
	CIP_ERRONEUS,
	CIP_MAX_MSGS
} CIP_LogMsg;

/**	
*	CInitializationReader

*	@author Ramon Molla
*	@version 2007-09-11
*/
class CInitializationReader : public CHTMLParser
{
private:

	void BeginParse(DWORD dwAppData, bool &bAbort);
    void StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort);
    void EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort);
    void Characters(const CString &rText, DWORD dwAppData, bool &bAbort);
    void Comment(const CString &rComment, DWORD dwAppData, bool &bAbort);
    void EndParse(DWORD dwAppData, bool bIsAborted);
	void TagError(const char *Text);

	///Translates from a string to the token that the string represents
	unsigned int String2Tag (const char *Name);

public:

	void Init(char *FileName);

	///Constructor
	CInitializationReader();
	///Destructor
	~CInitializationReader();

};