/* 
	Author: Vicente Broseta Toribio
	Last update: 2013-06
*/

#include"TexturesManager.h"


CTexturesManager::CTexturesManager(){
	FilesPath = NULL;
}

CTexturesManager::~CTexturesManager(){
	for(unsigned int i=0;i<Textures.size();i++)
		delete(Textures[i]);
}

void CTexturesManager::SetFilesPath(char *path){
	if(!FilesPath == NULL) delete(FilesPath);

	stringstream ss;
	string strg;

	ss << path;
	strg = ss.str();
	FilesPath = new char[strg.length()+1];
	strcpy(FilesPath, strg.c_str());
}

void CTexturesManager::CreateTexture(char *filename, unsigned int indexList){
	
	stringstream ss, ss1;
	string strg;

	CTexture *texAux;
	texAux = new (CTexture);

	texAux->SetFilePath(FilesPath);
	texAux->Load(filename);
	texAux->IndexInList = indexList;
	
	if(indexList < Textures.size()){	
		Textures[indexList] = texAux;
	}else if(indexList == Textures.size()){
		Textures.push_back(texAux);
	}else{
		Textures.resize(indexList);
		Textures.push_back(texAux);
	}
	
}

void CTexturesManager::CreateTexture(char *filename)
{
	CTexture *texAux;

	for(unsigned int i = 0; i<Textures.size();i++)
		if (0 == strcmp(filename, Textures[i]->GetFileName()))
			return;

	//This texture has not been stored before
	texAux = new (CTexture);

	texAux->IndexInList = Textures.size();
	texAux->SetFilePath(FilesPath);
	texAux->Load(filename);
	
	Textures.push_back(texAux);
}

void CTexturesManager::FreePositionOfTexture(unsigned int indexList){
	Textures[indexList]->FreeTexture();
}

void CTexturesManager::ReserveTexturePosition(unsigned int indexList){
	CTexture *texAux;
	texAux = new (CTexture);
	texAux->IndexInList = indexList;
	Textures.insert(Textures.begin()+indexList, texAux);
}

void CTexturesManager::AssignFile2Texture(char *path, char *filename, unsigned int indexList)
{
	if(Textures[indexList]->IndexInList == indexList)
	{
		Textures[indexList]->SetFilePath(path);
		Textures[indexList]->Load(filename);
	}
}


/////////////////////////////////// PRIVATE ///////////////////////////////

