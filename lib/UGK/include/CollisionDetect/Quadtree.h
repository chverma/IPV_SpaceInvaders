/*	Class Quad Tree for collision detection support

*	Prefix: QT_

	@author Hector Martinez Cebrian
	@version 2013-05-08
*/

#ifndef QT_QUADTREE
#define QT_QUADTREE

#include <vector>
#include <UGKCharacter.h>

using namespace UGK;

class Quadtree{

	//static int maxLevel; I comment this line and add a maxLevel in each quadtree node for more flexibility. 
private:

	float				x;
	float				y;
	float				width;
	float				height;
	int					level;
	int					maxLevel;
	int					cantidad;
	vector<CCharacter*>	characters;

	float	minX,minY,maxX,maxY;

	Quadtree* parent;
	Quadtree* NE;
	Quadtree* NW;
	Quadtree* SE;
	Quadtree* SW;

	bool				Contains(Quadtree *child, CCharacter *character);
	bool				ContainsBigCharacter(Quadtree *child, CCharacter *character);
	bool				BeforeContains(Quadtree *child, CCharacter *character);
	bool				Contains(Quadtree *child, float x, float y, float radius);

public:
	
							Quadtree(float _x, float _y, float _width, float _height, int _level, int _maxLevel);
							~Quadtree();
	void					AddCharacter(CCharacter *character);
	bool					RemoveCharacter(CCharacter *character);
	void					AddBigCharacter(CCharacter *character);
	void					RemoveBigCharacter(CCharacter *character);
	bool					ActualizeCharacter(CCharacter *character);
	vector<CCharacter*>		GetNearestObjectsTo(float _x, float _y, float radius);
	void					Clear();
	void					Render();
	void					Purge();


};

#endif