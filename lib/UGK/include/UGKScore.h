/**	Definition of the class Score

*Prefix: UGKCSC_

*	@author Ramon Molla
*	@author Nuria Martinez
*	@version 2012-06
*/

#ifndef CSC_SCORE
#define CSC_SCORE

#include <UGK.h>
#include <string.h>
#define CSC_MAX_NAME_SCORE 10

//#include "Copyright.h"

namespace UGK
{
	class UGK_API CScore
	{
		//Atributes
		char name[CSC_MAX_NAME_SCORE]; ///Player name
		int points;	///Player score points
	
	public:

		//Methods
		void Init();
    
		///Constructor of the class
		CScore	(){Init();};

		///Destructor of the class
		~CScore	();

		///Returns the name of player
		inline char* GetName() { return name; }

		///Returns the points of score
		inline int GetPoints() {  return points; }

		///Returns the name of the player score
		inline void SetName(char *n) { strcpy(name, n); }

		///Sets the points of score
		inline void SetPoints(int p) { points = p; }	
	};
}	//Namespace

#endif