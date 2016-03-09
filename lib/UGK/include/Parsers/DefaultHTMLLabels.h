/*	Class that loads the values of the default labels for all parsers in the game 

	Prefix DHTML_: Default HTML Labels for all Parsers

	@author Ramón Mollá
	@version 2014-10*/

#ifndef DHTML_INITIALIZED //Se borra la macro inicializacion
#define DHTML_INITIALIZED //Se define la macro inicializacion


#define DHTML_TAG_MAX_LONG 64	//Maximun length of a sensitive tag

/**   
   It defines the default types of labels that can match the scanner during the scan that exist in the game
*/
typedef enum {
	NIL_D,		//This tag is for management purpouses only. There is no correspondence to any string label in the HTML file
	BODY_D,		//HTML tag
	HEAD_D,		//HTML tag
	HTML_D,		//HTML tag
	TITLE_D,	//HTML tag
	TYPE_D,
	VERSION_D,
	MAXTAGS_D		//This tag is for management purpouses only. There is no correspondence to any string label in the HTML file
} DHTML_TagType;

typedef char DHTML_TagsArrayType [MAXTAGS_D][DHTML_TAG_MAX_LONG];
extern		 DHTML_TagsArrayType DHTML_TagsArray;

#endif
