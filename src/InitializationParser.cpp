/**	Declaration of all the parser methods to understand the initialization files
*	@author Ramon Molla, Carlos Martinez Perez
*	@version 2011-02
*/

#include <InitializationParser.h>
#include <string>
#include "Global.h"
#include <UGKString.h>

//Externals
extern char CHARS_Tags[CHARS_MAX_CHARTYPE][GCHAR_TAG_MAX_LONG ];

///Sensitive string tags
char CIP_Tags[MAXTAGS][CIP_TAG_MAX_LONG] = 
{
	UGKS_EMPTY_STRING,
	"ACCELERATION",
	"AI",
	"ANIMATION2D",
	"BODY",							///<HTML tag
	"CENTER",
	"CHARACTER",
	"DIR",
	"EPS",							///<Energy Per Shoot
	"EXPLOSION",
	"FILE",
	"FILE",
	"GEOMETRY",
	"HAVE",							///<Recursive definition of a character
	"HEAD",							///<HTML tag
	"HEALTH",						///<How strong is the character. How much has to be hurt before dieing
	"HTML",							///<HTML tag
	"LANGUAGE",
	"LINE",
	"LINE",
	"LINE",
	"LIVES",						///<Amount of ships the Player has still before finishing the game
	"MESH",
	"MESH",
	"MUSIC",
	"NAME",
	"NAME",
	"NAME",
	"NAME",
	"NAME",
	"PERIODE",
	"RADIO",
	"SCORE",
	"SCORES",
	"SHOOT",						///<Kind of shooting
	"SPEED",
	"SPS",							///<Shoots Per Second
	"SOUNDS",
	"TEXTURE",
	"TEXTURE2D",
	"TEXTURE3D",
	"TEXTURE2D",
	"TEXTU2D",
	"TEXTURE3D",
	"TITLE",						///<HTML tag
	"TYPE",
	"TYPE",
	"TYPE",
	"UNKNOWN",						///<This tag has no matching. It is for management purpouses only. 
	"VERSION",
	"VELOCITY",
	"X",							///<X position of the character described
	"Y",							///<Y position of the character described
	"Z",							///<Z position of the character described
	"X",							///<X speed of the character described
	"Y",							///<Y speed of the character described
	"Z"								///<Z speed of the character described
};

char  CIP_ParserMsgs[CIP_MAX_MSGS][CIP_MSG_MAX_LONG] = 
{
	"Minimum amount of lives reported (",
	"Maximum amount of lives reported (",
	") is lower than the minimum ",
	") is higher than the maximum ",
	"amount allowed by the program (1).",
	" Value cut to the ",
	"minimum.",
	"maximum.",
	"Initialization",
	"Speed coordinates not defined. Tag ",
	" erroneous."
};

char CIP_Ani2DTypes[MAX_NUM_ANI_TYPES][CIP_TAG_MAX_LONG] = 
{
	"ANITYPE_CICLIC",
	"ANITYPE_PINGPONG",
	"ANITYPE_SERIAL"
};
extern CSIGame Game;

/**
*	\defgroup	default	Default characters
*	 Used by the parsers while reading objects
*	 Default characters for initialization purpouses only
*    Updated when the initialization parser is running
*	 @{
*/
CBonus			defaultBonus;
CBunker			defaultBunker;
CBrick			defaultBrick;
CPlayer			defaultPlayer;
CShip			defaultShip;
CSupplyShip		defaultSShip;
CCircleShip		defaultCShip;
CAirplane		defaultAirplane;

GCHARS_CharacterType CharType;
bool MeshSemaphore[CHARS_MAX_CHARTYPE];

CInitializationReader::CInitializationReader()
{
	strcpy(Type, CIP_ParserMsgs[CIP_INITIALIZATION]);	//Type of parser
	ResetState();
	StateStack.push(NIL);
}

CInitializationReader::~CInitializationReader()
{
	if (NULL != LogFile)
		LogFile.close();
	ResetState();
}

/**   
   @fn Init
   Initialize
   @param char FileName : file name
   Number starts at 0 bunkers    
   Starts the default values ??at the beginning of each level in the game
*/
void CInitializationReader::Init(char *FileName)
{
	BunkersManager.BunkersAmount = 0; // Initialization of the number of the bunkers
	LogFile.open(FileName);
	if (LogFile)
	{
		ResetState();
		StateStack.push(NIL);
	}
	else exit(0);
	
};

void CInitializationReader::TagError(const char *Text)
{	char	msj[CP_MAX_MSJ_LENGTH];

	if (LogFile)
	{
		strcpy(msj, CP_ParserMsgs[LOG_PARSER_TAG_CHARACTER_TYPE]);
		strcat(msj, Text);
		strcat(msj, CP_ParserMsgs[LOG_PARSER_TAG_NOT_COMPATIBLE]);
		ErrorParser (msj);
	}
}

///Translates from a string to the token that the string represents
unsigned int CInitializationReader::String2Tag (const char *Name)
{
	switch (Name[0])
	{
	case 'A': if ('I' == Name[1])								return AI;
			  else if (strcmp(Name, CIP_Tags[ACCELERATION]))
					if (strcmp(Name, CIP_Tags[ANIMATION2D]))	return UNKNOWN;
					else										return ANIMATION2D;
				   else											return ACCELERATION;
		break;
	case 'B': if (0 == strcmp(Name, CIP_Tags[BODY]))			return BODY;
		break;
	case 'C': if (strcmp(Name, CIP_Tags[CHARACTER]))
				  if (strcmp(Name, CIP_Tags[CENTER]))			return UNKNOWN;
				else											return CENTER;
			else												return CHARACTER;	  
		break;
	case 'D': if (0 == strcmp(Name, CIP_Tags[DIR]))				return DIR;
		break;
	case 'E': if (strcmp(Name, CIP_Tags[EPS]))
				if (strcmp(Name, CIP_Tags[EXPLOSION]))			return UNKNOWN;
				else											return EXPLOSION;
			  else												return EPS;
		 break;
	case 'F': if (0 == strcmp(Name, CIP_Tags[FILE_TAG]))		return FILE_TAG;
		break;
	case 'G': if (0 == strcmp(Name, CIP_Tags[GEOMETRY]))		return GEOMETRY;
		break;
	case 'H': if ('E' == Name[1])
				  if (strcmp(Name, CIP_Tags[HEAD]))		
					  if (strcmp(Name, CIP_Tags[HEALTH]))		return UNKNOWN;
					   else										return HEALTH;
				  else											return HEAD;
			  else if (strcmp(Name, CIP_Tags[HAVE]))
						if (strcmp(Name, CIP_Tags[HTML]))		return UNKNOWN;
						else									return HTML;
				   else											return HAVE;
		break;
	case 'L': if  (strcmp(Name, CIP_Tags[LINE]))
				if  (strcmp(Name, CIP_Tags[LANGUAGE]))
					if (strcmp(Name, CIP_Tags[LIVES]))			return UNKNOWN;
					else										return LIVES;
				 else											return LANGUAGE;
			  else												return LINE;
		break;
	case 'M': if (strcmp(Name, CIP_Tags[MESH]))
				if (strcmp(Name, CIP_Tags[MUSIC]))				return UNKNOWN;
				else											return MUSIC;
			  else												return MESH;
		break;
	case 'N': if (0 == strcmp(Name, CIP_Tags[NAME]))			return NAME;
		break;
	case 'P': if (0 == strcmp(Name, CIP_Tags[PERIODE_ANI2D_CHAR]))	return PERIODE_ANI2D_CHAR;
		break;
	case 'R': 
				if(strcmp(Name, CIP_Tags[RADIO]))				return UNKNOWN;
				else											return RADIO;

		break;
	case 'S': if (strcmp(Name, CIP_Tags[SCORE]))
				if (strcmp(Name, CIP_Tags[SCORES]))
					if (strcmp(Name, CIP_Tags[SPEEDX]))
						if (strcmp(Name, CIP_Tags[SPEEDY]))
							if (strcmp(Name, CIP_Tags[SPEEDZ]))
								if (strcmp(Name, CIP_Tags[SHOOT]))
									if (strcmp(Name, CIP_Tags[SPEED]))
										if (strcmp(Name, CIP_Tags[SPS]))
											if (strcmp(Name, CIP_Tags[SOUNDS]))	return UNKNOWN;
											else								return SOUNDS;
										else								return SPS;
									else								return SPEED;
								else									return SHOOT;			  
							else									return SPEEDZ;
						else									return SPEEDY;
					else									return SPEEDX;
				else									return SCORES;
			  else										return SCORE;
		break;
	case 'T': 
				if (strcmp(Name, CIP_Tags[TEXTURE2D]))
					if (strcmp(Name, CIP_Tags[TEXTURE3D]))
					  if (strcmp(Name, CIP_Tags[TEXTURE]))
						if (strcmp(Name, CIP_Tags[TITLE]))
							if (strcmp(Name, CIP_Tags[TYPE]))	return UNKNOWN;
							else								return TYPE;
						else								return TITLE;
					  else								return TEXTURE;
					else							return TEXTURE3D;
				else							return TEXTURE2D;
		break;
	case 'V': if (strcmp(Name, CIP_Tags[VELOCITY]))	
				if (strcmp(Name, CIP_Tags[VERSION]))	return UNKNOWN;
				else									return VERSION;
			  else										return VELOCITY;
		break;
	case 'X': if (0 == strcmp(Name, CIP_Tags[X]))		return X;
		break;
	case 'Y': if (0 == strcmp(Name, CIP_Tags[Y]))		return Y;
		break;
	case 'Z': if (0 == strcmp(Name, CIP_Tags[Z]))		return Z;
		break;
	};
	return UNKNOWN;
};

void  CInitializationReader::BeginParse(DWORD dwAppData, bool &bAbort)
{
	UNUSED_ALWAYS(dwAppData);
	bAbort = false;
#ifdef CIP_DEBUG	//Class HTML Parser Debug activated
	LogFile << CP_ParserMsgs[LOG_PARSER_BEGIN_PARSE] << " . Initialization.\n";
#endif
	ResetState();			//Stack reset
	StateStack.push(NIL);

	for (unsigned int i = 0; i< CHARS_MAX_CHARTYPE;i++)
		MeshSemaphore[i] = true;
}

void CInitializationReader::StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
{
	CString TagString;
	unsigned int Tag;
	char msg[256];

	TagString = pTag->getTagName();

#ifdef CIP_DEBUG	//Class HTML Parser Debug activated
	LogFile << CP_ParserMsgs[LOG_PARSER_START_TAG] << TagString << UGKS_NEW_LINE_STRING;
#endif

	UNUSED_ALWAYS(dwAppData);
	bAbort = false;

	Tag = String2Tag(LPCTSTR(TagString));
	switch(StateStack.top())
	{case NIL:		//Starting and ending states
				if (HTML == Tag)
					//It is a HTML file. Change to HTML state
					StateStack.push(HTML);
				else ErrorParser(CP_ParserMsgs[LOG_PARSER_NOT_HTML_TAG]);
		break;
	case ANIMATION2D:
					switch (Tag)
					{
					case NAME:
						StateStack.push(NAME_ANI2D_CHAR);
						break;
					case PERIODE_ANI2D_CHAR:
						StateStack.push(PERIODE_ANI2D_CHAR);
						break;
					case TEXTURE2D:
						StateStack.push(TEXTU2D_ANI2D_CHAR);
						break;
					case TYPE:
						StateStack.push(TYPE_ANIMA2D_CHAR);
						break;
					}
		break;
	case BODY:		//HTML tag
					switch (Tag)
					{
					case DIR:
					case CHARACTER:
					case SCORES: StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					default:
						TagNotAllowed (TagString, CIP_Tags[BODY]);
					}
		break;
	case CHARACTER:
					switch (Tag)
					{
					case ANIMATION2D:
						StateStack.push(ANIMATION2D);
						break;
					case NAME:
						StateStack.push(NAME_CHARACTER); //Change to the state NAME used to identify a character
						break;
					case TYPE:	
						StateStack.push(TYPE_CHARACTER);
						break;
					case MESH:
						StateStack.push(MESH_CHARACTER);
						break;
					case TEXTURE2D:
						StateStack.push(TEXTURE2D_CHARACTER);
						break;
					case TEXTURE3D:
						StateStack.push(TEXTURE3D_CHARACTER);
						break;
					case EXPLOSION:
					case ACCELERATION:
					case LIVES:
					case HEALTH:
					case RADIO:
					case SPEED:
					case SHOOT:
					case VELOCITY:
						StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					default:
						TagNotAllowed(TagString, CIP_Tags[CHARACTER]);
					}
		break;
	case DIR:
					switch (Tag)
					{
					case AI:
					case MUSIC:
					case SOUNDS:
					case TEXTURE:
					case GEOMETRY:
					case LANGUAGE: StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					default:
						TagNotAllowed (TagString, CIP_Tags[DIR]);
					}
		break;
	case HEAD:		//HTML tag
					switch (Tag)
					{
					case TITLE:
					case VERSION:
					case TYPE:	StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					default:
						TagNotAllowed (TagString, CIP_Tags[HEAD]);
					}
		break;		
	case HTML:		//HTML Tag
					//The parser is at the very beginning of the iniitalization file
					switch (Tag)
					{
					case HEAD:		//A HEAD Tag is expected
					case BODY: 
						StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					default:
						TagNotAllowed (TagString, CIP_Tags[HTML]);
					}					
		break;
	case SCORES:	//A NAME Tag is expected
					if (NAME == Tag)
						//Change to NAME state
						StateStack.push(NAME_SCORE);

					//May be that no NAME tag appears or that the NAME section is finished.
					//There is a SCORE part also
					else if (SCORE == Tag)
						//Change to SCORE state
						StateStack.push(SCORE);
					else TagNotAllowed(TagString, CIP_Tags[SCORES]);
		break;
	case SHOOT:		//Kind of shooting
					switch (Tag)
					{
					case SPS:	//Change to SPS state
					case EPS:	//May be that no SPS tag appears or that the SPS section is finished.
								//There is a EPS part also
						StateStack.push(Tag); //Change to the state specified by the Tag
						break;
					default:
						TagNotAllowed(TagString, CIP_Tags[SHOOT]);
					}
		break;
	case SPEED:
					switch (Tag)
					{
					case X:
						StateStack.push(SPEEDX); //Change to the state specified by the Tag
						break;
					case Y:
						StateStack.push(SPEEDY); //Change to the state specified by the Tag
						break;
					case Z:
						StateStack.push(SPEEDZ); //Change to the state specified by the Tag
						break;
					default: 
						strcpy (msg, CIP_Tags[CIP_MAX_LIVES_NOT_DEF]);
						strcat (msg, TagString);
						strcat (msg, CIP_Tags[CIP_ERRONEUS]);
						ErrorParser( msg);
					}
		break;
	case TEXTURE:
					if (NAME == Tag)
						//Change to NAME state
						StateStack.push(NAME_TEXTURE);

					//May be that no NAME tag appears or that the NAME section is finished.
					//There is a FILE part also
					else if (FILE_TAG == Tag)
						//Change to FILE state
						StateStack.push(FILE_TEXTURE);
					else TagNotAllowed(TagString, CIP_Tags[TEXTURE]);
		break;
	default:;
	};
}

void CInitializationReader::EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
{
	CString TagString;
	unsigned int Tag, StateStackTop;
	bool Error = false;
	char msj[128];

	TagString = pTag->getTagName();

#ifdef CIP_DEBUG	//Class HTML Parser Debug activated
	LogFile << CP_ParserMsgs[LOG_PARSER_END_TAG_END] << pTag->getTagName() << std::endl;
#endif

	UNUSED_ALWAYS(dwAppData);
	bAbort = false;

	Tag = String2Tag(LPCTSTR(TagString));
	StateStackTop = StateStack.top();
	switch(StateStackTop)
	{case ACCELERATION:
	 case AI:
	 case BODY:
	 case CHARACTER:
	 case DIR:
	 case EPS:		//Energy Per Shoot
	 case EXPLOSION:
	 case GEOMETRY:
	 case HAVE:		//Recursive definition of a character
	 case HEAD:		
	 case HEALTH:	//How strong is the character. How much has to be hurt before dieing
	 case HTML:		//The parser is at the very end of the initialization file					
	 case LANGUAGE:
	 case LIVES:	
	 case MESH:
	 case MUSIC:
 	 case PERIODE_ANI2D_CHAR:
	 case SOUNDS:
	 case RADIO:
	 case SCORE:
	 case SCORES:	
	 case SHOOT:		
	 case SPEED:
	 case SPS:		//Shoots Per Second
	 case TEXTURE:
	 case TITLE:		
	 case TYPE:
	 case VERSION:
	 case VELOCITY:
			if (StateStackTop == Tag) StateStack.pop();	//Change to the previous state
			else Error = true;
		break;
	 case ANIMATION2D:
			if (StateStackTop == Tag) 
			{
				StateStack.pop();	//Change to the previous state
				switch (CharType)
				{
				case CHARS_AIRPLANE:
					defaultAirplane.IndAnimation2D = AnimationsManager.Animations.size()-1;
					break;
				case CHARS_BONUS:
						break;
				case CHARS_BRICK:
						break;
				case CHARS_BUNKER:
						break;
				case CHARS_PLAYER:
					defaultPlayer.IndAnimation2D = AnimationsManager.Animations.size()-1;
						break;
				case CHARS_SHIP:
					defaultShip.IndAnimation2D = AnimationsManager.Animations.size()-1;
						break;
				case CHARS_SUPPLYSHIP:
					defaultSShip.IndAnimation2D = AnimationsManager.Animations.size()-1;
						break;
				case CHARS_CIRCLESHIP:
					defaultCShip.IndAnimation2D = AnimationsManager.Animations.size()-1;
					break;
				case CHARS_LASER:
					for(int i=0;i<CP_MAX_LASERS;i++)
						defaultPlayer.Laser[i].IndAnimation2D = AnimationsManager.Animations.size()-1;
						break;
				case CHARS_SPHERE_OGIVE:
					Background.IndAnimation2D = AnimationsManager.Animations.size()-1;
					break;
				}
			}
			else Error = true;
		 break;
	 case FILE_TEXTURE:
			if (FILE_TAG == Tag) StateStack.pop();	//Change to the previous state
			else Error = true;
		break;
	 case NAME_CHARACTER:
	 case NAME_SCORE:
	 case NAME_TEXTURE:
	 case NAME_ANI2D_CHAR:
			if (NAME == Tag) StateStack.pop();	//Change to the previous state
			else Error = true;
		break;
	 case MESH_CHARACTER:
			if (MESH == Tag) StateStack.pop();	//Change to the previous state
			else Error = true;
		 break;
	 case SPEEDX:
			if (X == Tag) StateStack.pop();	//Change to the previous state
						else Error = true;
		 break;
	 case SPEEDY:
			if (Y == Tag) StateStack.pop();	//Change to the previous state
						else Error = true;
		 break;
	 case SPEEDZ:
			if (Z == Tag) StateStack.pop();	//Change to the previous state
						else Error = true;
		 break;
	 case TEXTURE2D_CHARACTER:
	 case TEXTU2D_ANI2D_CHAR:
			if (TEXTURE2D == Tag) StateStack.pop();	//Change to the previous state
					else Error = true;
		 break;
	 case TEXTURE3D_CHARACTER:
			if (TEXTURE3D == Tag) StateStack.pop();	//Change to the previous state
					else Error = true;
		 break;
	 case TYPE_ANIMA2D_CHAR:
			if (TYPE == Tag) StateStack.pop();	//Change to the previous state
					else Error = true;
		 break;
	 case UNKNOWN:	//This tag is for management purpouses only. There is no correspondence to any string label in the HTML file
			strcpy_s(msj, TagString);
			strcat_s(msj, CP_ParserMsgs[LOG_PARSER_END_TAG_UNK]);
			ErrorParser(msj);
		break;
	 case NIL:
	default:;
	};
	if (Error)
	{	strcpy_s(msj, TagString);
		strcat_s(msj, CP_ParserMsgs[LOG_PARSER_END_TAG_ERROR2]);
		if (StateStack.size()>=2)
		{
			StateStack.pop();
			strcat_s(msj, CIP_Tags[StateStack.top()]);
			StateStack.push(StateStackTop);
		}
		else strcat_s(msj, CIP_Tags[StateStackTop]);
		strcat_s(msj, CP_ParserMsgs[LOG_PARSER_END_TAG_ERROR2]);
		ErrorParser(msj);
	}
}

void CInitializationReader::Characters(const CString &rText, DWORD dwAppData, bool &bAbort)
{
char			msj[128];
double			ms;
unsigned int	i;


#ifdef CIP_DEBUG	//Class HTML Parser Debug activated
LogFile << CP_ParserMsgs[LOG_PARSER_TEXT] << rText << std::endl;
#endif

	UNUSED_ALWAYS(dwAppData);
	bAbort = false;

	switch(StateStack.top())
	{
	
	case ACCELERATION:
		switch (CharType)
		{
		case CHARS_PLAYER:
			defaultPlayer.Acceleration.v[XDIM] = atof (rText);
			break;
		case CHARS_SHIP:
			defaultShip.Acceleration.v[XDIM] = atof (rText);
			break;
		}
		break;
	case AI:
		strcpy(Game.Directories[CG_AI_DIR], rText);
		break;
	case EPS:		//Energy Per Shoot
		break;
	case EXPLOSION:
			switch (CharType)
			{
			case CHARS_AIRPLANE:
				defaultAirplane.Hit_duration	= atof (rText);
				defaultAirplane.Explosion.SubType = CE_SUPPLYSHIP_EXPLOSION;
				break;
			case CHARS_BONUS:
				 defaultBonus.Hit_duration	= atof (rText);
				break;
			case CHARS_BRICK:
				 defaultBrick.Hit_duration	= atof (rText);
				 break;
			case CHARS_BUNKER:
				 defaultBunker.Hit_duration	= atof (rText);
				 break;
			case CHARS_CIRCLESHIP:
 				 defaultCShip.Hit_duration	= atof (rText);
				 defaultCShip.Explosion.SubType = CE_SUPPLYSHIP_EXPLOSION;
				 break;
			case CHARS_LASER:
				defaultPlayer.Laser[CP_LEFT_LASER].Hit_duration	= atof (rText);
				defaultPlayer.Laser[CP_LEFT_LASER].Explosion.SubType = CE_LASER_EXPLOSION;
				defaultPlayer.Laser[CP_RIGHT_LASER].Hit_duration	= atof (rText);
				defaultPlayer.Laser[CP_RIGHT_LASER].Explosion.SubType = CE_LASER_EXPLOSION;
				 break;
			 case CHARS_PLAYER:
				 defaultPlayer.Hit_duration	= atof (rText);
				 defaultPlayer.Explosion.SubType = CE_PLAYER_EXPLOSION;
				 break;
			case CHARS_SHIP:
				 defaultShip.Hit_duration	= atof (rText);
				 defaultShip.Explosion.SubType = CE_SHIP_EXPLOSION;
				 break;
			case CHARS_SUPPLYSHIP:
 				 defaultSShip.Hit_duration	= atof (rText);
				 defaultSShip.Explosion.SubType = CE_SUPPLYSHIP_EXPLOSION;
				 break;
			}
		break;
	case FILE_TEXTURE:
			///Save the texture folder on a game singleton attribute 
			strcpy(Game.Directories[CG_TEXTURE_DIR], rText);
		break;
	case GEOMETRY:
			strcpy(msj, rText);
			MeshesManager.SetFilesPath(msj);
			strcpy(Game.Directories[CG_GEOMETRY_DIR], rText);
		break;
	case HEALTH:	//How strong is the character. How much has to be hurt before dieing
			switch (CharType)
			{
			case CHARS_AIRPLANE:
				defaultAirplane.Health	= atof (rText);
				break;
			case CHARS_BONUS:
				 defaultBonus.Health	= atof (rText);
				break;
			case CHARS_BRICK:
				 defaultBrick.Health	= atof (rText);
				 break;
			case CHARS_BUNKER:
				 defaultBunker.Health	= atof (rText);
				 break;
			case CHARS_CIRCLESHIP:
 				 defaultCShip.Health	= atof (rText);
				 break;
			case CHARS_LASER:
				defaultPlayer.Laser[CP_LEFT_LASER].Health	= atof (rText);
				defaultPlayer.Laser[CP_RIGHT_LASER].Health	= atof (rText);
				 break;
			 case CHARS_PLAYER:
				 defaultPlayer.Health	= atof (rText);
				 break;
			case CHARS_SHIP:
				 defaultShip.Health	= atof (rText);
				 break;
			 case CHARS_SUPPLYSHIP:
 				 defaultSShip.Health	= atof (rText);
				 break;
			}
		break;
	case LANGUAGE:
					strcpy(Application.LanguageFolder, rText);
		break;
	case LIVES:		//Amount of ships the Player has still before finishing the game
			if(CP_INFINITE_LIVES == atoi(rText))
				defaultPlayer.Lives = CP_INFINITE_LIVES;
			else if (0 > atoi(rText))
				{
					strcpy(msj, CIP_ParserMsgs[CIP_MIN_LIVES_NOT_DEF]);
					strcat(msj, rText);
					strcat(msj, CIP_ParserMsgs[CIP_LOWER_THAN_MIN]);
					strcat(msj, CIP_ParserMsgs[CIP_AMOUNT_ALLOWED]);
					strcat(msj, CIP_ParserMsgs[CIP_VALUE_CUT]);
					strcat(msj, CIP_ParserMsgs[CIP_MINIMUM]);
					ErrorParser (msj);
					defaultPlayer.Lives = 1;
				}
				else if (SHRT_MAX > atoi(rText)) defaultPlayer.Lives = atoi(rText);
					 else	
					 {
						strcpy(msj, CIP_ParserMsgs[CIP_MAX_LIVES_NOT_DEF]);
						strcat(msj, rText);
						strcat(msj, CIP_ParserMsgs[CIP_LOWER_THAN_MIN]);
						strcat(msj, CIP_ParserMsgs[CIP_AMOUNT_ALLOWED]);
						strcat(msj, CIP_ParserMsgs[CIP_VALUE_CUT]);
						strcat(msj, CIP_ParserMsgs[CIP_MAXIMUM]);
						ErrorParser (msj);
						defaultPlayer.Lives = SHRT_MAX;
					}
		break;
	case MESH_CHARACTER:
			switch (CharType)
			{
			case CHARS_AIRPLANE:
				if (MeshSemaphore[CHARS_AIRPLANE])
				{
					MeshSemaphore[CHARS_AIRPLANE] = false;
					strcpy(msj, rText);
					MeshesManager.AddModel(msj);
					defaultAirplane.IndMesh = MeshesManager.Meshes.size()-1;
					defaultAirplane.Mesh = MeshesManager.Meshes[defaultAirplane.IndMesh];
					strcpy(defaultAirplane.MeshName, defaultAirplane.Mesh->GetFileName());
					}
				break;
			case CHARS_BONUS:
				 break;
			case CHARS_BRICK:
				 break;
			case CHARS_BUNKER:
				 break;
			case CHARS_PLAYER:
				if (MeshSemaphore[CHARS_PLAYER])
				{
					MeshSemaphore[CHARS_PLAYER] = false;
					strcpy(msj, rText);
					MeshesManager.AddModel(msj);
					defaultPlayer.IndMesh = MeshesManager.Meshes.size()-1;
					defaultPlayer.Mesh = MeshesManager.Meshes[defaultPlayer.IndMesh];
					strcpy(defaultPlayer.MeshName, defaultPlayer.Mesh->GetFileName());
				}
				 break;
			case CHARS_SHIP:
				if (MeshSemaphore[CHARS_SHIP])
				{
					MeshSemaphore[CHARS_SHIP] = false;
					strcpy(msj, rText);
					MeshesManager.AddModel(msj);
					defaultShip.IndMesh = MeshesManager.Meshes.size()-1;
					defaultShip.Mesh = MeshesManager.Meshes[defaultShip.IndMesh];
					strcpy(defaultShip.MeshName, defaultShip.Mesh->GetFileName());
				}
				 break;
			case CHARS_SUPPLYSHIP:
				if (MeshSemaphore[CHARS_SUPPLYSHIP])
				{
					MeshSemaphore[CHARS_SUPPLYSHIP] = false;
					strcpy(msj, rText);
					MeshesManager.AddModel(msj);
					defaultSShip.IndMesh = MeshesManager.Meshes.size()-1;
					defaultSShip.Mesh = MeshesManager.Meshes[defaultSShip.IndMesh];
					strcpy(defaultSShip.MeshName, defaultSShip.Mesh->GetFileName());
				}
				 break;
			case CHARS_CIRCLESHIP:
				if (MeshSemaphore[CHARS_CIRCLESHIP])
				{
					MeshSemaphore[CHARS_CIRCLESHIP] = false;
					strcpy(msj, rText);
					MeshesManager.AddModel(msj);
					defaultCShip.IndMesh = MeshesManager.Meshes.size()-1;
					defaultCShip.Mesh = MeshesManager.Meshes[defaultCShip.IndMesh];
					strcpy(defaultCShip.MeshName, defaultCShip.Mesh->GetFileName());
					}
				break;
			case CHARS_LASER:
				if (MeshSemaphore[CHARS_LASER])
				{
					MeshSemaphore[CHARS_LASER] = false;
					strcpy(msj, rText);
					MeshesManager.AddModel(msj);
					for(i=0;i<CP_MAX_LASERS;i++)
					{
						defaultPlayer.Laser[i].IndMesh = MeshesManager.Meshes.size()-1;
						defaultPlayer.Laser[i].Mesh = MeshesManager.Meshes[defaultPlayer.Laser[i].IndMesh];
						strcpy(defaultPlayer.Laser[i].MeshName, defaultPlayer.Laser[i].Mesh->GetFileName());
					}
				}
				 break;
			case CHARS_SPHERE_OGIVE:
				if (MeshSemaphore[CHARS_SPHERE_OGIVE])
				{
					MeshSemaphore[CHARS_SPHERE_OGIVE] = false;
					strcpy(msj, rText);
					MeshesManager.AddModel(msj);
					Background.IndMesh = MeshesManager.Meshes.size()-1;
					Background.Mesh = MeshesManager.Meshes[Background.IndMesh];
					strcpy(Background.MeshName, Background.Mesh->GetFileName());
				}
				break;
			}
		break;
	case MUSIC:
			SoundsManager.SetMusicPath((char*) rText.GetString());
			strcpy(Game.Directories[CG_MUSIC_DIR], rText);
		break;
	case NAME_CHARACTER:
			CharType = (GCHARS_CharacterType) CharacterFactory.String2Tag(rText);
			switch (CharType)
			{
				case CHARS_AIRPLANE:
				//Reset all the values by default in case the file does not cover every object aspect
				defaultAirplane.Init();
				break;
			case CHARS_BONUS:
				//Reset all the values by default in case the file does not cover every object aspect
				defaultBonus.Init();
				break;
			case CHARS_BRICK:
				//Reset all the values by default in case the file does not cover every object aspect
				defaultBrick.Init();
				break;
			case CHARS_BUNKER:
				//Reset all the values by default in case the file does not cover every object aspect
				defaultBunker.Init();
				break;
			case CHARS_PLAYER:
				//Reset all the values by default in case the file does not cover every object aspect
				defaultPlayer.Init();
				break;
			case CHARS_SHIP:
				//Reset all the values by default in case the file does not cover every object aspect
				defaultShip.Init();
				break;
			case CHARS_SUPPLYSHIP:
				//Reset all the values by default in case the file does not cover every object aspect
				defaultSShip.Init();
				break;
			case CHARS_LASER:
				//Reset all the values by default in case the file does not cover every object aspect
				for(i=0;i<CP_MAX_LASERS;i++){
					defaultPlayer.Laser[i].Init();
				}
				break;
			case CHARS_CIRCLESHIP:
				//Reset all the values by default in case the file does not cover every object aspect
				defaultCShip.Init();
				break;
			case CHARS_SPHERE_OGIVE:
				Background.Init();
				break;
			case CHARS_CHARACTER_UNKNOWN:
				TagError(rText);
				break;
			};
		break;
	case NAME_SCORE:
		break;
	case NAME_TEXTURE:
		break;
	case NAME_ANI2D_CHAR:
			switch (CharType)
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
					AnimationsManager.CreateAnimation(msj,(double) 0,(ANI_ANIMATION_TYPE)0);
				break;
			}
		break;
	case PERIODE_ANI2D_CHAR:
			switch (CharType)
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
					ms = atof (rText);
					AnimationsManager.Animations[AnimationsManager.Animations.size()-1]->SetPeriod(ms);
				break;
			}
		break;
	case RADIO:
		switch (CharType)
		{
		case CHARS_AIRPLANE:
			//defaultAirplane.Radio = atof(rText);
			break;
		case CHARS_CIRCLESHIP:
			defaultCShip.Radio = atof(rText);
			break;
		
		}
		break;
	case SCORE:
		break;
	case SOUNDS:
		SoundsManager.SetTrackPath((char*) rText.GetString());
		strcpy(Game.Directories[CG_SOUNDS_DIR], rText.GetString());
		break;
	case SPEEDX:
		switch (CharType)
		{
		case CHARS_AIRPLANE:
			defaultAirplane.Speed.v[XDIM] = atof (rText);
			 break;
		case CHARS_BONUS:
			defaultBonus.Speed.v[XDIM] = atof (rText);
			break;
		case CHARS_BRICK:
			defaultBrick.Speed.v[XDIM] = atof (rText);
			break;
		case CHARS_BUNKER:
			defaultBunker.Speed.v[XDIM] = atof (rText);
			break;
		case CHARS_PLAYER:
			defaultPlayer.Speed.v[XDIM] = atof (rText);
			break;
		case CHARS_SHIP:
			defaultShip.Speed.v[XDIM] = atof (rText);
			break;
		case CHARS_SUPPLYSHIP:
			defaultSShip.Speed.v[XDIM] = atof (rText);
			 break;
		case CHARS_CIRCLESHIP:
			defaultCShip.Speed.v[XDIM] = atof (rText);
			 break;
		 case CHARS_WEAPON:
			break;
		}
		break;
	case SPEEDY:
		switch (CharType)
		{
		case CHARS_AIRPLANE:
			defaultAirplane.Speed.v[YDIM] = atof (rText);
			 break;
		case CHARS_BONUS:
			defaultBonus.Speed.v[YDIM] = atof (rText);
			break;
		case CHARS_BRICK:
			defaultBrick.Speed.v[YDIM] = atof (rText);
			break;
		case CHARS_BUNKER:
			defaultBunker.Speed.v[YDIM] = atof (rText);
			break;
		case CHARS_PLAYER:
			defaultPlayer.Speed.v[YDIM] = atof (rText);
			break;
		case CHARS_SHIP:
			defaultShip.Speed.v[YDIM] = atof (rText);
			break;
		case CHARS_SUPPLYSHIP:
			defaultSShip.Speed.v[YDIM] = atof (rText);
			 break;
		case CHARS_CIRCLESHIP:
			defaultCShip.Speed.v[YDIM] = atof (rText);
			 break;
		 case CHARS_WEAPON:
			break;
		}break;
	case SPEEDZ:
		switch (CharType)
		{
		case CHARS_AIRPLANE:
			defaultAirplane.Speed.v[ZDIM] = atof (rText);
			 break;
		case CHARS_BONUS:
			defaultBonus.Speed.v[ZDIM] = atof (rText);
			break;
		case CHARS_BRICK:
			defaultBrick.Speed.v[ZDIM] = atof (rText);
			break;
		case CHARS_BUNKER:
			defaultBunker.Speed.v[ZDIM] = atof (rText);
			break;
		case CHARS_PLAYER:
			defaultPlayer.Speed.v[ZDIM] = atof (rText);
			break;
		case CHARS_SHIP:
			defaultShip.Speed.v[ZDIM] = atof (rText);
			break;
		case CHARS_SUPPLYSHIP:
			defaultSShip.Speed.v[ZDIM] = atof (rText);
			 break;
		case CHARS_CIRCLESHIP:
			defaultCShip.Speed.v[ZDIM] = atof (rText);
			 break;
		 case CHARS_WEAPON:
			break;
		}
		break;
	case SPS:		//Shoots Per Second
		break;
	case TEXTURE:
			strcpy(msj, rText);
			TexturesManager.SetFilesPath(msj);
			strcpy(Game.Directories[CG_TEXTURE_DIR], rText);

			//Load basic application textures.
			LoadTextures();
		break;
	case TEXTURE2D_CHARACTER:
		switch (CharType)
			{
			case CHARS_AIRPLANE:
				strcpy(msj, rText);
				TexturesManager.CreateTexture(msj);
				defaultAirplane.IndTexture2D = TexturesManager.Textures.size()-1;
				 break;
			case CHARS_BONUS:
				 break;
			case CHARS_BRICK:
				 break;
			case CHARS_BUNKER:
				 break;
			case CHARS_PLAYER:
				strcpy(msj, rText);
				for(i=0;i<CP_MAX_PLAYERS;i++){
					TexturesManager.CreateTexture(msj);
					defaultPlayer.IndTexture2D = TexturesManager.Textures.size()-1;
				}
				 break;
			case CHARS_SHIP:
				strcpy(msj, rText);
				TexturesManager.CreateTexture(msj);
				defaultShip.IndTexture2D = TexturesManager.Textures.size()-1;
				 break;
			case CHARS_SUPPLYSHIP:
				strcpy(msj, rText);
				TexturesManager.CreateTexture(msj);
				defaultSShip.IndTexture2D = TexturesManager.Textures.size()-1;
				 break;
			case CHARS_CIRCLESHIP:
				strcpy(msj, rText);
				TexturesManager.CreateTexture(msj);
				defaultCShip.IndTexture2D = TexturesManager.Textures.size()-1;
				 break;
			case CHARS_SPHERE_OGIVE:
				
				break;
			}
		break;
	case TEXTU2D_ANI2D_CHAR:
			switch (CharType)
			{
			case CHARS_BONUS:
				 break;
			case CHARS_BRICK:
				 break;
			case CHARS_BUNKER:
				 break;
			case CHARS_PLAYER:
				strcpy(msj, rText);
				TexturesManager.CreateTexture(msj);
				AnimationsManager.Animations.back()->AddPhotogram(TexturesManager.Textures.back());
				 break;
			case CHARS_SHIP:
				strcpy(msj, rText);
				TexturesManager.CreateTexture(msj);
				AnimationsManager.Animations.back()->AddPhotogram(TexturesManager.Textures.back());
				 break;
			case CHARS_SUPPLYSHIP:
				strcpy(msj, rText);
				TexturesManager.CreateTexture(msj);
				AnimationsManager.Animations.back()->AddPhotogram(TexturesManager.Textures.back());
				 break;
			case CHARS_CIRCLESHIP:
				strcpy(msj, rText);
				TexturesManager.CreateTexture(msj);
				AnimationsManager.Animations.back()->AddPhotogram(TexturesManager.Textures.back());
				 break;
			case CHARS_SPHERE_OGIVE:			
				break;
			}
		break;
	case TEXTURE3D_CHARACTER:
		switch (CharType)
			{
			case CHARS_AIRPLANE:
				strcpy(msj, rText);
				TexturesManager.CreateTexture(msj);
				defaultAirplane.IndTexture3D = TexturesManager.Textures.size()-1;
				 break;
			case CHARS_BONUS:
				 break;
			case CHARS_BRICK:
				 break;
			case CHARS_BUNKER:
				 break;
			case CHARS_PLAYER:
				strcpy(msj, rText);
				TexturesManager.CreateTexture(msj);
				defaultPlayer.IndTexture3D = TexturesManager.Textures.size()-1;
				 break;
			case CHARS_SHIP:
				strcpy(msj, rText);
				TexturesManager.CreateTexture(msj);
				defaultShip.IndTexture3D = TexturesManager.Textures.size()-1;
				 break;
			case CHARS_SUPPLYSHIP:
				strcpy(msj, rText);
				TexturesManager.CreateTexture(msj);
				defaultSShip.IndTexture3D = TexturesManager.Textures.size()-1;
				 break;
			case CHARS_CIRCLESHIP:
				strcpy(msj, rText);
				TexturesManager.CreateTexture(msj);
				defaultCShip.IndTexture3D = TexturesManager.Textures.size()-1;
				 break;
			case CHARS_SPHERE_OGIVE:
				strcpy(msj, rText);
				TexturesManager.CreateTexture(msj);
				Background.IndTexture3D = TexturesManager.Textures.size()-1;
				break;
			}
		break;
	case TITLE:		//HTML tag
			if (strcmp(rText, Game.GetTitle()))
			{ErrorParser (CP_ParserMsgs[LOG_PARSER_NOT_THIS_VG]);
				bAbort = true;}
			else ;	//File content corresponds to the expected one for this videogame.
					//Go on. Nothing is done
		break;
	case TYPE:		
			if (strcmp(rText, CIP_ParserMsgs[CIP_INITIALIZATION]))
			{
				FileTypeMismatch(rText, CIP_ParserMsgs[CIP_INITIALIZATION]);
				bAbort = true;
			}
		break;
	case TYPE_CHARACTER:
		break;
	case TYPE_ANIMA2D_CHAR:
			switch (CharType)
			{
			case CHARS_AIRPLANE:
			case CHARS_BONUS:
			case CHARS_BRICK:
			case CHARS_BUNKER:
			case CHARS_PLAYER:
			case CHARS_SHIP:
			case CHARS_SUPPLYSHIP:
			case CHARS_CIRCLESHIP:
			case CHARS_LASER:
			case CHARS_SPHERE_OGIVE:
					if (0 == strcmp(rText, CIP_Ani2DTypes[ANITYPE_CICLIC]))
						AnimationsManager.Animations[AnimationsManager.Animations.size()-1]->Type = ANITYPE_CICLIC;
					else if (0 == strcmp(rText, CIP_Ani2DTypes[ANITYPE_PINGPONG]))
						 AnimationsManager.Animations[AnimationsManager.Animations.size()-1]->Type = ANITYPE_PINGPONG;
					else if (0 == strcmp(rText, CIP_Ani2DTypes[ANITYPE_SERIAL]))
						 AnimationsManager.Animations[AnimationsManager.Animations.size()-1]->Type = ANITYPE_SERIAL;
				break;
			}
		break;
	case VELOCITY:
		defaultCShip.Velocity = atof(rText);
		//defaultAirplane.Velocity = atof(rText);
		break;
	case VERSION:	
			if (strcmp(rText, Game.GetVersion()))
			{
				VersionMismatch (rText);
				bAbort = true;
			}
			else ;	//File version corresponds to the one from this videogame.
					//Go on. Nothing is done
		break;
	default:;		//Tags not supported are not managed, even if they have content associated
	};
}

void CInitializationReader::Comment(const CString &rComment, DWORD dwAppData, bool &bAbort)
{
#ifdef CIP_DEBUG	//Class HTML Parser Debug activated
	LogFile << "Comments: " << rComment << std::endl;
#endif

	UNUSED_ALWAYS(dwAppData);
	bAbort = false;
}

void CInitializationReader::EndParse(DWORD dwAppData, bool bIsAborted)
{
	UNUSED_ALWAYS(dwAppData);
#ifdef CIP_DEBUG	//Class HTML Parser Debug activated
	if (bIsAborted) LogFile << CP_ParserMsgs[LOG_PARSER_END_PARSE] << UGKS_NEW_LINE_STRING;
#endif
}