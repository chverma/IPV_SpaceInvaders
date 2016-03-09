/**	Class that generates the amount of characters required by the game

	Prefix CFC_: Class Factory of Characters

	@author Ramon Molla
	@version 2015-01
*/
#ifndef CHARACTERS  /// Macro Characters is erased
#define CHARACTERS  /// Macro Characters is defined

//#include "Copyright.h"
#include <Character.h>	/// Header File for the basic characters used in a given game
#include <map>
#include <atlstr.h>

#define CFC_CharacterIdType unsigned int

class CCFC_Character
{
public:
	CString				Name;	///< Label that identifies the object
	CFC_CharacterIdType	Id;		///< Identity number that identifies the object
	unsigned int		Size;	///< Amount of bytes of memory required by the given character
};

typedef map<CString,CCFC_Character> CFC_CharacterDict;

class CCharacterFactory
{
	CFC_CharacterDict Characters;

public:

	//Void constructor
	CCharacterFactory(){};

	/**
	*	\fn GCHARS_CharacterType CCharacterFactory::String2Tag (const char* Name) 
	*	Converts a string into a tag indicating the type of character it means
	*	\param[in] Name The lexema that identifies a character
	*	returns The token that identifies the character name
	*/
	inline CFC_CharacterIdType String2Tag (const CString Name){return Characters[Name].Id;}

	/**
	*	\fn GCHARS_CharacterType CCharacterFactory::String2Tag (const char* Name) 
	*	Converts a string into a tag indicating the type of character it means
	*	\param[in] Name The lexema that identifies a character
	*	returns The token that identifies the character name
	*/
	inline void SetCharacter (const CCFC_Character Character){Characters[Character.Name] = Character;}

	///Generates a pointer to the array of characters requested
	void		*Create (CFC_CharacterIdType Type, unsigned int Amount);
	inline void	*Create (CString Name, unsigned int Amount)	{return malloc(Characters[Name].Size*Amount);}
};

#endif
