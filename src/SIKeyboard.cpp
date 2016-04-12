/** 
*	Class Keyboard

*	Class prefix: CK_

*	@author Ramon Molla
*	@version 2013-10
*/

#include <Global.h>

///Amount of time (ms) a pressed key lasts to send a new command to the game
float	CSIK_ActionsDelay	[CSIK_MAX_KBD_ACTION] = {
	0,		//CSIK_NO_ACTION			No action has been detected. For instance, an incorrect key pressed

	500,		//CSIK_GAME_END				Finishes the game
	200,	//CSIK_GAME_2D_3D			Changes between graphical mode 2D to 3D and viceversa
	0,		//CSIK_GAME_BACKGROUND
	200,	//CSIK_GAME_LEVEL
	200,	//CSIK_GAME_ANTIALIASING	Sets or resets the antialiasing mode
	200,	//CSIK_GAME_VSYNC
	0,		//CSIK_GAME_FULL_SCREEN
	1000,	//CSIK_MODE_GAME			Alternates between continuous mode and discrete mode
	0,		//CSIK_SAVE_GAME			Saves the current state of the game 

	200,	//CSIK_PLAYER_INMORTAL		Player passes from mortal mode to inmortal mode and viceversa
	0,		//CSIK_PLAYER_MOVE_RIGHT	Player ship is move rightwards
	0,		//CSIK_PLAYER_MOVE_LEFT		Player ship is move leftwards
	200,	//CSIK_PLAYER_SHOOT			Laser fire. 10 shoots per second
	100,	//CSIK_PLAYER_LASERS

	0,		//CSIK_CAMERA_INC_FOV	
	0,		//CSIK_CAMERA_DEC_FOV,
	0,		//CSIK_CAMERA_RESHAPE
	500,	//CSIK_CAMERA_ZOOM_IN
	500,	//CSIK_CAMERA_ZOOM_OUT

	200,	//CSIK_TIMER				For monitoring purpouses
	300,	//CSIK_CONSOLE
	300,	//CSIK_CONSOLE_TEXT

	200,	//CSIK_MUSIC_MUTE			Activates or mutes the music during the playing
	200,	//CSIK_NEXT_MUSIC			Changes the music for the next one
	200,	//CSIK_SPECTRUM				Sets or resets the music spectrum

	200,	//CSIK_TEXT2D

	1000,	//CSIK_PLAYER_PAUSE
	200,		//CSIK_HELP

	0,		//CSIK_PLAYER2_MOVE_RIGHT	Player2 ship is move rightwards
	0,		//CSIK_PLAYER2_MOVE_LEFT		Player2 ship is move leftwards
	200,	//CSIK_PLAYER2_SHOOT			Laser fire. 10 shoots per second
};

/*______________________________________________________________________________________

  CONTROLS
  ========
  
	 [LEFT], [RIGHT]       -> Displacements
	 [UP]		           -> Shoots
	 [ESC]                 -> Game Quit
	 [+], [-]              -> Zoom in/out respectively TODO
	 [SPACE]               -> Change music
 	 [F1]                  -> Change gaame from 2D to 3D and viceversa
	 [A]                   -> Anti-aliasing on/off
	 [B]                   -> Change background
	 [L]                   -> Change Level of playing
	 [M]                   -> Music on/off (Mute)
	 [P]                   -> Player pause
	 [R]                   -> Reinitialize camera : angle, zoom, position,...
 	 [V]                   -> Vertical Synchronization on/off
	 (MOUSE)               -> Rotation of the scene

  TODO:wheel,joystick,win-tab key,weapons, block-accel
______________________________________________________________________________________*/

unsigned char	CSIK_ActionKeys	[CSIK_MAX_KBD_ACTION] = {
	NULL,		//CSIK_NO_ACTION			No action has been detected. For instance, an incorrect key pressed

	VK_ESCAPE,	//CSIK_GAME_END				Finishes the game
	VK_F1,		//CSIK_GAME_2D_3D			Changes between graphical mode 2D to 3D and viceversa
	'B',		//CSIK_GAME_BACKGROUND
	'L',		//CSIK_GAME_LEVEL
	'A',		//CSIK_GAME_ANTIALIASING	Sets or resets the antialiasing mode
	'V',		//CSIK_GAME_VSYNC
	VK_F12,		//CSIK_GAME_FULL_SCREEN
	'C',		//CSIK_MODE_GAME			Alternates between continuous mode and discrete mode
	'S',		//CSIK_SAVE_GAME			Saves the current state of the game 

	VK_F5,		//CSIK_PLAYER_INMORTAL		Player passes from mortal mode to inmortal mode and viceversa
	VK_RIGHT,	//CSIK_PLAYER_MOVE_RIGHT	Player ship is move rightwards
	VK_LEFT,	//CSIK_PLAYER_MOVE_LEFT		Player ship is move leftwards
	VK_UP,		//CSIK_PLAYER_SHOOT			Laser fire
	VK_F3,		//CSIK_PLAYER_LASERS

	'W',		//CSIK_CAMERA_INC_FOV	
	'X',		//CSIK_CAMERA_DEC_FOV,
	'R',		//CSIK_CAMERA_RESHAPE
	VK_ADD,		//CSIK_CAMERA_ZOOM_IN
	VK_SUBTRACT,//CSIK_CAMERA_ZOOM_OUT

	VK_F4,		//CSIK_TIMER				For monitoring purpouses
	VK_OEM_7,	//CSIK_CONSOLE
	0,			//CSIK_CONSOLE_TEXT			Virtual event

	'M',		//CSIK_MUSIC_MUTE			Activates or mutes the music during the playing
	VK_SPACE,	//CSIK_NEXT_MUSIC			Changes the music for the next one
	VK_F2,		//CSIK_SPECTRUM				Sets or resets the music spectrum
	
	'T',		//CSIK_TEXT2D

	'P',		//CSIK_PLAYER_PAUSE
	'H',			//CSIK_HELP
	'I',		//Plyaer 2 right
	'Y',		//player2 left
	'7'			//player2 shoot
};

/**
   * @fn CKeyboard::KeyReleased(const CK_TOKEN Token)
   * Set the actions associated to a released token
   * @Param char Token to exclude in the list of active tokens
*/
void CSIKeyboard::Update ()	///<Release a new key from the active list of key pressed
{
	for (CK_ACTIVE_TOKENS::iterator it = ActiveTokens.begin(); it != ActiveTokens.end(); ++it)
		switch(*it)
		{case CSIK_PLAYER_SHOOT:
			if (HRTimerManager.Timers[CSIK_PLAYER_SHOOT].IsSounding())
			{
				HRTimerManager.Timers[CSIK_PLAYER_SHOOT].ResetAlarm();
				Player[CurrentPlayer].ManageEvent(CP_SHOOT);
			}
			break;
		case CSIK_PLAYER_MOVE_LEFT:
			if (HRTimerManager.Timers[CSIK_PLAYER_MOVE_LEFT].IsSounding())
			{
				HRTimerManager.Timers[CSIK_PLAYER_MOVE_LEFT].ResetAlarm();
				Player[CurrentPlayer].ManageEvent(CP_MOVE_LEFT);
			}
			break;
		case CSIK_PLAYER_MOVE_RIGHT:
			if (HRTimerManager.Timers[CSIK_PLAYER_MOVE_RIGHT].IsSounding())
			{
				HRTimerManager.Timers[CSIK_PLAYER_MOVE_RIGHT].ResetAlarm();
				Player[CurrentPlayer].ManageEvent(CP_MOVE_RIGHT);
			}
			break;
		case CSIK_PLAYER2_SHOOT:
			if (HRTimerManager.Timers[CSIK_PLAYER2_SHOOT].IsSounding())
			{
				HRTimerManager.Timers[CSIK_PLAYER2_SHOOT].ResetAlarm();
				Player2[CurrentPlayer].ManageEvent(CP_SHOOT);
			}
			break;
		case CSIK_PLAYER2_MOVE_LEFT:
			if (HRTimerManager.Timers[CSIK_PLAYER2_MOVE_LEFT].IsSounding())
			{
				HRTimerManager.Timers[CSIK_PLAYER2_MOVE_LEFT].ResetAlarm();
				Player2[CurrentPlayer].ManageEvent(CP_MOVE_LEFT);
			}
			break;
		case CSIK_PLAYER2_MOVE_RIGHT:
			if (HRTimerManager.Timers[CSIK_PLAYER2_MOVE_RIGHT].IsSounding())
			{
				HRTimerManager.Timers[CSIK_PLAYER2_MOVE_RIGHT].ResetAlarm();
				Player2[CurrentPlayer].ManageEvent(CP_MOVE_RIGHT);
			}
			break;

		case CSIK_GAME_END:
			Game.GameEvent(CSIG_FADING2END);		//v Controlado 4->12
			break;
		case CSIK_SAVE_GAME:
			Game.GameEvent(CSIG_SAVE);				//v Controlado 4->5
			break;
		case CSIK_PLAYER_PAUSE:
			Game.Paused = !Game.Paused;
			break;
		case CSIK_MODE_GAME:
			if (HRTimerManager.Timers[CSIK_MODE_GAME].IsSounding())
			{
				HRTimerManager.Timers[CSIK_MODE_GAME].ResetAlarm();

				DiscreteSimulation = !DiscreteSimulation;

				for(unsigned int i=0; i<CBN_MAX_BONUSES; i++)
					DiscreteBonusManager.Bonus[i].Active = DiscreteBonusManager.Bonus[i].Alive = false;

				std::list<CShoot>::iterator Shoot;
				Shoot = ShootsManager.ShootsAct.begin();
				while (Shoot != ShootsManager.ShootsAct.end())
				{
					if (Shoot->Active) Shoot->Active=false;
					Shoot++;
				}
				if(DiscreteSimulation)
					Sync2RT = true;
			}
			break;
		case CSIK_CAMERA_ZOOM_IN:
			if (HRTimerManager.Timers[CSIK_CAMERA_ZOOM_IN].IsSounding())
			{
				Camera.zoom =  0.02f * Game.RenderTemp;
				Camera.zoom_total += Camera.zoom;
			}
			break;
		case CSIK_CAMERA_ZOOM_OUT:
			if (HRTimerManager.Timers[CSIK_CAMERA_ZOOM_IN].IsSounding())
			{
				Camera.zoom =  -0.02f * Game.RenderTemp;
				Camera.zoom_total += Camera.zoom;
			}
			break;
		}
}