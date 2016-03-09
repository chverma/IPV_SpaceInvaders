/*	Class that creates a quadtree root

	@author Hector Martinez Cebrian
	@version 2013-05-08
*/
#ifndef QTR_QUADTREEROOT
#define QTR_QUADTREEROOT
#include "Quadtree.h"

class QuadtreeRoot{

	static Quadtree* quadtree;
	

	public:
	
		static QuadtreeRoot* Instance();
		Quadtree* getQuadtree();
		void setQuadtree(float x,float y,float width, float height, int maxLevel);
		void Render();
		int	 elementos[16];

	protected:
		QuadtreeRoot();
		QuadtreeRoot(const QuadtreeRoot &);
		QuadtreeRoot &operator= (const QuadtreeRoot &);
		//static void	Init(float x, float y, float width, float height, int level, int maxLevel);

	private:
		static QuadtreeRoot* pinstance;
	
	
};
#endif