/** 
	@author Vicente Broseta
	@version 2013-06

	* Version: 01-2015
	* Refactored: Ramón Mollá
*/

#include <MeshManager.h>

CMeshesManager::CMeshesManager()
{
	FilesPath = NULL;
}

/**   
   * @fn CMeshManager::~CMeshManager
   * destroyer Mesh Manager
   * Destroys all the behaviors of the game and their states lists.
*/
CMeshesManager::~CMeshesManager()
{
	for(unsigned int i=0;i<Meshes.size();i++)
		delete(&Meshes[i]->modelo);
}

void CMeshesManager::AddModel(char *filename, unsigned int indexList)
{
	CMesh3d *modAux;
	modAux = new (CMesh3d);

	modAux->SetFilePath(FilesPath);
	modAux->LoadMesh(filename);
	modAux->IndexInList = indexList;
	
	if(indexList < Meshes.size()){	
		Meshes[indexList] = modAux;
	}else if(indexList == Meshes.size()){
		Meshes.push_back(modAux);
	}else{
		Meshes.resize(indexList);
		Meshes.push_back(modAux);
	}
}

void CMeshesManager::AddModel(char *filename)
{
	CMesh3d *modAux;

	for(unsigned int i = 0; i<Meshes.size();i++)
		if (0 == strcmp(filename, Meshes[i]->GetFileName()))
			return;

	//This mesh has not been stored before
	modAux = new (CMesh3d);

	modAux->IndexInList = Meshes.size();
	modAux->SetFilePath(FilesPath);
	modAux->LoadMesh(filename);
	
	Meshes.push_back(modAux);
}

void CMeshesManager::SetFilesPath(char *path){
	if(NULL != FilesPath)
		delete(FilesPath);

	if(NULL == path)
	{
		FilesPath = NULL;
		return;
	}

	FilesPath = new char[strlen(path)+1];
	strcpy(FilesPath, path);
}

void CMeshesManager::ReserveMeshPosition(unsigned int indexList){
	CMesh3d *meshAux;
	meshAux = new (CMesh3d);
	meshAux->IndexInList = indexList;
	Meshes.insert(Meshes.begin()+indexList, meshAux);
}

void CMeshesManager::AssignFile2Mesh(char *path, char *filename, unsigned int indexList)
{
	if(Meshes[indexList]->IndexInList == indexList)
	{
		Meshes[indexList]->SetFilePath(path);
		Meshes[indexList]->LoadMesh(filename);
	}
}