/*/////////////////////////////////////////////////////////////////////////////////////
//
// Bunkers manager
//

	@author Ramon Molla
	@version 2011-11
*/

#include <BunkersManager.h>

//////////////////////////////////////////////////////////////////////////////////////
//
// Bunkers manager Initialisation 
//
//

void CBunkersManager::Init (void)	//Used when all the values are initialized by default
						
{
	BunkerAvailable = 0;
	BunkersAmount	= CBM_MAX_BUNKERS;
	DefaultBunker	= CBNK_HOME;
	RenderMode		= CHAR_2D;		///By default, render is always in 2D

	for(unsigned int i=0; i<CBM_MAX_BUNKERS; i++)
		Bunker[i].Active = Bunker[i].Alive = false;

}

void CBunkersManager::NewBunker(Vector &Pos)	///Generates a new shoot
{
	Bunker[BunkerAvailable].Alive	= true;
	Bunker[BunkerAvailable].Active	= true;
	Bunker[BunkerAvailable].TranslateBunker(Pos);
	BunkerAvailable++;

	if (BunkerAvailable >= CBM_MAX_BUNKERS)	/// If the pointer has reached the top of the array
		BunkerAvailable = 0;				/// It has to be reset to the begining again
}

void CBunkersManager::Render (void)	///Paint every bunker on the screen
{
	switch(RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character
		return;
	case CHAR_2D:
	case CHAR_LINES3D:
	case CHAR_3D:
		///Renders all the Bunkers available
		for (unsigned int k=0; k < CBM_MAX_BUNKERS; k++)
			Bunker[k].Render();
		break;
	default: return;
	}
}

void CBunkersManager::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	RenderMode = Mode;

	for (unsigned int k=0; k < CBM_MAX_BUNKERS; k++)	Bunker[k].ChangeRenderMode(Mode);
}

void CBunkersManager::Update ()		//Update the bunkers. Similar to render() but updating
{
	//No action in this game version
}

#ifdef CHAR_USE_AABB
///Recalculates AABB of every bunker
void CBunkersManager::UpdateAABB()
{
	for(unsigned int i=0; i<CBM_MAX_BUNKERS; i++)
		Bunker[i].UpdateAABB();
}
#endif
