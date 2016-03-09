/*	Definition of the class Scene Graph

	Prefix: CSGN_

	@author Ramon Molla
	@version 2011-08
*/

#ifndef CSGN_SCENE_GRAPH_NODE
#define CSGN_SCENE_GRAPH_NODE

#include "Characters.h"

class CSceneGraphNode
{
	//Atributes
public:

	GCHARS_CharacterType	Type;	// Type of character inserted into this node
	void *Character;			// The character inserted into this node

	//Methods
	void Init ();

	CSceneGraphNode(){Init();}	//Constructor of the class
};

#endif