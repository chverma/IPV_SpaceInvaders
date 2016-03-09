/**	Class that loads the values of a given level of game 

	Prefix CLeP_: Class Level Parser

	@author Ramon Molla
	@version 2012-08
*/

#include <LevelParser.h>
#include "Global.h"
#include <UGKPhysics.h>
#include <UGKString.h>

extern CBunkersManager	BunkersManager;  //se llama a la variable global Manegador de Bunkers
unsigned int			CurrentBunker = CBM_MAX_BUNKERS; //Se asigna la cantidad maxima de bunkers a la variable de buncker vigentes
GCHARS_CharacterType		CLeP_CharType; //se define la variables Tipo de Caracter
UGKPHY_CINEMATIC_ACTION	CLeP_CinematicAction;
Vector					CLeP_CenterOfCoordinates; 
bool					AnimationExist=false;
char					msjAux[128];

Vector Pos;
float heightPerRow;
float weigthPerColumn;
unsigned int squareRoot, numLines;

/**   
   @fn CLeP_Tags
   @param MAXTAGS_L maximum number of tags
   @param CLeP_TAG_MAX_LONG the maximum length of the labels
   An array of allowed tags for the analyzer
*/
char CLeP_Tags[MAXTAGS_L][CLeP_TAG_MAX_LONG] = 
{
	UGKS_EMPTY_STRING,
	"ACCELERATION",
	"ANIMATION2D",
	"BODY",		//HTML tag
	"BUNKER",
	"BOUNCE",
	"TYPE",
	"CENTER",
	"CHARACTER",
	"DIR",
	"EPS",		//Energy Per Shoot
	"EXPLOSION",
	"FILE",
	"FILE",
	"FILE",
	"FROM",
	"GEOMETRY",
	"HAVE",		//Recursive definition of a character
	"HEAD",		//HTML tag
	"HEALTH",	//How strong is the character. How much has to be hurt before dieing
	"HTML",		//HTML tag
	"ID",
	"LEVEL",
	"LINE",
	"LINE",
	"LINE",
	"LIVES",	//Amount of ships the Player has still before finishing the game
	"MESH",		///3D mesh assigned to a game character
	"MESH_CHARACTER",
	"MOVEDOWN",
	"MUSIC",
	"NAME",
	"NAME",
	"NAME",
	"NAME",
	"NAME_ANI2D_CHAR",
	"NAVY",		///Enemy navy
	"NUMSS",
	"NUMLINES",
	"PERIODE",
	"PERIODE_ANI2D_CHAR",
	"POSITION",
	"RADIO",
	"REGENERATION",
	"RENDERMODE",
	"SCORES",
	"SCORE",
	"SHIPSAMOUNT",
	"SHOOT",		//Kind of shooting
	"SHOOTS",
	"SIZE",
	"SIMULATIONMODE",
	"SPEED",
	"SPS",			//Shoots Per Second
	"SUBTYPE",		//Only for SHIPS
	"TEXTURE",
	"TEXTURE2D",
	"TEXTURE3D",
	"TEXTURE2D_CHARACTER",
	"TEXTU2D_ANI2D_CHAR",
	"TEXTURE3D_CHARACTER",
	"TIMECIRCLESHIP",
	"TIMEPLAYER",
	"TIMERENDER",
	"TIMESHIP",
	"TIMESHOOTS",
	"TIMEBONUS",
	"TIMESUPPLYSHIP",
	"TIMEUPDATE",
	"TITLE",		//HTML tag
	"TYPE",
	"TYPE",
	"TYPE",
	"TYPE",
	"TYPE_ANIMA2D_CHAR",
	"UNKNOWN",	//This tag has no matching. It is for management purpouses only. 
	"VERSION",
	"X",
	"Y",
	"Z",
	"VELOCITY"
};

char CLeP_LevelParserMsgs[LPERROR_MAX_LOGMSG][CLeP_MSJ_MAX_LONG] = 
{
	"Tag LINE not in the proper context. CHARACTER does not allow a LINE attribute.",
	"Only supported different kinds of Bunkers. No other character may have \"types\".",
	"Only a NAME or SCORE command may appear in a SCORES group.",
	"Only the attribute SPS or EPS may appear in a SHOOT group. Nothing more.",
	"Only the attribute NAME or FILE may appear in a group TEXTURE. Nothing more.",
	"Tag allowed only for SHIP.",
	"Amount of SupplyShips is lower than the minimum. Reset to 1.",
	"Amount of SupplyShips is higher than the maximum. Reset to 3."
};

char CLeP_RenderModes[CHAR_MAX_RENDER_MODE][CLeP_TAG_MAX_LONG] =
{
	"NO_RENDER",			//No render for this character: camera, collision objects,...
	"2D",
	"LINES3D",
	"3D"
};

char CLeP_SimulationModes[CG_MAX_SIMULATION_MODE][CLeP_TAG_MAX_LONG] =
{
	"DISCRETE",
	"CONTINUOUS"
};

char CLeP_Ani2DTypes[MAX_NUM_ANI_TYPES][CLeP_TAG_MAX_LONG] = 
{
	"ANITYPE_CICLIC",
	"ANITYPE_PINGPONG",
	"ANITYPE_SERIAL"
};

enum{
	NO_RENDER,
	RENDER2D,
	LINES3D,
	RENDER3D
};

/**
*	\defgroup	default	Default characters
*	 Used by the parsers while reading objects
*	 Default characters for initialization purpouses only
*    Updated when the initialization parser is running
*	 @{
*/
extern CBonus			defaultBonus;
extern CBunker			defaultBunker;
extern CBrick			defaultBrick;
extern CPlayer			defaultPlayer;
extern CShip			defaultShip;
extern CAirplane		defaultAirplane;
extern CSupplyShip		defaultSShip;

/**   
   @fn TagError
   @param char Text: This is the text to be incorporated in the error message
   It displays the error message generated
*/
void CLevelReader::TagError(char *Text)
{	char	msj[128];

	if (LogFile)
	{
		strcpy(msj, CP_ParserMsgs[LOG_PARSER_TAG_CHARACTER_TYPE]);
		strcat(msj, Text);
		strcat(msj, CP_ParserMsgs[LOG_PARSER_TAG_NOT_COMPATIBLE]);
		ErrorParser (msj);
	}
}

CLevelReader::CLevelReader()
{
	strcpy(Type,"Level");	//Type of parser
	ResetState();
	StateStack.push(NIL_L);
}

CLevelReader::~CLevelReader()
{
	if (NULL != LogFile)
		LogFile.close();
	ResetState();
}

/**   
   @fn String2Tag
   Translates a string to label
   @param char Name represents the string 
   According to the first letter of the string is compared with the list of allowed tags   
   Returns the type of label is the string if it exists in the matrix of the analyzer is returned otherwise unknown label
*/
unsigned int CLevelReader::String2Tag (const char *Name)
{
switch (Name[0])
	{
	case 'A': if (strcmp(Name, CLeP_Tags[ACCELERATION_L]))
				if (strcmp(Name, CLeP_Tags[ANIMATION2D_L]))		return UNKNOWN_L;
				else											return ANIMATION2D_L;
			  else											   return ACCELERATION_L;
		break;
	case 'B': if (strcmp(Name, CLeP_Tags[BODY_L]))			
				  if (strcmp(Name, CLeP_Tags[BUNKER_L])) 
					  if (strcmp(Name, CLeP_Tags[BUNKERDEF_L]))
						if (strcmp(Name, CLeP_Tags[BOUNCE_L])) return UNKNOWN_L;
						else						return BOUNCE_L;
					  else						return BUNKERDEF_L;
				  else							return BUNKER_L;
			  else				  			    return BODY_L;
		break;
	case 'C': if (strcmp(Name, CLeP_Tags[CHARACTER_L]))
				  if (strcmp(Name, CLeP_Tags[CENTER_L]))			return UNKNOWN_L;
				else											return CENTER_L;
			else											return CHARACTER_L;	  
		break;
	case 'D': if (0 == strcmp(Name, CLeP_Tags[DIR_L]))				return DIR_L;
		break;
	case 'E': if (strcmp(Name, CLeP_Tags[EPS_L]))
				if (strcmp(Name, CLeP_Tags[EXPLOSION_L]))			return UNKNOWN_L;
				else											return EXPLOSION_L;
			  else												return EPS_L;
		 break;
	case 'F':  if(strcmp(Name, CLeP_Tags[FROM_L]))
					if(strcmp(Name, CLeP_Tags[FILE_TAG_L]))
					   if(strcmp(Name, CLeP_Tags[FILE_TEXTURE_L]))  return UNKNOWN_L;
					   else											return FILE_TEXTURE_L;
					else											return FILE_TAG_L;
			   else													return FROM_L;
		break;
	case 'G': if (0 == strcmp(Name, CLeP_Tags[GEOMETRY_L]))		return GEOMETRY_L;
		break;
	case 'H': if ('E' == Name[1])
				  if (strcmp(Name, CLeP_Tags[HEAD_L]))		
					  if (strcmp(Name, CLeP_Tags[HEALTH_L]))		return UNKNOWN_L;
					   else										return HEALTH_L;
				  else											return HEAD_L;
			  else if (strcmp(Name, CLeP_Tags[HAVE_L]))
						if (strcmp(Name, CLeP_Tags[HTML_L]))		return UNKNOWN_L;
						else									return HTML_L;
				   else											return HAVE_L;
		break;
	case 'I': if (0 == strcmp(Name, CLeP_Tags[ID_L]))		 return ID_L;
		break;
	case 'L': if (strcmp(Name, CLeP_Tags[LIVES_L]))			
				  if (strcmp(Name, CLeP_Tags[LEVEL_L])) 
					  if (strcmp(Name, CLeP_Tags[LINE_L])) return UNKNOWN_L;
					  else return LINE_L;
				  else return LEVEL_L;
			else return LIVES_L;
		break;
	case 'M': if (strcmp(Name, CLeP_Tags[MESH_L]))
				if (strcmp(Name, CLeP_Tags[MUSIC_L]))
					if (strcmp(Name, CLeP_Tags[MOVEDOWN_L]))				return UNKNOWN_L;
					else												return MOVEDOWN_L;
				else												return MUSIC_L;
			  else													return MESH_L;
		break;
	case 'N':  if (strcmp(Name, CLeP_Tags[NUMLINES_L]))
				   if (strcmp(Name, CLeP_Tags[NAVY_L]))
					  if (strcmp(Name, CLeP_Tags[NAME_L]))	
							if (strcmp(Name, CLeP_Tags[NUMSS_L]))				return UNKNOWN_L;
							else												return NUMSS_L;
					  else														return NAME_L;
				   else															return NAVY_L;
			   else																return NUMLINES_L;
		break;
	case 'P': if (strcmp(Name, CLeP_Tags[PERIODE_L]))
				if (strcmp(Name, CLeP_Tags[POSITION_L]))				return UNKNOWN_L;
				else													return POSITION_L;
			  else													return PERIODE_L;
		break;
	case 'R': if (strcmp(Name, CLeP_Tags[RADIO_L]))
				  if (strcmp(Name, CLeP_Tags[REGENERATION_L])) 
					  if(strcmp(Name, CLeP_Tags[RENDERMODE_L])) return UNKNOWN_L;
					  else										return RENDERMODE_L;
				  else											return REGENERATION_L;
			  else											return RADIO_L;
		break;
	case 'S': if (strcmp(Name, CLeP_Tags[SCORE_L]))
				if (strcmp(Name, CLeP_Tags[SCORES_L]))
					if(strcmp(Name, CLeP_Tags[SHIPSAMOUNT_L]))
					  if (strcmp(Name, CLeP_Tags[SHOOT_L]))
						if (strcmp(Name, CLeP_Tags[SPEED_L]))
						  if (strcmp(Name, CLeP_Tags[SPS_L]))
							if (strcmp(Name, CLeP_Tags[SUBTYPE_L]))
							  if (strcmp(Name, CLeP_Tags[SIZE_L]))
								if (strcmp(Name, CLeP_Tags[SHOOTS_L]))
									if (strcmp(Name, CLeP_Tags[SIMULATIONMODE_L]))	return UNKNOWN_L;
									else								return SIMULATIONMODE_L;
								else									return SHOOTS_L;
							  else										return SIZE_L;
							else										return SUBTYPE_L;
						  else											return SPS_L;
						else											return SPEED_L;
					  else												return SHOOT_L;			  
					else												return SHIPSAMOUNT_L;
				else													return SCORES_L;
			  else														return SCORE_L;
		break;
	case 'T': if  (strcmp(Name, CLeP_Tags[TEXTURE2D_L]))
				if  (strcmp(Name, CLeP_Tags[TEXTURE3D_L]))
					if  (strcmp(Name, CLeP_Tags[TEXTURE_L]))
						if (strcmp(Name, CLeP_Tags[TITLE_L]))
							if (strcmp(Name, CLeP_Tags[TYPE_L]))
								if (strcmp(Name, CLeP_Tags[TIMECIRCLESHIP_L]))
									if (strcmp(Name, CLeP_Tags[TIMEPLAYER_L]))
										if (strcmp(Name, CLeP_Tags[TIMERENDER_L]))
											if (strcmp(Name, CLeP_Tags[TIMESHIP_L]))
												if (strcmp(Name, CLeP_Tags[TIMESHOOTS_L]))
													if (strcmp(Name, CLeP_Tags[TIMESUPPLYSHIP_L]))
														if (strcmp(Name, CLeP_Tags[TIMEBONUS_L]))
															if (strcmp(Name, CLeP_Tags[TIMEUPDATE_L])) return UNKNOWN_L;
															else	return TIMEBONUS_L;
														else	return TIMEUPDATE_L;
													else		return TIMESUPPLYSHIP_L;
												else			return TIMESHOOTS_L;
											else				return TIMESHIP_L;
										else					return TIMERENDER_L;
									else						return TIMEPLAYER_L;
								else							return TIMECIRCLESHIP_L;
							else								return TYPE_L;
						else									return TITLE_L;
					else										return TEXTURE_L;
				else										return TEXTURE3D_L;
			  else										return TEXTURE2D_L;
		break;
	case 'V': if (strcmp(Name, CLeP_Tags[VERSION_L]))	
				  if (strcmp(Name, CLeP_Tags[VELOCITY_L]))	return UNKNOWN_L;
					else									return VELOCITY_L;
			  else											return VERSION_L;
		break;
	case 'X': if (0 == strcmp(Name, CLeP_Tags[X_L]))	return X_L;
		break;
	case 'Y': if (0 == strcmp(Name, CLeP_Tags[Y_L]))	return Y_L;
		break;
	case 'Z': if (0 == strcmp(Name, CLeP_Tags[Z_L]))	return Z_L;
		break;
	};
	return UNKNOWN_L;
};

/**   
   @fn Init
   Initialize Level
   @param char FileName : file name
   Number starts at 0 bunkers    
   Starts the default values ??at the beginning of each level in the game
*/
void CLevelReader::Init(char *FileName)
{
	BunkersManager.BunkersAmount = 0; // Initialization of the number of the bunkers
	LogFile.open(FileName);
	if (!LogFile)	//NAME the file
		exit(0);
	ResetState();
	StateStack.push(NIL_L);
};

/**   
   @fn BeginParse 
   @param DWORD dwAppData represents the open file
   @param bool bAbort is aborted if parser 
   This function starts the debugger parser
*/
void CLevelReader::BeginParse(DWORD dwAppData, bool &bAbort)
{
	UNUSED_ALWAYS(dwAppData);
	bAbort = false;
#ifdef CLeP_DEBUG	//Class HTML Parser Debug activated
	LogFile << CP_ParserMsgs[LOG_PARSER_BEGIN_PARSE] << UGKS_NEW_LINE_STRING;
#endif
}

/**   
   @fn StartTag
   @param CLiteHTMLTag pTag represents HTML tags
   @param DWORD dwAppData represents the open file
   @param bool bAbort represents whether the parser is aborted    
   This function defines the HTML tags permitted in the matrix of the analyzer and classified by categories or groups
*/
void CLevelReader::StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
{
	CString TagString, AuxString;
	unsigned int Tag;

	TagString = pTag->getTagName();

#ifdef CLeP_DEBUG	//Class HTML Parser Debug activated
	LogFile << CP_ParserMsgs[LOG_PARSER_START_TAG] << TagString << UGKS_NEW_LINE_STRING;
#endif

	UNUSED_ALWAYS(dwAppData);
	bAbort = false;

	Tag = String2Tag(LPCTSTR(TagString));
	switch(StateStack.top())
	{case NIL_L://Starting and ending states
				if (HTML_L == Tag)
				{//It is a HTML file. Change to HTML state
					StateStack.push(HTML_L);
					//Do some initializations
					CurrentBunker			= CBM_MAX_BUNKERS;
					CLeP_CinematicAction	= UGKPHY_TRASLATION;
					CLeP_CharType			= CHARS_CHARACTER_UNKNOWN;
					CLeP_CenterOfCoordinates.Reset();
				}
				else ErrorParser(CP_ParserMsgs[LOG_PARSER_NOT_HTML_TAG]);
		break;
	case ANIMATION2D_L:					
					switch (Tag)
					{
					case NAME_L:
						StateStack.push(NAME_ANI2D_CHAR_L);
						break;
					case PERIODE_L:
						StateStack.push(PERIODE_ANI2D_CHAR_L);
						break;
					case TEXTURE2D_L:
						StateStack.push(TEXTU2D_ANI2D_CHAR_L);
						break;
					case TYPE_L:
						StateStack.push(TYPE_ANIMA2D_CHAR_L);
						break;
					}
		break; 
	 case BODY_L:	//HTML tag
					switch (Tag)
					{
					case BUNKER_L:
					case BUNKERDEF_L:
					case DIR_L:
					case NUMSS_L:
					case CHARACTER_L:
					case REGENERATION_L:
					case SCORES_L: StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					default: 
						TagNotAllowed(TagString, CLeP_Tags[BODY_L]); 
					}
		break;
	 /*case BUNKER_L:
					switch (Tag)
					{
						case ID_L:		BunkersManager.BunkersAmount++;
							break;
						case LINE_L:	StateStack.push(LINE_BUNKER_L);
							break;
						default: ErrorParser("Tag not allowed in a BUNKER group."); 
					}
		break;*/
	 case CENTER_L:
			switch (Tag)
			{
				case X_L:
				case Y_L:
				case Z_L:
					StateStack.push(Tag);
					CLeP_CinematicAction = UGKPHY_CENTER;
					break;
				default: 
					TagNotAllowed(TagString, CLeP_Tags[POSITION_L]);
			}
		break;

	case CHARACTER_L:
					switch (Tag)
					{
					case ANIMATION2D_L:
						StateStack.push(ANIMATION2D_L);
						break;
					case BOUNCE_L:
						StateStack.push(Tag);
						break;
					case CENTER_L:
						CLeP_CinematicAction = UGKPHY_CENTER;
						StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					case LINE_L:
						///This may refers to a Bunker type or a Navy type
						if (CHARS_BUNKER == CLeP_CharType)
							Tag = LINE_BUNKER_L;
						else if (CHARS_NAVY == CLeP_CharType)
							Tag = LINE_NAVY_L;
						else ErrorParser(CLeP_LevelParserMsgs[LPERROR_NOT_LINE_IN_CHARACTER]);
						StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					case NAME_L:
						StateStack.push(NAME_CHARACTER_L); //Change to the state NAME used to identify a character
						break;
					case MESH_L:
						StateStack.push(MESH_CHARACTER_L);
						break;
					case POSITION_L:
						CLeP_CinematicAction = UGKPHY_TRASLATION;
						StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					case RENDERMODE_L:
						StateStack.push(Tag);
						break;
					case SIZE_L:
						CLeP_CinematicAction = UGKPHY_SIZE;
						StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					case SIMULATIONMODE_L:
						StateStack.push(Tag);
						break;
					case SPEED_L:
						CLeP_CinematicAction = UGKPHY_SPEED;
						StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					case TYPE_L:
						//StateStack.push(TYPE_CHARACTER_L);
						///This may refers to a bunker type
						if (CHARS_BUNKER == CLeP_CharType)
							Tag = TYPE_BUNKER_L;
						else ErrorParser(CLeP_LevelParserMsgs[LPERROR_ONLY_BUNKERS]);
						break;
					case TEXTURE2D_L:
						StateStack.push(TEXTURE2D_CHARACTER_L);
						break;
					case TEXTURE3D_L:
						StateStack.push(TEXTURE3D_CHARACTER_L);
						break;
					case ACCELERATION_L:
					case EXPLOSION_L:
					case FROM_L:
					case LIVES_L:
					case HEALTH_L:
					case HAVE_L:
					case MOVEDOWN_L:
					case NUMLINES_L:
					case RADIO_L:
					case TEXTURE_L:
					case TIMECIRCLESHIP_L:
					case TIMESHIP_L:
					case TIMESHOOTS_L:
					case TIMEBONUS_L:
					case TIMESUPPLYSHIP_L:
					case TIMEPLAYER_L:
					case TIMERENDER_L:
					case TIMEUPDATE_L:
					case SHIPSAMOUNT_L:
					case SHOOT_L:
					case SHOOTS_L:
					case SUBTYPE_L:
					case VELOCITY_L:
						StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					default:
						TagNotAllowed(TagString, CLeP_Tags[CHARACTER_L]); 
					}
					
		break;
	case DIR_L:
					switch (Tag)
					{
					case MUSIC_L:
					case TEXTURE_L:
					case GEOMETRY_L: StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					default:
						TagNotAllowed(TagString, CLeP_Tags[DIR_L]); 
					}
		break;
	case FROM_L:
				switch (Tag)
				{
					case X_L:
					case Y_L:
					case Z_L: 
							StateStack.push(Tag);
							CLeP_CinematicAction = UGKPHY_TRASLATION;
							break;
					default:
						TagNotAllowed(TagString, CLeP_Tags[FROM_L]);
				}
		break;
	case HAVE_L:
				switch (Tag)
				{
					case EXPLOSION_L:
					case LIVES_L:
					case HEALTH_L:
					case POSITION_L:
					case SPEED_L:
					case MESH_L:
					case TEXTURE_L:
					case TYPE_L:
					case SHOOT_L:
						StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					default:
						TagNotAllowed(TagString, CLeP_Tags[HAVE_L]);
				}
		 break;
	case HEAD_L:		//HTML tag
					switch (Tag)
					{
					case TYPE_L:
						//Rename Tag to the correct context
							Tag = TYPE_FILE_L;
					case TITLE_L:
					case VERSION_L:
					case LEVEL_L:
					case SCORE_L:  // ADDED THE SCORE
						StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					default:
						TagNotAllowed(TagString, CLeP_Tags[HEAD_L]);
					}
		break;
	case HEALTH_L:
					StateStack.push(Tag); //Change to the state specified by the Tag
		break;
	case HTML_L:	//HTML Tag
					//The parser is at the very beginning of the iniitalization file
					switch (Tag)
					{
					case HEAD_L:		//A HEAD Tag is expected
					case BODY_L: 
						StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					default: 
							ErrorParser(CP_ParserMsgs[LOG_PARSER_ONLY_HEAD_OR_BODY]);
							TagError(CLeP_Tags[Tag]);
					}
		break;
	case NAME_L:
	case NAME_CHARACTER_L: //Name of the character
	case NAME_SCORE_L:
	case NAME_TEXTURE_L:	//Texture name for the character
					StateStack.push(Tag); //Change to the state specified by the Tag
		break;
	/*case LINE_L:
		switch (CLeP_CharType)
		{
		case CHARS_NAVY:
			StateStack.push(LINE_NAVY_L);
			break;
		default:
			break;
		}
		break;*/
	case POSITION_L:
				switch (Tag)
				{
					case X_L:
					case Y_L:
					case Z_L: 
							StateStack.push(Tag);
							CLeP_CinematicAction = UGKPHY_TRASLATION;
							break;
					default:
						TagNotAllowed(TagString, CLeP_Tags[POSITION_L]);
				}
		break;
	case SCORES_L:	//A NAME Tag is expected
					if (NAME_L == Tag)
						//Change to NAME state
						StateStack.push(NAME_SCORE_L);

					//May be that no NAME tag appears or that the NAME section is finished.
					//There is a SCORE part also
					else if (SCORE_L == Tag)
						//Change to SCORE state
						StateStack.push(SCORE_L);
					else ErrorParser(CLeP_LevelParserMsgs[LPERROR_ONLY_NAME_OR_SCORE]);
		break;
	case SHOOT_L:		//Kind of shooting
					switch (Tag)
					{
					case SPS_L:	//Change to SPS state
					case EPS_L:	//May be that no SPS tag appears or that the SPS section is finished.
								//There is a EPS part also
						StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					default: ErrorParser(CLeP_LevelParserMsgs[LPERROR_ONLY_EPS_OR_SPS]);
					}
		break;
	case SIZE_L:
				switch (Tag)
				{
					case X_L:
					case Y_L:
					case Z_L:
							StateStack.push(Tag);
							CLeP_CinematicAction = UGKPHY_SIZE;
							break;
					default: 
						TagNotAllowed(TagString, CLeP_Tags[SIZE_L]);
				}
		break;
	case SPEED_L:
				switch (Tag)
				{
					case X_L:
					case Y_L:
					case Z_L:
							StateStack.push(Tag);
							CLeP_CinematicAction = UGKPHY_SPEED;
							break;
					default:
						TagNotAllowed(TagString, CLeP_Tags[POSITION_L]);
				}
		break;
	case TEXTURE_L:
					if (NAME_L == Tag)
						//Change to NAME state
						StateStack.push(NAME_TEXTURE_L);

					//May be that no NAME tag appears or that the NAME section is finished.
					//There is a FILE part also
					else if (FILE_TAG_L == Tag)
						//Change to FILE state
						StateStack.push(FILE_TEXTURE_L);
					else ErrorParser(CLeP_LevelParserMsgs[LPERROR_ONLY_NAME_OR_FILE]);
		break;
	default:;
	};
}

/**   
   @fn EndTag
   @param CLiteHTMLTag pTag represents HTML tags
   @param DWORD dwAppData represents the open file
   @param bool bAbort represents whether the parser is aborted    
   This function defines the end of a label acuardo permitted its category, If not shown an error message
*/
void CLevelReader::EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
{
	CString TagString;
	unsigned int Tag;
	bool Error = false;
	char msj[128];

	TagString = pTag->getTagName();

	#ifdef CLeP_DEBUG	//Class HTML Parser Debug activated
		LogFile << CP_ParserMsgs[LOG_PARSER_END_TAG_END] << pTag->getTagName() << std::endl;
	#endif
	//printf("Etiqueta fin: %s\n", pTag->getTagName());
	UNUSED_ALWAYS(dwAppData);
	bAbort = false;

	Tag = String2Tag(LPCTSTR(TagString));
	switch(StateStack.top())
	{
	 case ACCELERATION_L:
	 case BODY_L:
	 case BUNKER_L:
	 case BOUNCE_L:
	 case BUNKERDEF_L:
	 case CENTER_L:
	 case CHARACTER_L:
	 case DIR_L:
	 case EPS_L:		//Energy Per Shoot
	 case EXPLOSION_L:
	 case FROM_L:
	 case FILE_L:
	 case GEOMETRY_L:
	 case HAVE_L:		//Recursive definition of a character
	 case HEAD_L:		
	 case HEALTH_L:		//How strong is the character. How much has to be hurt before dieing
	 case ID_L:
	 case LEVEL_L:
	 case LINE_L:
	 case LIVES_L:
	 case MESH_L:
	 case MOVEDOWN_L:
	 case MUSIC_L:
	 case NAME_L:
	 case NUMSS_L:
	 case NUMLINES_L:
	 case POSITION_L:
	 case RADIO_L:
	 case REGENERATION_L:
	 case RENDERMODE_L:
	 case SCORES_L:
	 case SCORE_L:
	 case SHIPSAMOUNT_L:
	 case SHOOT_L:		
	 case SHOOTS_L:
	 case SIZE_L:
	 case SIMULATIONMODE_L:
	 case SPEED_L:
	 case SPS_L:		//Shoots Per Second
	 case SUBTYPE_L:
	case TEXTURE_L:
	case TIMECIRCLESHIP_L:
	case TIMESHIP_L:
	case TIMESHOOTS_L:
	case TIMEBONUS_L:
	case TIMESUPPLYSHIP_L:
	case TIMEPLAYER_L:
	case TIMERENDER_L:
	case TIMEUPDATE_L:
	 case TITLE_L:		
	 case TYPE_L:
	 case VERSION_L:
	 case VELOCITY_L:
	 case X_L:			//Pos. X
	 case Y_L:			//Pos. Y
	 case Z_L:
					if (StateStack.top() == Tag) StateStack.pop();	//Change to the previous state
					else Error = true;
		break;
	 case ANIMATION2D_L:
					switch (CLeP_CharType)
					{
					case CHARS_BONUS:
							break;
					case CHARS_BRICK:
							break;
					case CHARS_BUNKER:
							break;
					case CHARS_PLAYER:
						if(!AnimationExist)
							Player[CurrentPlayer].IndAnimation2D = AnimationsManager.Animations.size()-1;
						else
							Player[CurrentPlayer].IndAnimation2D = AnimationsManager.SearchIndOfName(msjAux);
							break;
					case CHARS_SHIP:
						//ACHTUNG: PENDIENTE DE CREAR SHIPS DESDE MATRIZ DE LEVEL
						/*if(!AnimationExist)
							Navy.Ship[i].IndAnimation2D = AnimationsManager.Animations.size()-1;
						else
							Navy.Ship[i].IndAnimation2D = AnimationsManager.SearchIndOfName(msjAux);*/
							break;
					case CHARS_SUPPLYSHIP:
						if(!AnimationExist)
							Navy.SupplyShip.back()->IndAnimation2D = AnimationsManager.Animations.size()-1;
						else
							Navy.SupplyShip.back()->IndAnimation2D = AnimationsManager.SearchIndOfName(msjAux);
							break;
					case CHARS_CIRCLESHIP:
						if(!AnimationExist)
							Navy.CircleShip.back()->IndAnimation2D = AnimationsManager.Animations.size()-1;
						else
							Navy.CircleShip.back()->IndAnimation2D = AnimationsManager.SearchIndOfName(msjAux);
							break;
						break;
					case CHARS_LASER:
						if(!AnimationExist)
							for(int i=0;i<CP_MAX_LASERS;i++)
								Player[CurrentPlayer].Laser[i].IndAnimation2D = AnimationsManager.Animations.size()-1;
						else
							for(int i=0;i<CP_MAX_LASERS;i++)
								Player[CurrentPlayer].Laser[i].IndAnimation2D = AnimationsManager.SearchIndOfName(msjAux);
							break;
					case CHARS_SPHERE_OGIVE:
						if(!AnimationExist)
							Background.IndAnimation2D = AnimationsManager.Animations.size()-1;
						else
							Background.IndAnimation2D = AnimationsManager.SearchIndOfName(msjAux);
						break;
					}
					AnimationExist=false;
					StateStack.pop();	//Change to the previous state
		 break;
	 case FILE_TAG_L:
	 case FILE_TEXTURE_L:
					if (FILE_L == Tag) StateStack.pop();	//Change to the previous state
					else Error = true;
		break;
	 case HTML_L:	//The parser is at the very end of the initialization file					
					if (StateStack.top() == Tag) StateStack.pop();	//Change to the previous state
					else Error = true;
		break;
	 case LINE_BUNKER_L:
	 case LINE_NAVY_L:
					if (LINE_L == Tag) StateStack.pop();	//Change to the previous state
					else Error = true;
		break;
	 case NAME_CHARACTER_L: //Name of the character
	 case NAME_SCORE_L:
	 case NAME_TEXTURE_L:	//Texture name for the character
	 case NAME_ANI2D_CHAR_L:
					if (NAME_L == Tag) StateStack.pop();	//Change to the previous state
					else Error = true;
		break;
	 case NIL_L:
	 case MAXTAGS_L:	//Closing a label when all the lables are closed is an error
					strcpy_s(msj, TagString);
					strcat_s(msj, CP_ParserMsgs[LOG_PARSER_END_TAG_MAXTAGS]);
					ErrorParser(msj);
		break;
	 case MESH_CHARACTER_L:
					if (MESH_L == Tag) StateStack.pop();	//Change to the previous state
					else Error = true;
	 case TYPE_BUNKER_L:
	 case TYPE_FILE_L:
					if (TYPE_L == Tag) StateStack.pop();	//Change to the previous state
					else Error = true;
		break;
	 case TEXTURE2D_CHARACTER_L:
	 case TEXTU2D_ANI2D_CHAR_L:
					 if (TEXTURE2D_L == Tag) StateStack.pop();	//Change to the previous state
								else Error = true;
		 break;
	 case TEXTURE3D_CHARACTER_L:
					 if (TEXTURE3D_L == Tag) StateStack.pop();	//Change to the previous state
								else Error = true;
		 break;
	 case TYPE_ANIMA2D_CHAR_L:
					 if (TYPE_L == Tag) StateStack.pop();	//Change to the previous state
								else Error = true;
		 break;
	 case PERIODE_ANI2D_CHAR_L:
					if (PERIODE_L == Tag) StateStack.pop();	//Change to the previous state
								else Error = true;
		 break;
	 case UNKNOWN_L:	//This tag is for management purpouses only. There is no correspondence to any string label in the HTML file
					strcpy_s(msj, TagString);
					strcat_s(msj, CP_ParserMsgs[LOG_PARSER_END_TAG_UNK]);
					ErrorParser(msj);
		break;
	default:;
	};	//Switch

	if (Error)
	{	strcpy_s(msj, TagString);
		strcat_s(msj, CP_ParserMsgs[LOG_PARSER_END_TAG_ERROR2]);
		if (StateStack.size()>=2)
		{
			unsigned int AuxState = StateStack.top();
			StateStack.pop();
			strcat_s(msj, CLeP_Tags[StateStack.top()]);
			StateStack.push(AuxState);
		}
		else strcat_s(msj, CLeP_Tags[StateStack.top()]);
		strcat_s(msj, CP_ParserMsgs[LOG_PARSER_END_TAG_ERROR2]);
		ErrorParser(msj);
	}
}

/**   
   @fn Characters
   @param CString &rText represents the text
   @param DWORD dwAppData represents the open file
   @param bool bAbort represents if the the parser is aborted
   This function defines the text for labels for the game   
   Otherwise it shows an error message
*/
void CLevelReader::Characters(const CString &rText, DWORD dwAppData, bool &bAbort)
{
char	msj[128];
float	f;
int		intAux;
unsigned int i;

CShip		*Ship;
CSupplyShip	*SShip;
CCircleShip *CCShip;

if (bAbort) return;

#ifdef CLeP_DEBUG	//Class HTML Parser Debug activated
LogFile << CP_ParserMsgs[LOG_PARSER_TEXT] << rText << std::endl;
#endif

	UNUSED_ALWAYS(dwAppData);
	bAbort = false;

	switch(StateStack.top())
	{
	case ACCELERATION_L:
		Player[CurrentPlayer].Acceleration.v[XDIM] = atof (rText);
		break;
	case BOUNCE_L:
		if (atoi(rText))
			BouncingMode = true;
		else
			BouncingMode = false;
		break;
	case EPS_L:		//Energy Per Shoot
					f = atof (rText);
		break;
	case EXPLOSION_L:
		f = atof(rText);
					switch (CLeP_CharType)
					{case CHARS_BONUS:
						 ///There is no possible explosion for the bonuses. No action performed
						break;
					case CHARS_BUNKER:
						BunkersManager.Bunker[CurrentBunker].Hit_duration = f;
						break;
					case CHARS_MISSIL:
						///No action possible for this game version
						break;
					 case CHARS_PLAYER:
						 Player[CurrentPlayer].Hit_duration = f;
						 break;
					 case CHARS_SHIP:						
						Navy.Ship[Navy.CurrentShip]->Hit_duration = f;
						break;					
					 case CHARS_SUPPLYSHIP:
						 Navy.SupplyShip[Navy.CurrentSupplyShip]->Hit_duration = f;
						 break;
					 case CHARS_CIRCLESHIP:
						 Navy.CircleShip[Navy.CurrentCircleShip]->Hit_duration = f;
						 break;
					 case CHARS_WEAPON:
						///No action possible for this game version
					 default:;
					}
		break;
	case FILE_TEXTURE_L:
					
		break;
	case GEOMETRY_L:
			strcpy(msj, rText);
			MeshesManager.SetFilesPath(msj);
		break;
	case HEALTH_L:	//How strong is the character. How much has to be hurt before dieing
					//It is supposed that this attribute is assigned to the current character
					intAux = atoi(rText); //Health of this given character
					switch (CLeP_CharType)
					{
					case CHARS_BUNKER:
						 if(intAux>0)
							BunkersManager.Bunker[CurrentBunker].Health = BunkersManager.Bunker[CurrentBunker].MaxHealth = intAux;
						break;
					case CHARS_PLAYER:
						 if(intAux>0)
								Player[CurrentPlayer].Health = Player[CurrentPlayer].MaxHealth = intAux;
						 else	if (intAux == CP_INFINITE_LIVES)
								Player[CurrentPlayer].Health = Player[CurrentPlayer].MaxHealth = CP_INFINITE_LIVES;
						 break;
					 case CHARS_SHIP:
						 if(intAux > 0)
							 Navy.Ship[Navy.CurrentShip]->Health = Navy.Ship[Navy.CurrentShip]->MaxHealth = intAux;						 
						 else if (intAux == CSS_HEALTH_INFINITE)
							 Navy.Ship[Navy.CurrentShip]->Health = Navy.Ship[Navy.CurrentShip]->MaxHealth = CS_HEALTH_INFINITE;
						 break;
					 case CHARS_SUPPLYSHIP:
						 if(intAux>0)
							Navy.SupplyShip[Navy.CurrentSupplyShip]->Health = Navy.SupplyShip[Navy.CurrentSupplyShip]->MaxHealth = intAux;
						 else if (intAux == CSS_HEALTH_INFINITE)
								Navy.SupplyShip[Navy.CurrentSupplyShip]->Health = Navy.SupplyShip[Navy.CurrentSupplyShip]->MaxHealth = CSS_HEALTH_INFINITE;
						 break;
					 case CHARS_CIRCLESHIP:
						 if(intAux>0)
							 Navy.CircleShip[Navy.CurrentCircleShip]->Health = Navy.CircleShip[Navy.CurrentCircleShip]->MaxHealth = intAux;
						 else if(intAux==CCS_HEALTH_INFINITE)
							 Navy.CircleShip[Navy.CurrentCircleShip]->Health = Navy.CircleShip[Navy.CurrentCircleShip]->MaxHealth = CCS_HEALTH_INFINITE;
						 break;
					 case CHARS_BONUS:
					 case CHARS_WEAPON:
					 default:;
					}
		break;
	case ID_L:
		///Not useful in this version
		break;
	case LEVEL_L:
		Game.Level = atoi (rText); 
		break;
	case LINE_L:
		///If line does not refer to a Bunker or a Navy, there is nothing to do
		break;
	case LINE_BUNKER_L:
		unsigned int length;
		CB_BRICK_TYPE BrickType;

		if (CBNK_MAX_COLUMNS <= rText.GetLength())
			 length = CBNK_MAX_COLUMNS;
		else length = rText.GetLength();

		strcpy(msj, "0");	//Default type brick, by default
		for(i=0;i<length;i++)
		{
			msj[0] = rText[i];
			BrickType = (CB_BRICK_TYPE) atoi(msj);
			BunkersManager.Bunker[CurrentBunker].Brick[BunkersManager.Bunker[CurrentBunker].FileAvailable][i].SubType = BrickType;
			if (CB_NO_BRICK == BrickType)
			{
				BunkersManager.Bunker[CurrentBunker].Brick[BunkersManager.Bunker[CurrentBunker].FileAvailable][i].Active = false;
				BunkersManager.Bunker[CurrentBunker].Brick[BunkersManager.Bunker[CurrentBunker].FileAvailable][i].Alive  = false;
			}else{
				BunkersManager.Bunker[CurrentBunker].Brick[BunkersManager.Bunker[CurrentBunker].FileAvailable][i].Active = true;
				BunkersManager.Bunker[CurrentBunker].Brick[BunkersManager.Bunker[CurrentBunker].FileAvailable][i].Alive  = true;
			}
		}
		///The rest of the bricks until reaching CBNK_MAX_COLUMNS have to be dead
		for(;i<CBNK_MAX_COLUMNS;i++)
		{
			BunkersManager.Bunker[CurrentBunker].Brick[BunkersManager.Bunker[CurrentBunker].FileAvailable][i].Alive = false;
			BunkersManager.Bunker[CurrentBunker].Brick[BunkersManager.Bunker[CurrentBunker].FileAvailable][i].Active = false;
		}

		BunkersManager.Bunker[CurrentBunker].FileAvailable++;

		///When found closing tag LINE_BUNKER_L, this method is also called and an error could happen
		StateStack.pop();
		StateStack.push(LINE_L);
		break;
	case LINE_NAVY_L:
		unsigned int size;
		CS_SHIP_TYPE ShipType;

		size = rText.GetLength();

		heightPerRow=10.f/size;
		weigthPerColumn=4.f/numLines;

		Pos.v[XDIM] = Navy.StartShipsPos.v[XDIM];
		Pos.v[YDIM] -= weigthPerColumn;

		strcpy(msj, "0");	//Default type Ship, by default
		for(i=0;i<size;i++)
		{
			msj[0] = rText[i];
			ShipType = (CS_SHIP_TYPE) atoi(msj);

			if(ShipType == CS_NO_SHIP){
				Pos.v[XDIM] += heightPerRow;
			}
			else{
				Ship = new CShip;
				*Ship = defaultShip;

				Pos.v[XDIM] += heightPerRow;
				Ship->MoveTo(Pos);

				Ship->Size.v[XDIM]	=  (.8f*5)/size;
				Ship->Size.v[YDIM]= weigthPerColumn*0.8f;

				switch (ShipType)
				{
				case CS_HIGH1:			
					Ship->SubType = CS_HIGH1;
					Ship->IndAnimation2D	= AnimationsManager.SearchIndOfName("ANI2SHIP_HIGH");
					break;
				case CS_HIGH2:
					Ship->SubType = CS_HIGH2;
					Ship->IndAnimation2D	= AnimationsManager.SearchIndOfName("ANI2SHIP_HIGH");
					break;
				case CS_HIGH3:
					Ship->SubType = CS_HIGH3;
					Ship->IndAnimation2D	= AnimationsManager.SearchIndOfName("ANI2SHIP_HIGH");
					break;
				case CS_MIDDLE:
					Ship->SubType = CS_MIDDLE;
					Ship->IndAnimation2D	= AnimationsManager.SearchIndOfName("ANI2SHIP_MIDDLE");
					break;
				case CS_LOW:
					Ship->SubType = CS_LOW;
					Ship->IndAnimation2D	= AnimationsManager.SearchIndOfName("ANI2SHIP_LOW");
					break;
				}
				Ship->Active = true;
				Ship->AI = NULL;
				Ship->AI_Init();
				if (NULL != Ship->AI)
					Ship->AI->outEvent(CS_NameTransition[CS_BORNING],NULL, (void*) Ship);

				Ship->numId = Navy.CurrentShip = Navy.Ship.size();
				Navy.Ship.push_back(Ship);	//Insert into the deque ship structure
			}//if
		}
		
		///When found closing tag LINE_NAVY_L, this method is also called and an error could happen
		StateStack.pop();
		StateStack.push(LINE_L);
		break;
	case LIVES_L:		//Amount of ships the Player has still before finishing the game
			Player[CurrentPlayer].Lives = atoi(rText);
		break;
	case MESH_CHARACTER_L:
			switch (CLeP_CharType)
			{
				case CHARS_NAVY:
					strcpy(Navy.MeshName, rText);	///Ships mesh default
					break;
				case CHARS_SUPPLYSHIP:
					strcpy(msj, rText);
					if(strcmp(msj, defaultSShip.MeshName))
					{		//return 0 if strings are equal
						MeshesManager.AddModel(msj);
						Navy.SupplyShip.back()->IndMesh = MeshesManager.Meshes.size()-1;
						Navy.SupplyShip.back()->Mesh	= MeshesManager.Meshes[Navy.SupplyShip.back()->IndMesh];
						strcpy(Navy.SupplyShip.back()->MeshName, Navy.SupplyShip.back()->Mesh->GetFileName());
					}
					else
					{
						Navy.SupplyShip.back()->IndMesh	= defaultSShip.IndMesh;
						Navy.SupplyShip.back()->Mesh	= defaultSShip.Mesh;
						strcpy(Navy.SupplyShip.back()->MeshName, defaultSShip.MeshName);
					}
					break;
				case CHARS_SHIP:
					strcpy(msj, rText);
					break;
				case CHARS_CIRCLESHIP:
					strcpy(msj, rText);
					if(strcmp(msj, defaultCShip.MeshName))
					{		//return 0 if strings are equal
						MeshesManager.AddModel(msj);
						Navy.CircleShip.back()->IndMesh = MeshesManager.Meshes.size()-1;
						Navy.CircleShip.back()->Mesh	= MeshesManager.Meshes[Navy.CircleShip.back()->IndMesh];
						strcpy(Navy.CircleShip.back()->MeshName, Navy.CircleShip.back()->Mesh->GetFileName());
					}
					else
					{
						Navy.CircleShip.back()->IndMesh	= defaultCShip.IndMesh;
						Navy.CircleShip.back()->Mesh	= defaultCShip.Mesh;
						strcpy(Navy.CircleShip.back()->MeshName, defaultCShip.MeshName);
					}
					break;
				case CHARS_PLAYER:
					strcpy(msj, rText);
					if(strcmp(msj, defaultPlayer.MeshName)){		//return 0 if strings are equal
						MeshesManager.AddModel(msj);
						Player[CurrentPlayer].IndMesh	= MeshesManager.Meshes.size()-1;
						Player[CurrentPlayer].Mesh		= MeshesManager.Meshes[Player[CurrentPlayer].IndMesh];
						strcpy(Player[CurrentPlayer].MeshName, Player[CurrentPlayer].Mesh->GetFileName());
					}
					else
					{
						Player[CurrentPlayer].IndMesh	= defaultPlayer.IndMesh;
						Player[CurrentPlayer].Mesh		= defaultPlayer.Mesh;
						strcpy(Player[CurrentPlayer].MeshName, defaultPlayer.MeshName);
					}			
					break;
				case CHARS_LASER:
					strcpy(msj, rText);
					if(strcmp(msj, defaultPlayer.Laser[CP_LEFT_LASER].MeshName))
					{		//return 0 if strings are equal
						MeshesManager.AddModel(msj);		
						Player[CurrentPlayer].Laser[CP_LEFT_LASER].IndMesh	= MeshesManager.Meshes.size()-1;
						Player[CurrentPlayer].Laser[CP_LEFT_LASER].Mesh		= MeshesManager.Meshes[Player[CurrentPlayer].Laser[CP_LEFT_LASER].IndMesh];
						strcpy(Player[CurrentPlayer].Laser[CP_LEFT_LASER].MeshName, Player[CurrentPlayer].Laser[CP_LEFT_LASER].Mesh->GetFileName());
				
						Player[CurrentPlayer].Laser[CP_RIGHT_LASER].IndMesh = MeshesManager.Meshes.size()-1;
						Player[CurrentPlayer].Laser[CP_RIGHT_LASER].Mesh	= MeshesManager.Meshes[Player[CurrentPlayer].Laser[CP_RIGHT_LASER].IndMesh];
						strcpy(Player[CurrentPlayer].Laser[CP_RIGHT_LASER].MeshName, Player[CurrentPlayer].Laser[CP_RIGHT_LASER].Mesh->GetFileName());
					}
					else
					{		//return 0 if strings are equal
						MeshesManager.AddModel(msj);		
						Player[CurrentPlayer].Laser[CP_LEFT_LASER].IndMesh	= defaultPlayer.Laser[CP_LEFT_LASER].IndMesh;
						Player[CurrentPlayer].Laser[CP_LEFT_LASER].Mesh		= defaultPlayer.Laser[CP_LEFT_LASER].Mesh;
						strcpy(Player[CurrentPlayer].Laser[CP_LEFT_LASER].MeshName, defaultPlayer.Laser[CP_LEFT_LASER].Mesh->GetFileName());
				
						Player[CurrentPlayer].Laser[CP_RIGHT_LASER].IndMesh = defaultPlayer.Laser[CP_LEFT_LASER].IndMesh;
						Player[CurrentPlayer].Laser[CP_RIGHT_LASER].Mesh	= defaultPlayer.Laser[CP_LEFT_LASER].Mesh;
						strcpy(Player[CurrentPlayer].Laser[CP_RIGHT_LASER].MeshName, defaultPlayer.Laser[CP_LEFT_LASER].Mesh->GetFileName());
					}
				
					break;
				case CHARS_SPHERE_OGIVE:
					strcpy(msj, rText);
					if(strcmp(msj, Background.MeshName)){
						MeshesManager.AddModel(msj);
						Background.IndMesh = MeshesManager.Meshes.size()-1;
						Background.Mesh = MeshesManager.Meshes[Background.IndMesh];
						strcpy(Background.MeshName, Background.Mesh->GetFileName());
					}
					break;
				case CHARS_BONUS:
				case CHARS_WEAPON:
				default:;
			}
		break;
	case MOVEDOWN_L:
			Navy.ShipsMoveDown = atoi (rText);
		break;
	case MUSIC_L:
					strcpy(Game.Directories[CG_MUSIC_DIR], rText);
		break;
	/*	In this case, A new node for the Scene Graph is performed.
		Type, casting and dynamic memory allocation has to be done
	*/
	case NAME_L:///Generic treatment. Only for management purpouses. This parser is only sensitive to specific "names"
		break;
	case NAME_CHARACTER_L:
		strcpy(msj, rText);
		CLeP_CharType = (GCHARS_CharacterType) CharacterFactory.String2Tag(msj);
		switch (CLeP_CharType)
			{
			case CHARS_BONUS:
				///They are predefined by the program. No allowed yet in this version.
				break;
			case CHARS_BUNKER:
				///The bunkers are already just created by default. There is an array of bunkers. No action performed
				///Bricks distribution will be created later
				CurrentBunker = BunkersManager.BunkerAvailable;		///<Remember the current editing bunker
				BunkersManager.NewBunker(CLeP_CenterOfCoordinates);	///<Generates a new one
				break;
			case CHARS_CIRCLESHIP:
				CCShip = new CCircleShip; //(CCircleShip*) CharacterFactory.Create(msj, 1);
				*CCShip=defaultCShip;
				CCShip->AI = NULL;
				CCShip->AI_Init();
				CCShip->AI->outEvent(CCS_NameTransition[CCS_BORNING],NULL,(void*)CCShip); //v 0->1
				CCShip->numId = Navy.CurrentCircleShip = Navy.CircleShip.size();
				Navy.CircleShip.push_back(CCShip);
				break;
			case CHARS_GAME:	
				///Updated during playing time in 3D mode. Not available for specification during level loading 
				break;
			case CHARS_LASER:	
				///Updated during playing time in 3D mode. Not available for specification during level loading 
				break;
			case CHARS_MISSIL:
				///Created during playing time in 3D mode. Not available for specification during level loading 
				break;
			case CHARS_NAVY:
				Navy.Ship.clear();
				numLines=0;
				break;
			case CHARS_PLAYER:
				//The Players are be defined in Global definitions. If it is not defined, a new player has to be created here.
			/*	CurrentPlayer++;	//The first time CurrentPlayer passes from CP_NO_PLAYER (-1) to player 0
				if (CP_MAX_PLAYERS < CurrentPlayer) 
					CurrentPlayer = CP_MAX_PLAYERS-1;	///There is a limit for the amount of simultaneous players 
			*/
				CurrentPlayer			= CP_DEFAULT_PLAYER;
				Player[CurrentPlayer]	= defaultPlayer;
				break;
			case CHARS_SHIP:
				//Ship = new CShip;
				//*Ship = defaultShip;
				//Ship->AI = NULL;
				////Ship->AI = AIManager.GetAI(SIAI_SHIP_DEFAULT);
				//Ship->AI_Init();
				//if (NULL != Ship->AI)
				//	Ship->AI->outEvent(CS_NameTransition[CS_BORNING],NULL, (void*) Ship);
				//	Navy.Ship.push_back(Ship);	//Insert into the deque ship structure
				break;
			case CHARS_SPHERE_OGIVE:
				///Not available by the moment
				break;
			case CHARS_SUPPLYSHIP:
				//Create a new node to set inside the scene graph
				SShip = new CSupplyShip; //*) CharacterFactory.Create(CHARS_SUPPLYSHIP, 1);
				*SShip = defaultSShip;
				SShip->AI = NULL;
				SShip->AI_Init();
				SShip->AI->outEvent(CSS_NameTransition[CSS_BORNING], NULL, (void*) SShip);	//v 0->1
				SShip->numId = Navy.CurrentSupplyShip = Navy.SupplyShip.size();
				Navy.SupplyShip.push_back(SShip);	//Insert into the deque Supply Ship structure
				break;
			case CHARS_WEAPON:
				///Not available by the moment
				break;
			case CHARS_CHARACTER_UNKNOWN:
				strcpy(msj, rText);
				TagError(msj);
				break;
			default:;
		};
		break;
	case NAME_SCORE_L:
		//score = atoi(rText);
		//if(score > hiscore){ hiscore = score; }
		break;
	case NAME_TEXTURE_L:
		break;
	case NAME_ANI2D_CHAR_L:
			switch (CLeP_CharType)
			{
			case CHARS_BONUS:
			case CHARS_BRICK:
			case CHARS_BUNKER:
			case CHARS_PLAYER:
			case CHARS_SHIP:
			case CHARS_SUPPLYSHIP:
			case CHARS_CIRCLESHIP:
			case CHARS_LASER:
			case CHARS_SPHERE_OGIVE:
					strcpy(msj, rText);
					if(-1 == AnimationsManager.SearchIndOfName(msj))
						AnimationsManager.CreateAnimation(msj,(double) 0,(ANI_ANIMATION_TYPE)0);
					else{
						AnimationExist=true;
						strcpy(msjAux, msj);
					}
				break;
			}
		break;
	case NUMSS_L:
				Game.SSxLEV=atoi(rText);
				if(Game.SSxLEV<1){
						strcpy(msj,	CLeP_LevelParserMsgs[LPERROR_MIN_SSHIP]);
						strcat(msj, rText);
						Game.SSxLEV = 1;
				}
				if(Game.SSxLEV>3){
						strcpy(msj, CLeP_LevelParserMsgs[LPERROR_MAX_SSHIP]);
						strcat(msj, rText);
						Game.SSxLEV = 3;
				}
		break;
	case NUMLINES_L:
		switch (CLeP_CharType)
			{
			case CHARS_NAVY:
				numLines = atof (rText);
				break;
			}
		break;
	case PERIODE_ANI2D_CHAR_L:
			switch (CLeP_CharType)
			{
			case CHARS_BONUS:
			case CHARS_BRICK:
			case CHARS_BUNKER:
			case CHARS_PLAYER:
			case CHARS_SHIP:
			case CHARS_SUPPLYSHIP:
			case CHARS_CIRCLESHIP:
			case CHARS_LASER:
			case CHARS_SPHERE_OGIVE:
				if (!AnimationExist)
					AnimationsManager.Animations[AnimationsManager.Animations.size()-1]->SetPeriod(atof (rText));
				else 
					AnimationsManager.Animations[AnimationsManager.SearchIndOfName(msjAux)]->SetPeriod(atof (rText));
				break;
			}
		break;
	case POSITION_L:
		break;
	case RADIO_L:
			switch (CLeP_CharType)
			{
			case CHARS_CIRCLESHIP:
				Navy.CircleShip.back()->Radio = atof(rText);
				break;
			}
		break;
	case RENDERMODE_L:
		if(rText==CLeP_RenderModes[NO_RENDER]){
			Game.NextRenderMode=(CHAR_RENDER_MODE)NO_RENDER;
		}else if(rText==CLeP_RenderModes[RENDER2D]){
			Game.NextRenderMode=(CHAR_RENDER_MODE)RENDER2D;
		}else if(rText==CLeP_RenderModes[LINES3D]){
			Game.NextRenderMode=(CHAR_RENDER_MODE)LINES3D;
		}else if(rText==CLeP_RenderModes[RENDER3D]){
			Game.NextRenderMode=(CHAR_RENDER_MODE)RENDER3D;
		}
		break;
	case REGENERATION_L:
			int M1;
			M1=atoi(rText);
/*			if(M1==1)
				//Navy.ReGenShips=true;
			else
				//Navy.ReGenShips=false;
*/		break;
	case SCORE_L:
		Game.Score = atoi(rText);
		if(Game.Score > Game.HiScore) Game.HiScore = Game.Score;
		break;
	case SHIPSAMOUNT_L:
		//Navy.ShipsAmount=atoi(rText);
		break;
	case SHOOTS_L:
			Navy.WithShoots = atoi (rText);
		break;
	case SIMULATIONMODE_L:
		if(rText==CLeP_SimulationModes[CG_DISCRETE]){
			DiscreteSimulation = true;
		}else if(rText==CLeP_SimulationModes[CG_CONTINUOUS]){
			DiscreteSimulation = false;
		}
		break;
	case SPEED_L:
		break;
	case SPS_L:		//Shoots Per Second
		break;
	case SUBTYPE_L:
			CS_SHIP_TYPE SubType;
			strcpy(msj,rText);
			SubType = (CS_SHIP_TYPE) atoi(msj);
			switch (CLeP_CharType)
				{
					case CHARS_SHIP:
						Navy.Ship[Navy.CurrentShip]->SubType = SubType;
					break;
					default: 
						ErrorParser(CLeP_LevelParserMsgs[LPERROR_ONLY_FOR_SHIP]);
				};
		break;	
	case TEXTURE_L:
			strcpy(Game.Directories[CG_TEXTURE_DIR], rText);
		break;
	case TEXTURE2D_CHARACTER_L:
		switch (CLeP_CharType)
			{
			case CHARS_BONUS:
				 break;
			case CHARS_BRICK:
				 break;
			case CHARS_BUNKER:
				 break;
			case CHARS_PLAYER:
				strcpy(msj, rText);
				if(strcmp(msj, TexturesManager.Textures[defaultPlayer.IndTexture2D]->GetFileName())){
					TexturesManager.CreateTexture(msj);
					Player[CurrentPlayer].IndTexture2D = TexturesManager.Textures.size()-1;
				}
				 break;
			case CHARS_SHIP:
				strcpy(msj, rText);
				if(strcmp(msj, TexturesManager.Textures[defaultShip.IndTexture2D]->GetFileName())){
					TexturesManager.CreateTexture(msj);
					Navy.Ship.back()->IndTexture2D = TexturesManager.Textures.size()-1;
				}
				 break;
			case CHARS_SUPPLYSHIP:
				strcpy(msj, rText);
				if(strcmp(msj, TexturesManager.Textures[defaultSShip.IndTexture2D]->GetFileName())){
					TexturesManager.CreateTexture(msj);
					Navy.SupplyShip.back()->IndTexture2D = TexturesManager.Textures.size()-1;
				}
				 break;
			case CHARS_CIRCLESHIP:
				strcpy(msj, rText);
				if(strcmp(msj, TexturesManager.Textures[defaultCShip.IndTexture2D]->GetFileName())){
					TexturesManager.CreateTexture(msj);
					Navy.CircleShip.back()->IndTexture2D = TexturesManager.Textures.size()-1;
				}
				 break;
			case CHARS_SPHERE_OGIVE:
				break;
			}
		break;
	case TEXTU2D_ANI2D_CHAR_L:
			switch (CLeP_CharType)
			{
			case CHARS_BONUS:
				 break;
			case CHARS_BRICK:
				 break;
			case CHARS_BUNKER:
				 break;
			case CHARS_PLAYER:
				strcpy(msj, rText);		
				if(!AnimationExist){
					TexturesManager.CreateTexture(msj);
					AnimationsManager.Animations.back()->AddPhotogram(TexturesManager.Textures.back());
				}
				 break;
			case CHARS_SHIP:
				if(!AnimationExist){
					strcpy(msj, rText);
					TexturesManager.CreateTexture(msj);
					AnimationsManager.Animations.back()->AddPhotogram(TexturesManager.Textures.back());
				}
				 break;
			case CHARS_SUPPLYSHIP:
				if(!AnimationExist){
					strcpy(msj, rText);
					TexturesManager.CreateTexture(msj);
					AnimationsManager.Animations.back()->AddPhotogram(TexturesManager.Textures.back());
				}
				 break;
			case CHARS_CIRCLESHIP:
				if(!AnimationExist){
					strcpy(msj, rText);
					TexturesManager.CreateTexture(msj);
					AnimationsManager.Animations.back()->AddPhotogram(TexturesManager.Textures.back());
				}
				 break;
			case CHARS_SPHERE_OGIVE:			
				break;
			}
		break;
	case TEXTURE3D_CHARACTER_L:
		switch (CLeP_CharType)
			{
			case CHARS_BONUS:
				 break;
			case CHARS_BRICK:
				 break;
			case CHARS_BUNKER:
				 break;
			case CHARS_PLAYER:
				strcpy(msj, rText);
				if(strcmp(msj, TexturesManager.Textures[defaultPlayer.IndTexture3D]->GetFileName())){
					TexturesManager.CreateTexture(msj);
					Player[CurrentPlayer].IndTexture3D = TexturesManager.Textures.size()-1;
				}
				 break;
			case CHARS_SHIP:
				strcpy(msj, rText);
				if(strcmp(msj, TexturesManager.Textures[defaultShip.IndTexture3D]->GetFileName())){
					TexturesManager.CreateTexture(msj);
					Navy.Ship.back()->IndTexture3D = TexturesManager.Textures.size()-1;
				}
				 break;
			case CHARS_SUPPLYSHIP:
				strcpy(msj, rText);
				if(strcmp(msj, TexturesManager.Textures[defaultSShip.IndTexture3D]->GetFileName())){
					TexturesManager.CreateTexture(msj);
					Navy.SupplyShip.back()->IndTexture3D = TexturesManager.Textures.size()-1;
				}
				 break;
			case CHARS_CIRCLESHIP:
				strcpy(msj, rText);
				if(strcmp(msj, TexturesManager.Textures[defaultCShip.IndTexture3D]->GetFileName())){
					TexturesManager.CreateTexture(msj);
					Navy.CircleShip.back()->IndTexture3D = TexturesManager.Textures.size()-1;
				}
				 break;
			case CHARS_SPHERE_OGIVE:
				strcpy(msj, rText);
				if(strcmp(msj, TexturesManager.Textures[Background.IndTexture3D]->GetFileName())){
					TexturesManager.CreateTexture(msj);
					Background.IndTexture3D = TexturesManager.Textures.size()-1;
				}
				break;
			}
		break;
	case TIMECIRCLESHIP_L:
		Game.CircleShipTemp=atoi(rText);
		break;
	case TIMESHIP_L:
		Game.ShipTemp=atoi(rText);
		Game.NavyShipsTemp=atoi(rText);
		break;
	case TIMESHOOTS_L:
		Game.ShootsTemp=atoi(rText);
		break;
	case TIMEBONUS_L:
		Game.BonusTemp=atoi(rText);
		break;
	case TIMESUPPLYSHIP_L:
		Game.SupplyShipTemp=atoi(rText);
		break;
	case TIMEPLAYER_L:
		Game.PlayerTemp=atoi(rText);
		break;
	case TIMERENDER_L:
		Game.RenderTemp=atof(rText);
		break;
	case TIMEUPDATE_L:
		Game.UpdateTemp=atof(rText);
		break;
	case TITLE_L:	//HTML tag
		if (strcmp(rText, Game.GetTitle()/*Title*/))
					{
						ErrorParser (CP_ParserMsgs[LOG_PARSER_NOT_THIS_VG]);
						bAbort = true;
					}
		break;
	case TYPE_L:
		//No treatment for this label. An error has occurred
		break;
	case TYPE_BUNKER_L:
					if (strcmp(CBNK_TypeNames[CBNK_BLANK], rText))
						if (strcmp(CBNK_TypeNames[CBNK_HOME], rText))
							if (strcmp(CBNK_TypeNames[CBNK_USER_DEFINED], rText))
								{
									MessageBox(HWND_DESKTOP, "Bunker type not known. Creating a blank one by default", "Error", MB_OK | MB_ICONEXCLAMATION);
									BunkersManager.DefaultBunker = CBNK_BLANK;
								}
							else	BunkersManager.DefaultBunker = CBNK_USER_DEFINED;
						else		BunkersManager.DefaultBunker = CBNK_HOME;
					else			BunkersManager.DefaultBunker = CBNK_BLANK;
					///When found closing tag TYPE_BUNKER_L, this method is also called and an error could happen
					StateStack.pop();
					StateStack.push(TYPE_L);
		break;
	case TYPE_FILE_L:
					if (strcmp(rText, "Level"))
					{
						FileTypeMismatch(rText, CLeP_Tags[LEVEL_L]);
						bAbort = true;
					}
					else ///When found closing tag TYPE_FILE_L, this method is also called and an error could happen
					StateStack.pop();
					StateStack.push(TYPE_L);
		break;
	case TYPE_ANIMA2D_CHAR_L:
			switch (CLeP_CharType)
			{
			case CHARS_BONUS:
			case CHARS_BRICK:
			case CHARS_BUNKER:
			case CHARS_PLAYER:
			case CHARS_SHIP:
			case CHARS_SUPPLYSHIP:
			case CHARS_CIRCLESHIP:
			case CHARS_LASER:
			case CHARS_SPHERE_OGIVE:
				if(!AnimationExist){
					if (0 == strcmp(rText, CLeP_Ani2DTypes[ANITYPE_CICLIC]))
						AnimationsManager.Animations[AnimationsManager.Animations.size()-1]->Type = ANITYPE_CICLIC;
					if (0 == strcmp(rText, CLeP_Ani2DTypes[ANITYPE_PINGPONG]))
						AnimationsManager.Animations[AnimationsManager.Animations.size()-1]->Type = ANITYPE_PINGPONG;
					if (0 == strcmp(rText, CLeP_Ani2DTypes[ANITYPE_SERIAL]))
						AnimationsManager.Animations[AnimationsManager.Animations.size()-1]->Type = ANITYPE_SERIAL;
				}else{
					if (0 == strcmp(rText, CLeP_Ani2DTypes[ANITYPE_CICLIC]))
						AnimationsManager.Animations[AnimationsManager.SearchIndOfName(msjAux)]->Type = ANITYPE_CICLIC;
					if (0 == strcmp(rText, CLeP_Ani2DTypes[ANITYPE_PINGPONG]))
						AnimationsManager.Animations[AnimationsManager.SearchIndOfName(msjAux)]->Type = ANITYPE_PINGPONG;
					if (0 == strcmp(rText, CLeP_Ani2DTypes[ANITYPE_SERIAL]))
						AnimationsManager.Animations[AnimationsManager.SearchIndOfName(msjAux)]->Type = ANITYPE_SERIAL;
				}
				break;
			}
		break;
	case VELOCITY_L:
			Navy.CircleShip[Navy.CurrentCircleShip]->Velocity=atof(rText);
			break;
	case VERSION_L:	if (strcmp(rText, Game.GetVersion()))
					{
						VersionMismatch(rText);
						bAbort = true;
					}
		break;
	case X_L:
		f = atof(rText);
		switch(CLeP_CinematicAction)
		{
		case UGKPHY_CENTER:
				switch (CLeP_CharType)
				{
				case CHARS_CIRCLESHIP:
					Navy.CircleShip[Navy.CurrentCircleShip]->Center.v[XDIM]=f;
				break;
				}
			break;
		case UGKPHY_TRASLATION:
				switch (CLeP_CharType)
				{
				 case CHARS_BUNKER:
					 BunkersManager.Bunker[CurrentBunker].TranslateBunker(Vector(f, FLT_MAX, FLT_MAX));
					 break;
				 case CHARS_NAVY:
					 Navy.StartShipsPos.v[XDIM] = f;
					 break;
				 case CHARS_PLAYER:
					 Player[CurrentPlayer].MoveTo(f, Player[CurrentPlayer].Position.v[YDIM], Player[CurrentPlayer].Position.v[ZDIM]);
					 break;
				 case CHARS_SUPPLYSHIP:
					 Navy.SupplyShip[Navy.CurrentSupplyShip]->MoveTo(f, Navy.SupplyShip[Navy.CurrentSupplyShip]->Position.v[YDIM], Navy.SupplyShip[Navy.CurrentSupplyShip]->Position.v[ZDIM]);
					 break;
				 case CHARS_BONUS:
				 case CHARS_SHIP:
					 Navy.Ship[Navy.CurrentShip]->MoveTo(f, Navy.Ship[Navy.CurrentShip]->Position.v[YDIM], Navy.Ship[Navy.CurrentShip]->Position.v[ZDIM]);
					 break;
				 case CHARS_WEAPON:
				 default:;
				}	//switch CLeP_CharType
			break;
		case UGKPHY_SPEED:
				switch (CLeP_CharType)
				{
				 case CHARS_BUNKER:
					 BunkersManager.Bunker[CurrentBunker].Speed.v[XDIM] = f;
					 break;
				 case CHARS_NAVY:
					 Navy.Speed.v[XDIM] = f;
					 for(i=0;i<Navy.Ship.size();i++){
						Navy.Ship[i]->Speed.v[XDIM] = f;
					 }
					 break;
				 case CHARS_PLAYER:
					 Player[CurrentPlayer].Speed.v[XDIM] = f;
					 break;
				 case CHARS_SUPPLYSHIP:
					 Navy.SupplyShip[Navy.CurrentSupplyShip]->Speed.v[XDIM] = f;
					 break;
				 case CHARS_BONUS:
				 case CHARS_SHIP:
					 Navy.Ship[Navy.CurrentShip]->Speed.v[XDIM] = f;
					 break;
				 case CHARS_WEAPON:
				 default:;
				}	//switch CLeP_CharType
		case UGKPHY_SIZE:
			switch (CLeP_CharType)
				{
				 case CHARS_SHIP:
					 Navy.Ship[Navy.CurrentShip]->Size.v[XDIM] = f;
					 break;
				 default:;
				}	//switch CLeP_CharType
			}	//switch(CLeP_CinematicAction)
		break;
	case Y_L:
		f = atof(rText);
		switch(CLeP_CinematicAction)
		{
		case UGKPHY_CENTER:
				switch (CLeP_CharType)
				{
				case CHARS_CIRCLESHIP:
					Navy.CircleShip[Navy.CurrentCircleShip]->Center.v[YDIM]=f;
				break;
				}
			break;
		case UGKPHY_TRASLATION:
				switch (CLeP_CharType)
				{
				 case CHARS_BUNKER:
					 BunkersManager.Bunker[CurrentBunker].TranslateBunker(Vector(FLT_MAX,f,FLT_MAX));
					 break;
				 case CHARS_NAVY:
					 Navy.StartShipsPos.v[YDIM] = f;
					 Pos.v[YDIM] = Navy.StartShipsPos.v[YDIM];
					 break;
				 case CHARS_PLAYER:
					 Player[CurrentPlayer].MoveTo(Player[CurrentPlayer].Position.v[XDIM], f, Player[CurrentPlayer].Position.v[ZDIM]);
					 break;
				 case CHARS_SUPPLYSHIP:
					 Navy.SupplyShip[Navy.CurrentSupplyShip]->MoveTo(Navy.SupplyShip[Navy.CurrentSupplyShip]->Position.v[XDIM], f, Navy.SupplyShip[Navy.CurrentSupplyShip]->Position.v[ZDIM]);
					 break;
				 case CHARS_BONUS:
				 case CHARS_SHIP:
					 Navy.Ship[Navy.CurrentSupplyShip]->MoveTo(Navy.Ship[Navy.CurrentSupplyShip]->Position.v[XDIM], f, Navy.Ship[Navy.CurrentSupplyShip]->Position.v[ZDIM]);
					 break;
				 case CHARS_WEAPON:
				 default:;
				}	//switch CLeP_CharType
			break;
		case UGKPHY_SPEED:
				switch (CLeP_CharType)
				{
				 case CHARS_BUNKER:
					 BunkersManager.Bunker[CurrentBunker].Speed.v[YDIM] = f;
					 break;
				 case CHARS_NAVY:
					 Navy.Speed.v[YDIM] = f;
					 for(i=0;i<Navy.Ship.size();i++){
						Navy.Ship[i]->Speed.v[YDIM] = f;
					 }
					 break;
				 case CHARS_PLAYER:
					 Player[CurrentPlayer].Speed.v[YDIM] = f;
					 break;
				 case CHARS_SUPPLYSHIP: 
					 Navy.SupplyShip[Navy.CurrentSupplyShip]->Speed.v[YDIM] = f;
					 break;
				 case CHARS_BONUS:
				 case CHARS_SHIP:
					 Navy.Ship[Navy.CurrentShip]->Speed.v[YDIM] = f;
					 break;
				 case CHARS_WEAPON:
				 default:;
				}
		case UGKPHY_SIZE:
			switch (CLeP_CharType)
				{
				 case CHARS_SHIP:
					 Navy.Ship[Navy.CurrentShip]->Size.v[YDIM] = f;
					 break;
				 default:;
				}	//switch CLeP_CharType
		}	//switch(CLeP_CinematicAction)
		break;
	case Z_L:
		f = atof(rText);
		switch(CLeP_CinematicAction)
		{
			case UGKPHY_CENTER:
				switch (CLeP_CharType)
				{
				case CHARS_CIRCLESHIP:
					Navy.CircleShip[Navy.CurrentCircleShip]->Center.v[ZDIM]=f;
				break;
				}
			break;
		
		case UGKPHY_TRASLATION:
				switch (CLeP_CharType)
				{
				 case CHARS_BUNKER:
					 BunkersManager.Bunker[CurrentBunker].TranslateBunker(Vector(FLT_MAX,FLT_MAX,f));break;
				 case CHARS_NAVY:
					 Navy.StartShipsPos.v[ZDIM] = f;
					 Pos.v[ZDIM] = Navy.StartShipsPos.v[ZDIM];
					 break;
				 case CHARS_PLAYER:
					 Player[CurrentPlayer].MoveTo(Player[CurrentPlayer].Position.v[XDIM], Player[CurrentPlayer].Position.v[YDIM], f);
					 break;
				 case CHARS_SUPPLYSHIP:
					 Navy.SupplyShip[Navy.CurrentSupplyShip]->MoveTo(Navy.SupplyShip[Navy.CurrentSupplyShip]->Position.v[XDIM], Navy.SupplyShip[Navy.CurrentSupplyShip]->Position.v[YDIM], f);
					 break;
				 case CHARS_BONUS:
				 case CHARS_SHIP:
					 Navy.Ship[Navy.CurrentShip]->MoveTo(Navy.Ship[Navy.CurrentShip]->Position.v[XDIM], Navy.Ship[Navy.CurrentShip]->Position.v[YDIM], f);
					 break;
				 case CHARS_WEAPON:
				 default:;
				}	//switch CLeP_CharType
			break;
		case UGKPHY_SPEED:
				switch (CLeP_CharType)
				{
				 case CHARS_BUNKER:
					 BunkersManager.Bunker[CurrentBunker].Speed.v[ZDIM] = f;
					 break;
				 case CHARS_NAVY:
					 Navy.Speed.v[ZDIM] = f;
					 for(i=0;i<Navy.Ship.size();i++){
						Navy.Ship[i]->Speed.v[ZDIM] = f;
					 }
					 break;
				 case CHARS_PLAYER:
					 Player[CurrentPlayer].Speed.v[ZDIM] = f;
					 break;
				 case CHARS_SUPPLYSHIP:
					 Navy.SupplyShip[Navy.CurrentSupplyShip]->Speed.v[ZDIM] = f;
					 break;
				 case CHARS_BONUS:
				 case CHARS_SHIP:
					 Navy.Ship[Navy.CurrentShip]->Speed.v[ZDIM] = f;
					 break;
				 case CHARS_WEAPON:
				 default:;
				}
		case UGKPHY_SIZE:
			switch (CLeP_CharType)
				{
				 case CHARS_SHIP:
					 Navy.Ship[Navy.CurrentShip]->Size.v[ZDIM] = f;
					 break;
				 default:;
				}	//switch CLeP_CharType//switch CLeP_CharType
		}	//switch(CLeP_CinematicAction)
		break;
	//default:;		//Tags not supported are not managed, even if they have content associated
	};	//Switch
}

/**   
   @fn Comment
   @param CString &rComment represents the string of comments
   @param DWORD dwAppData represents the open file
   @param bool bAbort represents if the parser is aborted    
   This function defines Entries for tags
*/
void CLevelReader::Comment(const CString &rComment, DWORD dwAppData, bool &bAbort)
{
	printf(CP_ParserMsgs[LOG_PARSER_COMMENT], rComment);
	UNUSED_ALWAYS(dwAppData);
	bAbort = false;
}

/**   
   @fn EndParse
   @param DWORD dwAppData represents the open file
   @param bool bAbort represents if the parser is aborted  
   This function closes the file if the parser is aborted  
*/
void CLevelReader::EndParse(DWORD dwAppData, bool bIsAborted)
{
	UNUSED_ALWAYS(dwAppData);
	if (bIsAborted) printf (CP_ParserMsgs[LOG_PARSER_END_PARSE]);
};
