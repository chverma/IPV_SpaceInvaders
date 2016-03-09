/**	Physics declarations
*	@author Ramon Molla
*	@version 2011-09-11
*/

#include <CollisionDetect.h>
#include <UGKLog.h>
#include <QuadtreeRoot.h>

char CD_PhysicsMsgs[CD_ERROR_MAX_LOGMSG][CD_MSJ_MAX_LONG] = 
{
	"Position out of range"
};

extern CLog Log;

using namespace UGK;

#ifdef CHAR_USE_AABB
///Increases in one all positions stored from a given point up to the AABB deque end
void CCollisionDetection::Renumber (UGK::SpaceCoords Dim, unsigned int p)
{
	unsigned int i, Size = Axis[Dim].size();

	for(i=p;i<Size;i++)
		Axis[Dim][i]->Position = i;
}

///Inserts a new object into the AABB structure
void CCollisionDetection::Insert (CCharacter *C)
{
	///Insert minimun AABB values for all the axises
	///All dimensions have the same size. Obviously
		unsigned int Dim;
	int i, Size;
	bool insertedMin,
		 insertedMax;
	deque<CBBPoint*>::iterator BBPoint;

	for (Dim = XDIM; Dim < WDIM; Dim++)
	{
		Size = Axis[Dim].size();
		if (Size)
		{
			insertedMin = insertedMax = false;
			if (CHAR_NO_AABB != C->AABB[CHAR_BBMAX][Dim].Position)
				if (Size <= C->AABB[CHAR_BBMAX][Dim].Position)
				{
					//Error: pointer out of range
					Log.DisplayMsgBoxErrorAndExit("Physics.cpp", 39, CD_PhysicsMsgs[CD_POSITION_OUTRANGE]);
					return;
				}
				else Remove(C);

			BBPoint	= Axis[Dim].begin();
			for(i = 0;i<Size;i++)
			{
				if (C->AABB[CHAR_BBMIN][Dim].Value <= Axis[Dim][i]->Value)
				{	//Updates current position
					C->AABB[CHAR_BBMIN][Dim].Position = i;
					//The Character has to be inserted just before
					Axis[Dim].insert(BBPoint + i, &C->AABB[CHAR_BBMIN][Dim]);
					insertedMin = true;
					i++;
					Size++;
					Renumber(Dim,i);
					break;
				}
			}
			//After deque insert command, all iterators are overridden. So they have to be recalculated			
			BBPoint = Axis[Dim].begin();
			
			for(;i<Size;i++)
			{
				if (C->AABB[CHAR_BBMAX][Dim].Value <= Axis[Dim][i]->Value)
				{	//Updates current position
					C->AABB[CHAR_BBMAX][Dim].Position = i;
					//The Character has to be inserted just before
					Axis[Dim].insert(BBPoint + i, &C->AABB[CHAR_BBMAX][Dim]);
					insertedMax = true;
					Renumber(Dim,i+1);
					break;
				}
			}

			if (!insertedMin)
			{
				//Reached the end of the deque and all characters have values under this character. Character has to be inserted at the end
				C->AABB[CHAR_BBMIN][Dim].Position = Axis[Dim].size();
				Axis[Dim].push_back(&C->AABB[CHAR_BBMIN][Dim]);
				//Since it is the last one, no renumbering has to be made
			}
			if (!insertedMax)
			{
				//Reached the end of the deque and all characters have values under this character. Character has to be inserted at the end
				C->AABB[CHAR_BBMAX][Dim].Position = Axis[Dim].size();
				Axis[Dim].push_back(&C->AABB[CHAR_BBMAX][Dim]);
				//Since it is the last one, no renumbering has to be made
			}
		}
		else	//No character currently inserted. This is the first one
		{
			C->AABB[CHAR_BBMIN][Dim].Position = 0;
			Axis[Dim].push_back(&C->AABB[CHAR_BBMIN][Dim]);
			C->AABB[CHAR_BBMAX][Dim].Position = 1;
			Axis[Dim].push_back(&C->AABB[CHAR_BBMAX][Dim]);
		}
	}
}
#endif

///Deletes an object from the structure
void CCollisionDetection::Remove (CCharacter *C)
{
	
#ifdef CHAR_USE_QUADTREE
	QuadtreeRoot::Instance()->getQuadtree()->RemoveCharacter(C);
#elif CHAR_USE_AABB
		unsigned int Dim;
	
	int	i = 0,
		Size;

	Size = Axis[XDIM].size();

	if (Size)
	{	//If any dimension has a CHAR_NO_AABB position, so all the dimensions are the same and no AABB entrance exists
		if (CHAR_NO_AABB != C->AABB[CHAR_BBMAX][XDIM].Position)
			if (Size <= C->AABB[CHAR_BBMAX][XDIM].Position)
			{
				//Error: pointer out of range
				Log.DisplayMsgBoxErrorAndExit("Physics.cpp", 108, CD_PhysicsMsgs[CD_POSITION_OUTRANGE]);
				return;
			}
	}
	else return;	//If no character is in the collision detector, there is no need for removing anything at all

	//All dimensions have the same size. Obviously
	for (Dim = XDIM; Dim < WDIM; Dim++)
	{
		i = C->AABB[CHAR_BBMAX][Dim].Position;
		C->AABB[CHAR_BBMAX][Dim].Position = CHAR_NO_AABB;
		Axis[Dim].erase(Axis[Dim].begin()+i);

		i = C->AABB[CHAR_BBMIN][Dim].Position;
		C->AABB[CHAR_BBMIN][Dim].Position = CHAR_NO_AABB;
		Axis[Dim].erase(Axis[Dim].begin()+i);

		//Reorder all positions in the deque from the deleted character to the end substracting one position since an element has been removed
		//The position of the Min coordinate of the AABB will be always lower than the max one. So, there is no need to repeat this proccess before 
		//for the Max AABB point
		Renumber (Dim, i);
	}
#endif
}

#ifdef CHAR_USE_AABB

void CCollisionDetection::UpdateBBPoint (unsigned int i, UGK::SpaceCoords Dim)
{
	//i means the position/index in the queue of the current character AABB minimun value in the dimension given
	CBBPoint *Aux;
	unsigned int LastPosition;

	//Check for good order in the maximun character AABB on the given axis
	//Compare to the left
	//While it is not the first character in the array and there is a previous AABB point higher than it
	if (i && Axis[Dim][i-1]->Value > Axis[Dim][i]->Value)
	{
		do
		{	//Swap Maximun character AABB on X axis and previous element
			Aux				= Axis[Dim][i-1];
			Axis[Dim][i-1]	= Axis[Dim][i];
			Axis[Dim][i]	= Aux;

			//Update indexes
			Axis[Dim][i]->Position = i--;
		}	while (i && Axis[Dim][i-1]->Value > Axis[Dim][i]->Value);
		Axis[Dim][i]->Position = i;	//The last time it is swapped it has to be updated its position in the axis queue
		return;
	}

	//Compare to the right
	LastPosition = Axis[Dim].size()-1;
	//While the position is not the last one and there is a next AABB point lower than it
	while (i<LastPosition && Axis[Dim][i+1]->Value < Axis[Dim][i]->Value )
	{	//Swap Maximun character AABB on X axis and next element
		Aux				= Axis[Dim][i+1];
		Axis[Dim][i+1]	= Axis[Dim][i];
		Axis[Dim][i]	= Aux;

		//Update indexes
		Axis[Dim][i]->Position = i++;
	}//Swap maximun

	Axis[Dim][i]->Position = i;
}
#endif

///Updates an object AABB inside the structure.
void CCollisionDetection::Update (CCharacter *C)
{
#ifdef CHAR_USE_QUADTREE
	QuadtreeRoot::Instance()->getQuadtree()->ActualizeCharacter(C);
#elif CHAR_USE_AABB
	unsigned int i;

	for (i=XDIM;i<WDIM;i++)
	{
		if (CHAR_NO_AABB == C->AABB[CHAR_BBMIN][i].Position)
			return;	//There is no insertion of this character in the Collision Detector for this dimension. No update is possible
		UpdateMin(C, (UGK::SpaceCoords) i);
		UpdateMax(C, (UGK::SpaceCoords) i);
	}
#endif
}

void CCollisionDetection::Init(void)
{
	//Nothing to do by the moment
#ifdef CHAR_USE_QUADTREE
	QuadtreeRoot::Instance()->setQuadtree(-10.f,-7.5f,20.f,20.f,6);
#elif CHAR_USE_AABB
#endif
};

///Performs a general collision detection for all the virtual world
void CCollisionDetection::DetectCollisions(void)
{
}

///<Returns if both objects have collided one another. If no collision detected between them, false is returned
///A general AABB collision detection is performed
bool CCollisionDetection::Collided (CCharacter *C1, CCharacter *C2)
{
#ifdef CHAR_USE_AABB
	unsigned int i;

	for (i=XDIM;i<WDIM;i++)
		if (C1->AABB[CHAR_BBMIN][i].Value > C2->AABB[CHAR_BBMAX][i].Value)
			//Both object do not touch
			return false;
		else if (C2->AABB[CHAR_BBMIN][i].Value > C1->AABB[CHAR_BBMAX][i].Value)
				//Both object do not touch
				return false;
	
	//Warn both objects that they have collided one another
	C1->Collided(C2);
	C2->Collided(C1);
	return true;
#elif CHAR_USE_QUADTREE
	float xIzqC1 = C1->Position.v[XDIM]-(C1->Size.v[XDIM]/2);
	float xDerC1 = C1->Position.v[XDIM]+(C1->Size.v[XDIM]/2);
	float xCenC1 = C1->Position.v[XDIM];
	float yBajC1 = C1->Position.v[YDIM]-(C1->Size.v[YDIM]/2);
	float yArrC1 = C1->Position.v[YDIM]+(C1->Size.v[YDIM]/2);
	float yCenC1 = C1->Position.v[YDIM];

	float xIzqC2 = C2->Position.v[XDIM]-(C2->Size.v[XDIM]/2);
	float xDerC2 = C2->Position.v[XDIM]+(C2->Size.v[XDIM]/2);
	float xCenC2 = C2->Position.v[XDIM];
	float yBajC2 = C2->Position.v[YDIM]-(C2->Size.v[YDIM]/2);
	float yArrC2 = C2->Position.v[YDIM]+(C2->Size.v[YDIM]/2);
	float yCenC2 = C2->Position.v[YDIM];

	/*if((C1->Position.v[XDIM]-(C1->Size.v[XDIM]/2)>= C2->Position.v[XDIM]-(C2->Size.v[XDIM]/2) 
	&& C1->Position.v[XDIM]-(C1->Size.v[XDIM]/2)<C2->Position.v[XDIM]+(C2->Size.v[XDIM]/2)) 
	&& (C1->Position.v[YDIM]-(C1->Size.v[YDIM]/2)>= C2->Position.v[YDIM]-(C2->Size.v[YDIM]/2) 
	&& C1->Position.v[YDIM]-(C1->Size.v[YDIM]/2)<C2->Position.v[YDIM]+(C2->Size.v[YDIM]/2)))*/
	if(xIzqC1 >= xIzqC2 && xIzqC1 <= xDerC2)
		if((yBajC1 >= yBajC2 && yBajC1 <= yArrC2) || (yArrC1 >= yBajC2 && yArrC1 <= yArrC2))
		{
			C1->Collided(C2);
			C2->Collided(C1);
			return true;
		}

	if(xDerC1 >= xIzqC2 && xDerC1 <= xDerC2)
		if((yBajC1 >= yBajC2 && yBajC1 <= yArrC2) || (yArrC1 >= yBajC2 && yArrC1 <= yArrC2))
		{
			C1->Collided(C2);
			C2->Collided(C1);
			return true;
		}
	
	if(xIzqC2 >= xIzqC1 && xIzqC2 <= xDerC1)
		if((yBajC2 >= yBajC1 && yBajC2 <= yArrC1) || (yArrC2 >= yBajC1 && yArrC2 <= yArrC1))
		{
			C1->Collided(C2);
			C2->Collided(C1);
			return true;
		}

	if(xDerC2 >= xIzqC1 && xDerC2 <= xDerC1)
		if((yBajC2 >= yBajC1 && yBajC2 <= yArrC1) || (yArrC2 >= yBajC1 && yArrC2 <= yArrC1))
		{
			C1->Collided(C2);
			C2->Collided(C1);
			return true;
		}

	return false;
#endif
}

#ifdef CHAR_USE_AABB

///Returns the list of objects that has collided with object C. If no one has collided, NULL is returned
///It is asumed that the data structures are updated
bool CCollisionDetection::Collided (CCharacter *C, UGK::SpaceCoords Dim)
{

	bool CollisionDetected = false;
	bool accesible=true;


	unsigned int i, size;

	size = Axis[Dim].size();
	//Detects the current character position inside the Collision Detector structure
	for(i=C->AABB[CHAR_BBMIN][Dim].Position+1;i<size;i++)
	{
		if (Axis[Dim][i]->Character == C)
				return CollisionDetected;	//The Max character's boundary is reached
											//This is the only way to go out from this loop
			else
			{
				CollisionDetected	= true;	//A potential collision has been detected on Dim Axis
				CharactersCollided[CD_CHARACTER_COLLIDED].push_front(Axis[Dim][i]);
				CharactersCollided [CD_CURRENT_CHARACTER].push_front(&C->AABB[CHAR_BBMIN][Dim]);
			}
		
	}
	return CollisionDetected;
}

//Returns the object that has collided with object C. If no one has collided, NULL is returned
//It is asumed that the data structures are updated
bool CCollisionDetection::Collided (CCharacter *C)
{
	deque<CBBPoint*> CollidedCharacters;
	if (Collided (C, XDIM))
		CollidedCharacters = CharactersCollided[CD_CHARACTER_COLLIDED];
	else return false;	//Although it may be collisions on the other dimensions...

	//Detects collision for the rest of the axises
	for (unsigned int Dim = YDIM; Dim<WDIM; Dim++)
	{
		///
		if (Collided(C, (UGK::SpaceCoords) Dim))
		{
			if (false == CheckCollided(CollidedCharacters)) 
				return false;
		}
		else
			//There is no collision with the any character in this given axis.
			//It does not matter if there are collision detected on the other axises
			return false;
	}
	CharactersCollided[CD_CHARACTER_COLLIDED] = CollidedCharacters;
	return true;
}

///Detects if there are characters that intersects both deque parameter and class CharactersCollided deque
bool CCollisionDetection::CheckCollided(deque<CBBPoint*> CollidedCharacters)
{
	
	unsigned int i, j;
	bool Collided = false;

	for (i=0;i<CollidedCharacters.size();i++)
	{
		for (j=0;j<CharactersCollided[CD_CHARACTER_COLLIDED].size();j++)
			if (CollidedCharacters[i]->Character == CharactersCollided[CD_CHARACTER_COLLIDED][j]->Character)
			{
				Collided = true;
				break;	//There is no need to look for more characters
			}
		if (j>CharactersCollided[CD_CHARACTER_COLLIDED].size())
			//No character has been detected, so CollidedCharacters[i] has no collision to current character
			//It has to be removed from CollidedCharacters deque
			CollidedCharacters.erase(CollidedCharacters.begin()+i);
	}	// for CollidedCharacters
	return Collided;
}

#endif

///Frees all deques and extracts all characters from the collision detector
void CCollisionDetection::Reset()
{
#ifdef CHAR_USE_QUADTREE
	QuadtreeRoot::Instance()->getQuadtree()->Clear();
#elif CHAR_USE_AABB
	CCharacter *C;
	unsigned int Size;

	for (	unsigned int Dim = XDIM; Dim < WDIM; Dim++)
	{
		Size = Axis[Dim].size();
		for(unsigned int i = 0;i<Size;i++)
		{
			C = Axis[Dim][0]->Character;
			C->AABB[CHAR_BBMIN][Dim].Position = CHAR_NO_AABB;
			C->AABB[CHAR_BBMAX][Dim].Position = CHAR_NO_AABB;
			Axis[Dim].erase(Axis[Dim].begin());
		}
	}
#endif
}

