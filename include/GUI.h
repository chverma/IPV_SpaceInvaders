/*	Definition of the class Interface

	Prefix: CGUI_

	@author Ramon Molla
	@version 2011-11
*/
#include <Boometer.h>

#ifndef CGUI_INTERFACE //Se borrar la macros interface
#define CGUI_INTERFACE //Se define la macros interface

#define CGUI_MAX_COMMAND 2
#define CGUI_MAX_CONSOLE_LENGTH	40
#define CGUI_MAX_CONSOLE_LINES	50
#define CGUI_MAX_CONSOLE_COLS	50

#define CGUI_MAX_CONSOLE_MSG_COLS 20
#define CGUI_MSG_MAX_LONG 64	//Maximun length of a msg

typedef enum {
	CGUI_INVALID_COMMAND,
	CGUI_TYPE_VBLE_NAME,
	CIP_AVERAGE_ERROR,
	CGUI_MAX_MSGS
} CGUI_LogMsg;

// CONSOLE Class
class CConsole
{
public:

	bool	Active;		// Is Active
	bool	ComeIn;		// On screen
	bool	Moving;		// Is moving
	bool	Start;		// Is starting

	char	Text[CGUI_MAX_CONSOLE_LINES][CGUI_MAX_CONSOLE_COLS];	// CONSOLE TEXT
	int		Line;			// CONSOLE LINE INDEX FOR CONSOLE_TEXT[][]

	char	Command			[CGUI_MAX_COMMAND][CGUI_MAX_CONSOLE_LENGTH];	// CONSOLE 1ERE COMMANDE
	char	Output			[CGUI_MAX_COMMAND][CGUI_MAX_CONSOLE_LENGTH];	// CONSOLE 1ERE SORTIE
	float	ViewportPush;	// CONSOLE VIEWPORT MINI-SCREEN

	CConsole();
};


/**   
   * @class cInterface
 
	* This class defines the Graphical User Interface for the game states, where is the constructor
    * Define the GUI to display the screens of the game such as pause, the player's score, lives, buttons
	
	@author Ramon Molla
	@version 2011-11
*/
class CGUI
{
	
public:

	//Attributes
	//Interface Fading percentage
	float		Fading;
	CConsole	Console;

	//Constructor
	CGUI() {Init();}

	//Methods
	void Init ();

	///Shows the user interface on the screen
	void Render(void);

	void display_ring_2d_to_3d			();					//mostrar conversion de 2D a #D
	void display_score_hiscore_lives	();					//Mostrar score de jugadors
	void display_below_lives			();					//mostrar estado de vida
	void display_background				(bool viewport2);	//mostrar fondo de pantalla
	void display_cube					(int texture_id, float size); //mostrar cuadrado
	void display_console				();					//mostrar la pantalla de consola
	void display_console_mini_screen	();					//Mostrar pequeña pantalla de consola
	void display_text_frames			();					//mostrar marcos de texto 
	void display_Health					();					//Mostrar salud de nave
	void display_spectrum				();					//mostrar spectrum
	

	void display_splashscreen	(); //pantalla de inicio
	void DisplayPressKey		(); //mostrar botones
	void DisplayPaused			(); //mostrar pausa

	void FadingInAnimation	(void);		///< Animation while Fading in the SI logo
	void FadingInWon		(void);		///< Animation while Fading in the Score
	void FadingOutLoading	(void);		///< Animation while Fading out the SI logo

private:
	CBoometer *boometer;

};

#endif
