/** Header with definitions needed for the management of any 3D mesh
*	Prefix: CM3D_	for class graphic card

*	@author Vicente Broseta Toribio
	First update: 2013-06
*	@Mofified: Ramón Mollá
*	@version 01-2015
*/



#ifndef MODELO3D_H
#define MODELO3D_H

#include "Model_3DS.h"
#include<sstream>
#include<string>
using namespace std;

#define CM3D_NO_MESH -1

class CMesh3d
{
	char *FilePath;
	char *FileName;

	char		*GetPathFile (char *filename);
	inline char *GetPathFile () {return GetPathFile(FileName);}

public:
	Model_3DS modelo;
	unsigned int IndexInList;
	bool Loaded;

	CMesh3d();
	~CMesh3d(){}

	int			LoadMesh(char * Modelname);
	inline int	LoadMesh()	{return LoadMesh(FileName);}

	void SetFileName(char*);
	void SetFilePath(char*);

	inline char *GetFileName(){return FileName;};
	inline char *GetFilePath(){return FilePath;};

};

#endif // MODELO3D.H