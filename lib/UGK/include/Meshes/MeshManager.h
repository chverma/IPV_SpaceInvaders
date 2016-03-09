/**	Definition of the class Mesh Manager

*Prefix: CMM_

	@Author: Vicente Broseta Toribio
	@Last update: 2013-06
*/

#ifndef CMM_MESHMANAGER //se borra la macro Manegador de Malla
#define CMM_MESHMANAGER //se define la macro Manegador de Malla

//#include "Copyright.h"
#include "Mesh3D.h"
#include <vector>

#define CMM_NO_MESH -1

/**   
   * Defines the class MeshManager
   * Where are the constructor, destructor and start
   * The characters or ships the same type have the same geometry, anyone have their mesh
   * Defines one Mesh array on base 3DS.h
*/
class CMeshesManager
{
	//Atributes
	
public:

	/// All the characters of the same type, use the same geometry. So there is no reason to replicate
	/// the same mesh for everyone. Every character has a pointer to a given mesh type.
	/// There is a need to have a common repository for every possible character mesh
	std::vector<CMesh3d*> Meshes;	///<An array of 3DS meshes
	char * FilesPath;
	unsigned int LogIndex;
    
	///Constructor of the class
	CMeshesManager	();

    ///Destructor of the class
	~CMeshesManager	();

	void AddModel(char *filename, unsigned int indexList);
	void AddModel(char *filename);
	void SetFilesPath(char *path);
	void ReserveMeshPosition(unsigned int indexList);
	void AssignFile2Mesh(char *path, char *filename, unsigned int indexList);
};

#endif
