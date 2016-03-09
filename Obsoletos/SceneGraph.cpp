/*	Implementation of the class Scene Graph

	Prefix: CSG_

	@author Ramon Molla
	@version 2011-08
*/

#include "Scenegraph.h"
#include "Characters.h"

void CSceneGraph::CreateCharacter (GCHARS_CharacterType Type)
{
	//Creates a new node
	CSceneGraphNode *Node = new CSceneGraphNode;
	Node->Type = Type;
	switch (Type)
	{case CHARS_BONUS:
		Node->Character = (void*)new CBonus;
		break;
	case CHARS_LASER:
		break;
	case CHARS_MISSIL:
		break;
	case CHARS_PLAYER:
		break;
	case CHARS_SHIP:
		break;
	case CHARS_SPHERE_OGIVE:
		break;
	case CHARS_SUPPLYSHIP:
		break;
	case CHARS_WEAPON:
		break;
	default:;
	};
}

void CSceneGraph::Init (void)
{
}
