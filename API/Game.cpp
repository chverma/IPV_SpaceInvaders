/**	Declaration of all the general game methods
*	@author Ramon Molla
*	@version 2012-10
*/

#include "Game.h"
#include <UGKString.h>

void CGame::Reset()
{
	if (Version)
		strcpy(Version, UGKS_EMPTY_STRING);
	if (Title)
		strcpy(Title, UGKS_EMPTY_STRING);

	RenderMode = CHAR_NO_RENDER;

	Score = HiScore = Level = MaxLevels = MaxPlayers = 0;

	for (unsigned int i = CG_AI_DIR;i<CG_MAX_DIR;i++) strcpy(Directories[i],UGKS_EMPTY_STRING);
}

CGame::~CGame()
{
}

/**   
   @fn SetVersion
   Assigns a game version to the Game in order to let the game to open a correct/understandable version of the initialization or game files
   @param char V Version to be set to the Game
*/
void CGame::SetVersion (char *V) 
{
	unsigned int VSize;

	VSize = strlen(V) + 1; //One extra position for the \0 at the end of the string for indicating the null terminated string

	if (Version)
		delete Version;
	
	Version = new char[VSize];
	
	strcpy(Version, V);
};

/**   
   @fn SetTitle 
   Assigns a game title
   @param char T Title to be set
*/
void CGame::SetTitle (char *T)
{
	unsigned int VSize;

	VSize = strlen(T) + 1; //One extra position for the \0 at the end of the string for indicating the null terminated string

	if (Title)
		delete Title;
	
	Title = new char[VSize];
	
	strcpy(Title, T);
};

void CGame::Save (char * fileName) {}; ///<Saves the game to a file - Rodrigo Aldecoa