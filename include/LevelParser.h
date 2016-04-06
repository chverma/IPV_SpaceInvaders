/*	Class that loads the values of a given level of game 

	Prefix CLeP_: Class Level Parser

	@author Ramon Molla
	@version 2007-09-11
*/

#include <Parsers\HTMLParser.h>
#include <float.h>
#ifndef FSTREAM_INITIALIZED //Se borra la macro inicializacion del nivel
#define FSTREAM_INITIALIZED //Se define la macro inicializacion del nivel
#include <fstream>
#endif

#define CLeP_DEBUG				//Class HTML Parser Debug activated
#define CLeP_TAG_MAX_LONG 20	//Maximun length of a sensitive tag
#define CLeP_MAXSTACK	  16	//Maximun deepness of the stack
#define CLeP_MSJ_MAX_LONG 128	//Maximun length of a msj for the user

typedef enum {
	LPERROR_NOT_LINE_IN_CHARACTER,
	LPERROR_ONLY_BUNKERS,
	LPERROR_ONLY_NAME_OR_SCORE,
	LPERROR_ONLY_EPS_OR_SPS,
	LPERROR_ONLY_NAME_OR_FILE,
	LPERROR_ONLY_FOR_SHIP,
	LPERROR_MIN_SSHIP,
	LPERROR_MAX_SSHIP,
	LPERROR_MAX_LOGMSG
} CLeP_LogMsg;

/**   
   It defines the types of labels that can match the scanner during the scan that exist in the game
   @param enum CLeP_TagType which lists the types of labels to be analyzed
*/
typedef enum {
	NIL_L,		//This tag is for management purpouses only. There is no correspondence to any string label in the HTML file
	ACCELERATION_L,
	ANIMATION2D_L,
	BODY_L,		//HTML tag
	BUNKER_L,
	BOUNCE_L,
	BUNKERDEF_L,
	CENTER_L,
	CHARACTER_L,
	DIR_L,
	EPS_L,		//Energy Per Shoot
	EXPLOSION_L,
	FILE_L,
	FILE_TAG_L,
	FILE_TEXTURE_L,
	FROM_L,
	GEOMETRY_L,
	HAVE_L,		//Recursive definition of a character
	HEAD_L,		//HTML tag
	HEALTH_L,		//How strong is the character. How much has to be hurt before dieing
	HTML_L,		//HTML tag
	ID_L,
	LEVEL_L,
	LINE_L,
	LINE_BUNKER_L,
	LINE_NAVY_L,
	LIVES_L,		//Amount of ships the Player has still before finishing the game
	MESH_L,
	MESH_CHARACTER_L,
	MOVEDOWN_L,
	MUSIC_L,
	NAME_L,
	NAME_CHARACTER_L, //Name of the character
	NAME_SCORE_L,
	NAME_TEXTURE_L,	//Texture name for the character
	NAME_ANI2D_CHAR_L,
	NAVY_L,			//Enemy navy
	NUMSS_L,
	NUMLINES_L,
	PERIODE_L,
	PERIODE_ANI2D_CHAR_L,
	POSITION_L,		//Posición específica de un personaje
	RADIO_L,
	REGENERATION_L, //Regeneración Sí/No
	RENDERMODE_L,
	SCORES_L,
	SCORE_L,
	SHIPSAMOUNT_L,
	SHOOT_L,		//Kind of shooting
	SHOOTS_L,		//Max. nº de disparos en el nivel.
	SIZE_L,
	SIMULATIONMODE_L,
	SPEED_L,
	SPS_L,			//Shoots Per Second
	SUBTYPE_L,		//Only for SHIPS
	TEXTURE_L,
	TEXTURE2D_L,
	TEXTURE3D_L,
	TEXTURE2D_CHARACTER_L,
	TEXTU2D_ANI2D_CHAR_L,
	TEXTURE3D_CHARACTER_L,
	TIMECIRCLESHIP_L,
	TIMEPLAYER_L,
	TIMEPLAYER2_L,
	TIMERENDER_L,
	TIMESHIP_L,
	TIMESHOOTS_L,
	TIMEBONUS_L,
	TIMESUPPLYSHIP_L,
	TIMEUPDATE_L,
	TITLE_L,		//HTML tag
	TYPE_L,
	TYPE_BUNKER_L,
	TYPE_FILE_L,
	TYPE_CHARACTER_L,
	TYPE_ANIMA2D_CHAR_L,
	UNKNOWN_L,		//This tag is for management purpouses only. There is no correspondence to any string label in the HTML file
	VERSION_L,
	X_L,			//Pos. X
	Y_L,			//Pos. Y
	Z_L,			//Pos. Z
	VELOCITY_L,
	MAXTAGS_L		//This tag is for management purpouses only. There is no correspondence to any string label in the HTML file
} CLeP_TagType;

/* Class that loads the values by default in the very beginning of every level in the game
*/
/**   
   Reading class is defined with derivation Level or class inheritance ILiteHTMLReaderEvents    
   Where is the constructor and destructor in addition to starting the analysis file
   @param CE_EXPLOSION_TYPE subType predefined variable for the types of explosion
*/

class CLevelReader : public CHTMLParser
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
	
	unsigned int String2Tag (const char *Name);	//Virtual parser function to be declared here

public:
	void Init(char *FileName);

	//Constructor and destructor
	CLevelReader();
	~CLevelReader();
};
