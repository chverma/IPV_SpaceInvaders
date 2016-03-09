/* 
	Author: Vicente Broseta Toribio
	Last update: 2013-06
*/

#ifndef TEXTUREANIMATIONMANAGER_H
#define TEXTUREANIMATIONMANAGER_H

#include"TextureAnimation.h"
#include <vector>

class CTextureAnimationManager {
public:
	
	vector <CTextureAnimation*> Animations;
	unsigned int LogIndex;

	CTextureAnimationManager();
	~CTextureAnimationManager();
	
	int CreateAnimation(double period, ANI_ANIMATION_TYPE type);
	int CreateAnimation(double period, ANI_ANIMATION_TYPE type, unsigned int indexList);
	int CreateAnimation(char *name, double period, ANI_ANIMATION_TYPE type);
	int CreateAnimation(char *name, double period, ANI_ANIMATION_TYPE type, unsigned int indexList);
	int SearchIndOfName(char *name);

private:

	
};

#endif // TEXTUREANIMATIONMANAGER.H