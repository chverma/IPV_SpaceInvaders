/*	Definition of the class Scene Graph

	Prefix: CSG_

	@author Ramon Molla
	@version 2011-08
*/

#ifndef CSG_SCENE_GRAPH
#define CSG_SCENE_GRAPH

#ifndef DEQUE_LOADED
#define DEQUE_LOADED
#include <deque>			// Uses Standard Template Library Double Ended Queue
using namespace std;
#endif

#include "SceneGraphNode.h"	// Node to insert into every node of the deque


class CSceneGraph
{
	//Atributes
public:

	deque<CSceneGraphNode> Graph;

	//Methods
	void Init (void);
	CSceneGraph(){Init();}	//Constructor of the class

	//Creates a new graph node and inserts inside a object of class "Type"
	void CreateCharacter (GCHARS_CharacterType	Type); 
	void Render();			//Renders on the screen the scene
};

//Definitions for the game


#endif