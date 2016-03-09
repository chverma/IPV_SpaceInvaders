/*LEVEL READER
	Author: Ramón Mollá
	Last update: 2007-09-11
*/

#include "LevelParser.h"

//Sensitive string tags
char CLeP_Tags[MAXTAGS][CLeP_TAG_MAX_LONG] = 
{
	"",
	"BODY",		//HTML tag
	"CHARACTER",
	"DIR",
	"EPS",		//Energy Per Shoot
	"EXPLOSION",
	"FILE",
	"GEOMETRY",
	"HAVE",		//Recursive definition of a character
	"HEAD",		//HTML tag
	"HEALTH",		//How strong is the character. How much has to be hurt before dieing
	"HTML",		//HTML tag
	"LIVES",		//Amount of ships the player has still before finishing the game
	"MAXBRICKS",
	"MAXPLAYERS",
	"MAXSHIPS",
	"MAXSUPPLYSHIPS",
	"MESH",
	"MUSIC",
	"NAME",
	"NAME",
	"NAME",
	"NAME",
	"SCORES",
	"SCORE",
	"SHOOT",		//Kind of shooting
	"SPEED",
	"SPS",		//Shoots Per Second
	"TEXTURE",
	"TITLE",		//HTML tag
	"TYPE",
	"UNKNOWN",	//This tag has no matching. It is for management purpouses only. 
	"VERSION"
};

//Translates from a string to the token that the string represents
CLeP_TagType CLevelReader::String2Tag (const char *Name)
{
switch (Name[0])
	{
	case 'B': if (0 == strcmp(Name, CLeP_Tags[BODY]))			return BODY;
		break;
	case 'C': if (0 == strcmp(Name, CLeP_Tags[CHARACTER]))		return CHARACTER;	  
		break;
	case 'D': if (0 == strcmp(Name, CLeP_Tags[DIR]))				return DIR;
		break;
	case 'E': if (strcmp(Name, CLeP_Tags[EPS]))
				if (strcmp(Name, CLeP_Tags[EXPLOSION]))			return UNKNOWN;
				else											return EXPLOSION;
			  else												return EPS;
		 break;
	case 'F': if (0 == strcmp(Name, CLeP_Tags[FILE_TAG]))		return FILE_TAG;
		break;
	case 'G': if (0 == strcmp(Name, CLeP_Tags[GEOMETRY]))		return GEOMETRY;
		break;
	case 'H': if ('E' == Name[1])
				  if (strcmp(Name, CLeP_Tags[HEAD]))		
					  if (strcmp(Name, CLeP_Tags[HEALTH]))		return UNKNOWN;
					   else										return HEALTH;
				  else											return HEAD;
			  else if (strcmp(Name, CLeP_Tags[HAVE]))
						if (strcmp(Name, CLeP_Tags[HTML]))		return UNKNOWN;
						else									return HTML;
				   else											return HAVE;
		break;
	case 'L': if (0 == strcmp(Name, CLeP_Tags[LIVES]))			return LIVES;
		break;
	case 'M': if ('A' == Name[1])
				  //Serie de MAX*
				  if ('S' == Name[3])
					   if (strcmp(Name, CLeP_Tags[MAXSHIPS]))
							if (strcmp(Name, CLeP_Tags[MAXSUPPLYSHIPS]))	return UNKNOWN;
					        else										return MAXSUPPLYSHIPS;
					   else												return MAXSHIPS;
				  else if (strcmp(Name, CLeP_Tags[MAXBRICKS]))
							if (strcmp(Name, CLeP_Tags[MAXPLAYERS]))		return UNKNOWN;
					        else										return MAXPLAYERS;
					   else												return MAXBRICKS;
			  else if (strcmp(Name, CLeP_Tags[MESH]))
					  if (strcmp(Name, CLeP_Tags[MUSIC]))				return UNKNOWN;
					  else												return MUSIC;
				   else													return MESH;
		break;
	case 'N': if (0 == strcmp(Name, CLeP_Tags[NAME]))	return NAME;
		break;
	case 'S': if (strcmp(Name, CLeP_Tags[SCORE]))
				if (strcmp(Name, CLeP_Tags[SCORES]))
				  if (strcmp(Name, CLeP_Tags[SHOOT]))
					if (strcmp(Name, CLeP_Tags[SPEED]))
					  if (strcmp(Name, CLeP_Tags[SPS]))	return UNKNOWN;
					  else								return SPS;
					else								return SPEED;
				  else									return SHOOT;			  
				else									return SCORES;
			  else										return SCORE;
		break;
	case 'T': if  (strcmp(Name, CLeP_Tags[TEXTURE]))
				if (strcmp(Name, CLeP_Tags[TITLE]))
					if (strcmp(Name, CLeP_Tags[TYPE]))	return UNKNOWN;
					else								return TYPE;
				else									return TITLE;
			  else										return TEXTURE;
		break;
	case 'V': if (0 == strcmp(Name, CLeP_Tags[VERSION]))	return VERSION;
		break;
	};
	return UNKNOWN;
};


/* Class that loads the values by default in the very beginning of every level in the game

   LEVEL INITIALIZATION
*/

void CLevelReader::BeginParse(DWORD dwAppData, bool &bAbort)
{
	UNUSED_ALWAYS(dwAppData);
	bAbort = false;
	printf("Comienza la interpretación del fichero de inicialización.\n");
}

void CLevelReader::StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
{
	printf("Etiqueta comienzo: %s\n", pTag->getTagName());
	UNUSED_ALWAYS(dwAppData);
	bAbort = false;

}

void CLevelReader::EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
{
	printf("Etiqueta fin: %s\n", pTag->getTagName());
	UNUSED_ALWAYS(dwAppData);
	bAbort = false;

}

void CLevelReader::Characters(const CString &rText, DWORD dwAppData, bool &bAbort)
{
	printf("Texto: %s\n", rText);
	UNUSED_ALWAYS(dwAppData);
	bAbort = false;
}

void CLevelReader::Comment(const CString &rComment, DWORD dwAppData, bool &bAbort)
{
	printf("Comentarios: %s\n", rComment);
	UNUSED_ALWAYS(dwAppData);
	bAbort = false;
}

void CLevelReader::EndParse(DWORD dwAppData, bool bIsAborted)
{
	UNUSED_ALWAYS(dwAppData);
	if (bIsAborted) printf ("Se ha acabado la interpretación del fichero.\n");
};
