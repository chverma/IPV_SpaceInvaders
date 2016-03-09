/*	Definition of the class Bunkers Manager

	Prefix: CBM_

	@author Ramon Molla
	@version 2011-10
*/

#ifndef CBM_BUNKERSMNG
#define CBM_BUNKERSMNG

#include "Bunker.h"

//Definitions for the game
#define CBM_MAX_BUNKERS 6	//Amount of simultaneous Bunkers in the same screen in a given time

class CBunkersManager: public CCharacter
{
	
public:

	CBunker				Bunker[CBM_MAX_BUNKERS];	/// Maximun available bunkers at a given moment
	unsigned int		BunkerAvailable,			/// Pointer to the current available bunker. For initialization purpouses
						BunkersAmount;				/// Number of bunkers actually available at a given level
	CBNK_BUNKER_TYPE	DefaultBunker;				/// What is the default type of bunker to make
	CHAR_RENDER_MODE	RenderMode;					/// The way the character is rendered on the screen

	//Methods
	void Init (void);

	CBunkersManager(){Init();}	///Constructor of the class

	void NewBunker(Vector &Pos);	///Generates a new bunker

	///Shows all the bunkers on the screen
	void Render(void);
	///Change the way all the bunkers are going to be rendered on the screen
	void ChangeRenderMode(CHAR_RENDER_MODE);

	void Update ();		//Update the bunkers. Similar to render() but updating
	void UpdateAABB();	///<Recalculates AABB of every bunker

};

#endif