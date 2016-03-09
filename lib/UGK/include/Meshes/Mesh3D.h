/* 
	Author: Vicente Broseta Toribio
	Last update: 2013-06
*/

#ifndef MODELO3D_H
#define MODELO3D_H

#include "Model_3DS.h"
#include<sstream>
#include<string>
using namespace std;

class CMesh3d {
public:
	Model_3DS modelo;
	unsigned int IndexInList;
	char *FilePath;
	char *FileName;
	bool Loaded;

	CMesh3d();
	~CMesh3d();

	int LoadMesh(char *filename);
	int LoadMesh();

};

#endif // MODELO3D.H