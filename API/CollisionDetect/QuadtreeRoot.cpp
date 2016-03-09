/*	Class that creates a quadtree root

	@author Hector Martinez Cebrian
	@version 2013-05-08
*/

#include "QuadtreeRoot.h"
#include "Quadtree.h"



QuadtreeRoot* QuadtreeRoot::pinstance=0;
Quadtree* QuadtreeRoot::quadtree=NULL;

QuadtreeRoot::QuadtreeRoot(){
	//for(int i=CHARS_MAX
}

QuadtreeRoot* QuadtreeRoot::Instance(){
	if(pinstance==0)
		pinstance=new QuadtreeRoot;
	return pinstance;
}

Quadtree* QuadtreeRoot::getQuadtree(){

	return quadtree;

}

void QuadtreeRoot::Render(){
	quadtree->Render();
}

void QuadtreeRoot::setQuadtree(float x,float y,float width, float height, int maxLevel){
	
	quadtree=new Quadtree(x,y,width,height,1,maxLevel);
}

	