/*	Definition of the class Equalizer
    General class to create and to display the equalizer
	Prefix: CEQ_

	@author Ramon Molla
	@author David Calabuig
	@version 2013-05
*/

#ifndef CEQ_EQUALIZER  /// Macro ecualizer is erased
#define CEQ_EQUALIZER  /// Macro ecualizer is defined

#include "Algebra.h"

#define CEQ_MIN_WINDOW_WIDTH	550

/**
	*	\class CEqualizer
	* CEqualizer class is defined to create and display Equalizer
*/
class CEqualizer
{
	//Atributes
public:

	bool	Moving;     ///< It defines the state of character or ship if the enabled or disabled
	Vector	Position,	///< 3D space coordinates and Rotation where the equalizer is NOW
			Rotation;  

	int		FontAlignRight; ///< It defines for the font alignment on the rightside of the game to equilazer
	int		FontAlignUp;    ///< It defines for the font alignment on the up of the game to equilaze

	/// Set and Get of variables: FontAlignRight and FontAlignUp
	inline void SetFontAlignRight(int Size)	{ FontAlignRight = Size;}
	inline void SetFontAlignUp(int Size)	{ FontAlignUp = Size;}

	inline int GetFontAlignRight(void)		{ return FontAlignRight;}
	inline int GetFontAlignUp(void)			{ return FontAlignUp;}

	//Methods
	void Init (void);  //Constructor
	void display(void); ///< Shows the equalizer on the screen

	CEqualizer() {Init();}
	~CEqualizer();	
};

//Definitions for the game


#endif
