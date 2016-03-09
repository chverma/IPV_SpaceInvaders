/*	Class that creates a quadtree

	@author Hector Martinez Cebrian
	@version 2013-05-08
*/
#include "Quadtree.h"
#include "QuadtreeRoot.h"
#include <math.h>



Quadtree::Quadtree(float _x, float _y, float _width, float _height, int _level, int _maxLevel){
	
	x=_x;
	y=_y;
	width=_width;
	height=_height;
	level=_level;
	maxLevel=_maxLevel;
	minX=500000;
	minY=500000;
	maxX=-1000000;
	maxY=-1000000;
	cantidad=0;

	if(level!=maxLevel){
		
			NE= new Quadtree(x+width/2.f,y,width/2.f,height/2.f,level+1,maxLevel);
			NW= new Quadtree(x,y,width/2.f,height/2.f,level+1,maxLevel);
			SE= new Quadtree(x+width/2.f,y+height/2.f,width/2.f,height/2.f,level+1,maxLevel);
			SW= new Quadtree(x,y+height/2.f,width/2.f,height/2.f,level+1,maxLevel);
			
	}

}
	
Quadtree::~Quadtree(){
	if(level==maxLevel) return;

	delete NE,NW,SE,SW;
}
	

vector<CCharacter*> Quadtree::GetNearestObjectsTo(float _x, float _y, float radius){
		

	if(level==maxLevel){
		return characters;
	}

	vector<CCharacter*> returnCharacters, childReturnCharacters;
		
	if(Contains(NE,_x,_y,radius)){
		childReturnCharacters=NE->GetNearestObjectsTo(_x,_y,radius);
		returnCharacters.insert(returnCharacters.end(),childReturnCharacters.begin(),childReturnCharacters.end());
	}

	if(Contains(NW,_x,_y,radius)){
		childReturnCharacters=NW->GetNearestObjectsTo(_x,_y,radius);
		returnCharacters.insert(returnCharacters.end(),childReturnCharacters.begin(),childReturnCharacters.end());
	}

	if(Contains(SE,_x,_y,radius)){
		childReturnCharacters=SE->GetNearestObjectsTo(_x,_y,radius);
		returnCharacters.insert(returnCharacters.end(),childReturnCharacters.begin(),childReturnCharacters.end());
	}

	if(Contains(SW,_x,_y,radius)){
		childReturnCharacters=SW->GetNearestObjectsTo(_x,_y,radius);
		returnCharacters.insert(returnCharacters.end(),childReturnCharacters.begin(),childReturnCharacters.end());
	}

	return returnCharacters;
	
}

void Quadtree::AddCharacter(CCharacter *character){
	

	cantidad++;

	if(level==maxLevel){
		characters.push_back(character);
		QuadtreeRoot::Instance()->elementos[character->Type]++;
//		QuadtreeRoot::Instance()->elementos++;
		return;
	}
		
	if(Contains(NE,character)){
		NE->AddCharacter(character);
	}else if(Contains(NW,character)){
		NW->AddCharacter(character);
	}else if(Contains(SE,character)){
		SE->AddCharacter(character);
	}else{
		SW->AddCharacter(character);
	}
}

bool Quadtree::RemoveCharacter(CCharacter *character){
	
	//cantidad--;

	if(level==maxLevel){
		unsigned int i=0;
		for(vector<CCharacter *>::iterator it=characters.begin();it!=characters.end();){
				
			if(*it==character){
				it=characters.erase(it);
				QuadtreeRoot::Instance()->elementos[character->Type]--;
				//QuadtreeRoot::Instance()->elementos--;
				return true;
			}else{it++;}
		}
		return false;
	}
		
	if(Contains(NE,character)){
		if(!NE->RemoveCharacter(character))
			return NW->RemoveCharacter(character)||SE->RemoveCharacter(character)||SW->RemoveCharacter(character);
	}else if(Contains(NW,character)){
		if(!NW->RemoveCharacter(character))
			return NE->RemoveCharacter(character)||SE->RemoveCharacter(character)||SW->RemoveCharacter(character);
	}else if(Contains(SE,character)){
		if(!SE->RemoveCharacter(character))
			return NW->RemoveCharacter(character)||NE->RemoveCharacter(character)||SW->RemoveCharacter(character);
	}else{
		if(!SW->RemoveCharacter(character))
			return NW->RemoveCharacter(character)||SE->RemoveCharacter(character)||NE->RemoveCharacter(character);
	}
}

void Quadtree::Purge(){
	
	if(level==maxLevel){
			
		unsigned int i=0;
		while(i<characters.size()){
				
			if(characters.at(i)->Type>=0 && characters.at(i)->Type<20 /*&& Contains(this,characters.at(i))*/){
			i++;
			}else{
				characters.erase(characters.begin()+i);
			}

		}


	}else{
		NE->Purge();
		NW->Purge();
		SE->Purge();
		SW->Purge();
	}
	
}

bool Quadtree::ActualizeCharacter(CCharacter *character){
	
	if(level==maxLevel){
		//In this node exist the character
		if(Contains(this,character)) return true;
		else{
			unsigned int i=0;
			for(;i<characters.size();i++)
				if(characters.at(i)==character){
					characters.erase(characters.begin()+i);
					QuadtreeRoot::Instance()->elementos[character->Type]--;
					cantidad--;
					break;
				}
			}
		return false;
	}

	if(BeforeContains(NE,character)){
		bool ret=NE->ActualizeCharacter(character);
		if(!ret){
			if(Contains(NW,character)){
				NW->AddCharacter(character);
				return true;
			}else if(Contains(SE,character)){
				SE->AddCharacter(character);
				return true;
			}else if(Contains(SW,character)){
				SW->AddCharacter(character);
				return true;
			}
			return false;
		}else return true;
	}

	else if(BeforeContains(NW,character)){
		bool ret=NW->ActualizeCharacter(character);
		if(!ret){
			if(Contains(NE,character)){
				NE->AddCharacter(character);
				return true;
			}else if(Contains(SE,character)){
				SE->AddCharacter(character);
				return true;
			}else if(Contains(SW,character)){
				SW->AddCharacter(character);
				return true;
			}
			return false;
		}else return true;
	}

	else if(BeforeContains(SE,character)){
		bool ret=SE->ActualizeCharacter(character);
		if(!ret){
			if(Contains(NW,character)){
				NW->AddCharacter(character);
				return true;
			}else if(Contains(NE,character)){
				NE->AddCharacter(character);
				return true;
			}else if(Contains(SW,character)){
				SW->AddCharacter(character);
				return true;
			}
			return false;
		}else return true;
	}

	else if(BeforeContains(SW,character)){
		bool ret=SW->ActualizeCharacter(character);
		if(!ret){
			if(Contains(NW,character)){
				NW->AddCharacter(character);
				return true;
			}else if(Contains(SE,character)){
				SE->AddCharacter(character);
				return true;
			}else if(Contains(NE,character)){
				NE->AddCharacter(character);
				return true;
			}
			return false;
		}else return true;
	}
	else AddCharacter(character);
return false;	
}

void Quadtree::Clear(){
	
	if(level==maxLevel){
		characters.clear();
		return;
	}
	
	NE->Clear();
	NW->Clear();
	SE->Clear();
	SW->Clear();

}

bool Quadtree::Contains(Quadtree *child, CCharacter *character){
	
	return/* !(character->Position.v[XDIM] < child->x ||
		character->Position.v[YDIM] < child->y ||
		character->Position.v[XDIM] > child->x + child->width  ||
		character->Position.v[YDIM] > child->y + child->height);*/
		(character->Position.v[XDIM]>= child->x && character->Position.v[XDIM]<child->x+child->width) &&
		(character->Position.v[YDIM]>= child->y && character->Position.v[YDIM]<child->y+child->height);
}

bool Quadtree::BeforeContains(Quadtree *child, CCharacter *character){
	
	return /*!(character->PositionPrev.v[XDIM] < child->x ||
		character->PositionPrev.v[YDIM] < child->y ||
		character->PositionPrev.v[XDIM] > child->x + child->width  ||
		character->PositionPrev.v[YDIM] > child->y + child->height);*/
	
		(character->PositionPrev.v[XDIM]>= child->x && character->PositionPrev.v[XDIM]<child->x+child->width) &&
		(character->PositionPrev.v[YDIM]>= child->y && character->PositionPrev.v[YDIM]<child->y+child->height);
}

bool Quadtree::Contains(Quadtree *child, float _x, float _y, float radius){
	
	
	float circleDistanceX = abs(_x - child->x - child->width/2);
	float circleDistanceY = abs(_y - child->y - child->height/2);
 
	if (circleDistanceX > (child->width/2 + radius)) { return false; }
	if (circleDistanceY > (child->height/2 + radius)) { return false; }
 
	if (circleDistanceX <= (child->width/2)) { return true; }
	if (circleDistanceY <= (child->height/2)) { return true; }
 
	float cornerDistance_sq = pow(circleDistanceX - child->width/2, 2) + pow(circleDistanceY - child->height/2, 2);
 
	return (cornerDistance_sq <= (pow(radius, 2)));	
}

bool Quadtree::ContainsBigCharacter(Quadtree *child, CCharacter *character){
		
	return (child->x + child->width		> character->Position.v[XDIM]-(character->Size.v[XDIM]/2.f) &&
			child->x					< character->Position.v[XDIM]+(character->Size.v[XDIM]/2.f) &&
			child->y + child->height	> character->Position.v[YDIM]-(character->Size.v[YDIM]/2.f) &&
			child->y					< character->Position.v[YDIM]+(character->Size.v[YDIM]/2.f)
			);


}

void Quadtree::AddBigCharacter(CCharacter *character){
	
	cantidad++;

	if(level==maxLevel){
		characters.push_back(character);
		QuadtreeRoot::Instance()->elementos[character->Type]++;
		return;
	}
		
	if(ContainsBigCharacter(NE,character)){
		NE->AddBigCharacter(character);
	}
	if(ContainsBigCharacter(NW,character)){
		NW->AddBigCharacter(character);
	}
	if(ContainsBigCharacter(SE,character)){
		SE->AddBigCharacter(character);
	}
	if(ContainsBigCharacter(SW,character)){
		SW->AddBigCharacter(character);
	}
	
}

void Quadtree::RemoveBigCharacter(CCharacter *character){}	

void Quadtree::Render(){	
	if(level==maxLevel){
		if(characters.size()==0) glColor3f(1.0,0.0,0.0);
		else {glColor3f(0.0,(GLfloat)1.0/characters.size(),0.0);
			
		glBegin(GL_LINE_STRIP);
		glVertex3f(x,y,0);
		glVertex3f(x,y+width,0);
		glVertex3f(x+height,y+width,0);
		glVertex3f(x+height,y,0);
		glVertex3f(x,y,0);
		glEnd();}
	}
	if(level!=maxLevel){
		/*glColor3f(0.0f,0.0f,1.0f);
		glBegin(GL_LINE_STRIP);
		glVertex3f(x,y,0);
		glVertex3f(x,y+width,0);
		glVertex3f(x+height,y+width,0);
		glVertex3f(x+height,y,0);
		glVertex3f(x,y,0);
		glEnd();
	*/
		SW->Render();
		SE->Render();
		NW->Render();
		NE->Render();
	}
}