/* 
	Author: Vicente Broseta Toribio
	Last update: 2013-06
*/

#include"TextureAnimationManager.h"


CTextureAnimationManager::CTextureAnimationManager(){
	
}

CTextureAnimationManager::~CTextureAnimationManager(){
	
}

int CTextureAnimationManager::CreateAnimation(double period, ANI_ANIMATION_TYPE type){

	CTextureAnimation *aniAux;
	aniAux = new (CTextureAnimation);

	aniAux->SetPeriod(period);
	aniAux->Type = type;
	aniAux->Id.IndexInMngr = Animations.size();
	aniAux->Id.Name = NULL;

	Animations.push_back(aniAux);

	return aniAux->Id.IndexInMngr;
}

int CTextureAnimationManager::CreateAnimation(double period, ANI_ANIMATION_TYPE type, unsigned int indexList){

	CTextureAnimation *aniAux;
	aniAux = new (CTextureAnimation);

	aniAux->SetPeriod(period);
	aniAux->Type = type;
	aniAux->Id.IndexInMngr = indexList;
	aniAux->Id.Name = NULL;

	if(indexList < Animations.size()){	
		Animations[indexList] = aniAux;
	}else if(indexList == Animations.size()){
		Animations.push_back(aniAux);
	}else{
		Animations.resize(indexList);
		Animations.push_back(aniAux);
	}

	return aniAux->Id.IndexInMngr;
}

int CTextureAnimationManager::CreateAnimation(char *name, double period, ANI_ANIMATION_TYPE type){

	stringstream ss;
	string strg;

	CTextureAnimation *aniAux;
	aniAux = new (CTextureAnimation);

	aniAux->SetPeriod(period);
	aniAux->Type = type;
	aniAux->Id.IndexInMngr = Animations.size();

	ss << name;
	strg = ss.str();
	aniAux->Id.Name = new char[strg.length()+1];
	strcpy(aniAux->Id.Name, strg.c_str());

	Animations.push_back(aniAux);

	return aniAux->Id.IndexInMngr;
}

int CTextureAnimationManager::CreateAnimation(char *name, double period, ANI_ANIMATION_TYPE type, unsigned int indexList){

	stringstream ss;
	string strg;

	CTextureAnimation *aniAux;
	aniAux = new (CTextureAnimation);

	aniAux->SetPeriod(period);
	aniAux->Type = type;
	aniAux->Id.IndexInMngr = indexList;

	ss << name;
	strg = ss.str();
	aniAux->Id.Name = new char[strg.length()+1];
	strcpy(aniAux->Id.Name, strg.c_str());

	if(indexList < Animations.size()){	
		Animations[indexList] = aniAux;
	}else if(indexList == Animations.size()){
		Animations.push_back(aniAux);
	}else{
		Animations.resize(indexList);
		Animations.push_back(aniAux);
	}

	return aniAux->Id.IndexInMngr;
}

int CTextureAnimationManager::SearchIndOfName(char *name){
	vector<CTextureAnimation*>::iterator it;

	string s1,s2;

	s1 = name;
	for(it=Animations.begin();it!=Animations.end();++it){
		if((*it)->Id.Name != NULL){
			s2 = (*it)->Id.Name;
			if(!s2.compare(s1)) return (*it)->Id.IndexInMngr;
		}
	}

	return -1;
}


/////////////////////////////////// PRIVATE ///////////////////////////////

