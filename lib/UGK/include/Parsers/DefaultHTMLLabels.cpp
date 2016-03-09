/*	Class that loads the values of the default labels for all parsers in the game 

	Prefix DHTML_: Default HTML Labels for all Parsers

	@author Ramón Mollá
	@version 2014-10*/

#include <DefaultHTMLLAbels.h>
#include <UGKString.h>

/**   
   It defines the default lexemas of labels that can match the scanner during the scan that exists in the game
*/
DHTML_TagsArrayType DHTML_TagsArray ={
	UGKS_EMPTY_STRING,	//This tag is for management purpouses only. There is no correspondence to any string label in the HTML file
	"BODY",				//HTML tag
	"HEAD",				//HTML tag
	"HTML",				//HTML tag
	"TITLE",			//HTML tag
	"TYPE",
	"VERSION"
};
