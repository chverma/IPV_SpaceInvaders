/**	Definition of the class CTexturesManager. Base class for all textures in the system

*	Prefix: CTM_

*	Author: Vicente Broseta Toribio
*	Last update: 2013-06

*/

#ifndef TEXTURESMANAGERV_H
#define TEXTURESMANAGERV_H

#include"Texture.h"
#include <vector>

#define CTM_NO_TEXTURE -1

class CTexturesManager {
public:
	
	vector <CTexture*> Textures;
	vector <CTexture*> Aux;
	char *FilesPath;
	unsigned int LogIndex;

	CTexturesManager();
	~CTexturesManager();
	
	void SetFilesPath(char *path);
	void CreateTexture(char *filename, unsigned int indexList);		//Creates one new Texture, assing position and load.
	void CreateTexture(char *filename);								//Creates one new Texture at the end position.
	void FreePositionOfTexture(unsigned int indexList);
	void ReserveTexturePosition(unsigned int indexList);			//Creates one new Texture and assing position (without load).
	void AssignFile2Texture(char *path, char *filename, unsigned int indexList);			//Assign a file to texture already created in reserved position.

private:

	
};

#endif // TEXTURESMANAGERV.H