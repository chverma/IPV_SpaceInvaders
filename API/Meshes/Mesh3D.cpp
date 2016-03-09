/** Header with definitions needed for the management of any 3D mesh
*	Prefix: CM3D_	for class graphic card

*	@author Vicente Broseta Toribio
	First update: 2013-06
*	@Mofified: Ramón Mollá
*	@version 01-2015
*/

#include <Mesh3D.h>
#include <UGKOS.h>

CMesh3d::CMesh3d()
{
	IndexInList	= CM3D_NO_MESH;
	FilePath	= NULL;
	FileName	= NULL;
	Loaded		= false;
}

char *CMesh3d::GetPathFile (char *filename)
{
	char dir[1024];
	
	if (NULL == filename)
		return NULL;

	strcpy(dir, UGKOS_THIS_DIRECTORY);
	strcat(dir, UGKOS_DIRECTORY_SEPARATOR);

	if (NULL != FilePath)
	{
		strcat(dir, FilePath);
		strcat(dir, UGKOS_DIRECTORY_SEPARATOR);
	}
	strcat(dir, filename);
	return dir;
}

int CMesh3d::LoadMesh(char * Modelname)
{
	char dir[1024];

	SetFileName(Modelname);
	modelo.path = FilePath;
	modelo.modelname = FileName;
	strcpy(dir, GetPathFile());
	modelo.Load(dir);
	Loaded = true;

	return 0;
}

void CMesh3d::SetFileName(char* filename)
{
	if (NULL != FileName)
		delete FileName;

	if (NULL == filename)
		FileName = NULL;
	else
	{
		FileName = new char[strlen(filename)+1];	//One more for the "\n" string ending char
		strcpy(FileName, filename);
	}
	
}

void CMesh3d::SetFilePath(char* filepath)
{
	if (NULL != FilePath)
		delete FilePath;

	if (NULL == filepath)
		FileName = NULL;
	else
	{
		FilePath = new char[strlen(filepath)+1];	//One more for the "\n" string ending char
		strcpy(FilePath, filepath);
	}
}



