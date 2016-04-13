// SI_2013.cpp: define el punto de entrada de la aplicación.
//

#include "Main_SI_2013.h"
#include <iostream>
#include <stdlib.h>
#include <GLWindow.h>
#include <Wininet.h>		// Header File For Internet Library
#include <string>
#include <cstring>
#include <Global.h>			// Header File For Main Global Variables
#include <TextResources.h>	// Header File For the internationalization of the text resources 
#include <UGKString.h>

#define WM_TOGGLEFULLSCREEN (WM_USER+1)		// Application Message For Toggling Fullscreen
#define MAX_LOADSTRING 100

typedef enum
{
	APP_STOP,
	APP_INIT,
	APP_CREATE,
	APP_START,
	APP_PAUSE,
	APP_END,
	APP_MAX_STATES
} MAIN_APP_STATES;

MAIN_APP_STATES AppControl = APP_STOP;
int initialWidthW=640;
int initialHeightW=480;
// THREADS
unsigned long	SIDLG_h_thread;	// Handle Of The Loading Thread

// Variables globales:
char *aux;
stringstream	ss;
string			strg;
HANDLE			hFind = INVALID_HANDLE_VALUE;

ofstream fout;

extern cFpsGL info;

// Declaraciones de funciones adelantadas incluidas en este módulo de código:
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void GenerateFullLangName (std::string DefaultLang, char * Name)
{
	DefaultLang.copy(UGKOS_THIS_DIRECTORY, 1, 0);
	DefaultLang.append(UGKOS_DIRECTORY_SEPARATOR);
	DefaultLang.append(Application.LanguageFolder);
	DefaultLang.append(UGKOS_DIRECTORY_SEPARATOR);
	DefaultLang.append(Name);
		
}


DWORD  SetUpHCI () 
{
	HMENU hMenubar	= CreateMenu();
	HMENU hGame		= CreateMenu();
	HMENU hLoad		= CreateMenu();
	HMENU hHelp		= CreateMenu();
	HMENU idioma	= CreateMenu();
	HMENU maxMinWin = CreateMenu();

	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hGame, LPCSTR("Game")); //"Space Invaders"
	
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hLoad, LPCSTR("Load Level"));
	
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hHelp, LPCSTR("Help"));	//"Help"

	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)idioma, LPCSTR("Language"));
	AppendMenu(hGame, MF_STRING, ID_Start, LPCSTR("Start game"));	//"Start Game"
	AppendMenu(hGame, MF_STRING, ID_FULLSCREEN, LPCSTR("Max/Min Window"));
	AppendMenu(hGame, MF_STRING, ID_Exit, LPCSTR("Exit"));

	AppendMenu(idioma, MF_STRING, ID_LANGUAGE_FR, LPCSTR("French"));	//French
	AppendMenu(idioma, MF_STRING, ID_LANGUAGE_ES, LPCSTR("Spanish"));	//Spanish
	AppendMenu(idioma, MF_STRING, ID_LANGUAGE_EN, LPCSTR("English"));	//English
	AppendMenu(idioma, MF_STRING, ID_LANGUAGE_CA, LPCSTR("Catalan"));	//Catalonian

	//Reading possible levels in .\Levels\UserLevels
	TCHAR szDir[MAX_PATH];
	WIN32_FIND_DATA ffd;

	strcpy((char *) szDir,CSIG_GAME_ULEVEL_FILE);
	strcat((char *) szDir,"*.html");
	hFind = FindFirstFile(szDir, &ffd);
	do
	{
		ss.str(UGKS_EMPTY_STRING); 
		ss << ffd.cFileName;
		strg = ss.str();
		aux = new char[strg.length()+1];
		strcpy(aux,strg.c_str());
		Game.HtmlFiles.push_back(aux);
	}
	while (FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);
    
	if(Game.HtmlFiles.size() > MAX_LEVELS_HTML)
		for(int i=0; i!=MAX_LEVELS_HTML; i++){
			AppendMenu(hLoad, MF_BYPOSITION, i, Game.HtmlFiles[i]);
		}
	else
		for(int i=0; i!=Game.HtmlFiles.size(); i++){
			AppendMenu(hLoad, MF_BYPOSITION, i, Game.HtmlFiles[i]);
		}
                
	AppendMenu(hHelp, MF_STRING, IDM_ABOUT, "About...");
	
	SetMenu(Application.Window.hWnd, hMenubar);

	Application.Window.Show();

	return NULL;
}

#ifdef	UGKOS_WINDOWS
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
#elif	defined(UGKOS_LINUX)
#elif	defined(UGKOS_OSX)
#elif	defined(UGKOS_ANDROID)
#endif
	
	char	temp[50], GameTitle[MAX_LOADSTRING];
	bool	firstTime=true;
	BOOL	SIDLG_MainProgramLooping;

	// Fill Out Application Data
	Application.Name = new char[sizeof(CSIG_TITTLE)];
	strcpy(Application.Name, CSIG_TITTLE);	// Application Class Name
	Application.hInstance = hInstance;		// // Saves the Application Instance instance id into the corresponding attribute in the application singleton

	// Fill Out Window Data
	strcpy(GameTitle, Game.GetTitle());

	//Update Application Window attributes
	Application.Window.setWindowMetadata(GameTitle, strlen(GameTitle),initialWidthW,initialHeightW,16,true,true);
	Application.Window.Name		=	Application.Name;
	Application.Window.hInstance=	Application.hInstance;
	Application.Window.SetId(IDI_SI_2013, IDI_SMALL, IDS_APP_TITLE);
	Application.Window.SetWndProc(WndProc);
	Application.Window.SetCmdShow(nCmdShow);

	//Initializing application general parameters
	if (!Game.Initialize())
	{// Main Game initialization failure
		Application.Terminate();
		AppControl = APP_END;
	}

	SetUpHCI();

	//**************************************************************//

	// Bucle principal de mensajes:	
	while (GetMessage(&msg, NULL, 0, 0))									// Loop Until WM_QUIT Is Received
	{	
		//AFD that represents the behavior of the main Application
		switch (AppControl)
		{
		case APP_INIT:
				//// Register A Class For Our Window To Use
				//if (FALSE == Application.Window.Register())	// Did Registering A Class Fail?
				//{
				//	// Failure
				//	MessageBox(HWND_DESKTOP, "Error Registering Window Class!", "Error", MB_OK | MB_ICONEXCLAMATION);
				//	return -1;								// Terminate Application
				//}

				// Load Device for DialogBox
				Game.LoadDevice();

				if (CP_NO_PLAYER == CurrentPlayer)
				{
					///If no player initialization during device loading, a explicit one has to be performed
					CurrentPlayer = CP_DEFAULT_PLAYER;
					Player[CurrentPlayer].Init();
				}

				strcpy(temp, Game.GetTitle());
				strcat(temp, " [Start]");
				OutputDebugString(LPCSTR(temp));

				// Get the resolution for "Quick FullScreen" mode
				if (Application.Window.quickFullScreen == TRUE && Application.Window.FullScreen == FALSE)// If Quick Fullscreen, we catch the screen resolution
				{
					Application.Window.Width	= GraphicsCard.DeskTop.Width;
					Application.Window.Height	= GraphicsCard.DeskTop.Height;
				}

				AppControl = APP_CREATE;	
			break;
		case APP_CREATE:
				if (Application.Window.IsCreated())		// Was Window Creation Successful?	
				{	// At This Point We Should Have A Window That Is Setup To Render OpenGL
					// Start The Message Pump. Initialization was a success
					SIDLG_MainProgramLooping = TRUE;
					Game.AI->SetState(CSIG_INITIAL);
					AppControl = APP_START;
				}
				else									// If Window Creation Failed
				{
					// Error Creating Window
					MessageBox(HWND_DESKTOP, SIGLB_GetText(TR_BASECODE_ERROR_CREATING_OPENGL), "Error", MB_OK | MB_ICONEXCLAMATION);
					AppControl = APP_STOP;
				}
			break;
		case APP_START:
				while (SIDLG_MainProgramLooping && !Game.StopAfterOneLevel)								// While The Message Pump Is Active
				{
					if(Application.Update() == 0){
						SIDLG_MainProgramLooping = FALSE;
						//Game.AI->SetState(CSIG_FADING_OUT2END);
					}
					if(Game.Paused){		
						AppControl = APP_PAUSE;
						break;
					}
					#ifdef DEF_EXP_CSV_FILE
					switch (WayToEnd)
					{
					case END_BY_TIME:
						if(DEF_MAX_TICKS < Game.TimerManager.Timers[TmrApp].TicksUntilNow()){
							Game.Snapshot(".\\Resultados\\Snapshot.bmp");
							endByTime = true;
							SIDLG_MainProgramLooping = FALSE;
						}
						break;
					case END_BY_FRAME:
						if(info.AllnFrames >= DEF_END_IN_FRAME){
							Game.Snapshot(".\\Resultados\\Snapshot.bmp");
							endByFrame = true;
							SIDLG_MainProgramLooping = FALSE;
						}
						break;
					}
					#endif

					Game.RunMainLoop();	
				}
				if(!Game.Paused)
					AppControl = APP_END;
			break;
		
		case APP_END:
			#ifdef DEF_EXP_CSV_FILE
			if(endByTime || endByFrame){			
				EndAccCounting(TmrIdl);

				double msTotApl = Game.TimerManager.Timers[TmrApp].GetLastPeriodms();
				double medFps = info.fpsAcum/info.fpsContTotal;

				ss.str(UGKS_EMPTY_STRING);
				ss << msTotApl <<";"<< msTotRnd <<";"<< msTotUpd <<";"<< msTotIdl <<";"<< msTotRTDSKMM <<";"<< (msTotIdl*100)/msTotApl <<";"<< medFps <<";"<< info.AllnFrames <<UGKS_NEW_LINE_STRING;

				if(DiscreteSimulation) fout.open(".\\Resultados\\ResumenD.csv", std::ios_base::app);
				else fout.open(".\\Resultados\\ResumenC.csv", std::ios_base::app);
				fout << ss.str() << std::endl;
				fout.flush();
				fout.close();

				msTotRnd=0;
				msTotUpd=0;
				msTotIdl=0;
				msTotRTDSKMM=0;
				info.Reset();
				initReg=false;

				#ifdef DEF_LEC_PARTIAL
				FILE *fsalida;
				fsalida = fopen("DLecRnd.csv", "a+");
				for(int lec=0; lec<indTotRnd; lec++){
					fprintf(fsalida, "%d;%.6f\n", lec, muestrasTotRnd[lec]);
				}
				fclose(fsalida);

				fsalida = fopen("DLecUpd.csv", "a+");
				for(int lec=0; lec<indTotUpd; lec++){
					fprintf(fsalida, "%d;%.6f\n", lec, muestrasTotUpd[lec]);
				}
				fclose(fsalida);

				fsalida = fopen("DLecIdl.csv", "a+");
				for(int lec=0; lec<indTotIdl; lec++){
					fprintf(fsalida, "%d;%.6f\n", lec, muestrasTotIdl[lec]);
				}
				fclose(fsalida);

				fsalida = fopen("DLecRtd.csv", "a+");
				for(int lec=0; lec<indTotRtd; lec++){
					fprintf(fsalida, "%d;%.6f\n", lec, muestrasTotRtd[lec]);
				}
				fclose(fsalida);
				#endif
			}
			#endif

			// Application Is Finished
			Game.ResetObjects();
			Game.Deinitialize();											// User Defined DeInitialization

			#ifdef CHW_USE_KINECT
			CleanupExit();													// Clean kinect stuff
			#elif  CHW_USE_WIIMOTE
			//Disconnect the wiimotes
			wiiuse_cleanup(wiimotes, CWM_MAX_WIIMOTES);
			#endif
			Application.Window.Destroy();									///Destroy The Active Window
			UnregisterClass(LPCSTR(Application.Name), Application.hInstance);		// UnRegister Window Class
			AppControl = APP_STOP;
			break;
		case APP_PAUSE:
			SIKeyboard.Update();
			if(!Game.Paused)
				AppControl = APP_START;		
		case APP_STOP:		
		default:
			if (!Application.Window.TranslateAcc(&msg))
			{
				if (msg.message != WM_QUIT){
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			break;
		}
	}
	return (int) msg.wParam;
}

//
//  FUNCIÓN: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PROPÓSITO: procesar mensajes de la ventana principal.
//
//  WM_COMMAND	: procesar el menú de aplicación
//  WM_PAINT	: pintar la ventana principal
//  WM_DESTROY	: enviar un mensaje de finalización y volver
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:

		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Menu selections analize:
		//All options between 0 <-> 50 are reserved to load individual levels.
		if(wmId>=MIN_LEVELS_HTML && wmId < MAX_LEVELS_HTML){
			Game.NextHtmlInd2Load = wmId;
			Game.OnlyOneLevel = true;
			AppControl = APP_INIT;
		}
		else{
			std::string SourceLang, DefaultLang;
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(Application.hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case ID_Exit:
				DestroyWindow(hWnd);
				exit(EXIT_SUCCESS);
				break;
			case ID_Start:
				Game.OnlyOneLevel = false;
				AppControl = APP_INIT;
				break;
			case ID_LANGUAGE_FR:
				GenerateFullLangName(SourceLang, "FR.html");
				GenerateFullLangName(DefaultLang, CA_DEFAULT_LANGUAGE_FILE);
				CopyFile(SourceLang.c_str(),DefaultLang.c_str(),FALSE); 
				break;
			case ID_LANGUAGE_ES:
				GenerateFullLangName(SourceLang, "ES.html");
				GenerateFullLangName(DefaultLang, CA_DEFAULT_LANGUAGE_FILE);
				CopyFile(SourceLang.c_str(),DefaultLang.c_str(),FALSE);				  
				break;
			case ID_LANGUAGE_EN:
				GenerateFullLangName(SourceLang, "EN.html");
				GenerateFullLangName(DefaultLang, CA_DEFAULT_LANGUAGE_FILE);
				CopyFile(SourceLang.c_str(),DefaultLang.c_str(),FALSE);				  
				break;
			case ID_LANGUAGE_CA:
				GenerateFullLangName(SourceLang, "CA.html");
				GenerateFullLangName(DefaultLang, CA_DEFAULT_LANGUAGE_FILE);
				CopyFile(SourceLang.c_str(),DefaultLang.c_str(),FALSE);				  
				break;
			case ID_FULLSCREEN:									// Toggle FullScreen Mode On/Off
				Application.Window.Visible = TRUE;									// Set Visible To True
				Application.Window.Width	= GraphicsCard.DeskTop.Width;
				Application.Window.Height	= GraphicsCard.DeskTop.Height;
				Game.ReshapeGL();				
				break;
			case ID_MINIMIZE:									// Toggle FullScreen Mode On/Off
				Application.Window.Visible = FALSE;									// Set Visible To True
				Game.ReshapeGL();				
				break;		
			}
		}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: agregar código de dibujo aquí...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	////////////////////////////////////////////////////////////////////////////////
	case WM_CREATE:													// Window Creation
		{
		CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);		// Store Window Structure Pointer
		// Get The OS Window Context
		LPVOID OSWindow = creation->lpCreateParams;
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)(OSWindow));
		}
		return 0;													// Return
	case WM_CLOSE:													// Closing The Window
		Application.Terminate();									// Terminate The Application
		AppControl = APP_END;
		return 0;													// Return

	case WM_ENTERSIZEMOVE:
		if (CSIG_INITIAL == Game.AI->GetState())											// pendant le chargement on baisse la priorité
			SetThreadPriority((void*)SIDLG_h_thread, THREAD_PRIORITY_LOWEST);
		return 0;
	case WM_EXITSIZEMOVE:
		{		
			SetThreadPriority((void*)SIDLG_h_thread, THREAD_PRIORITY_NORMAL );
		}
		return 0;

	case WM_ERASEBKGND:
		return true;

	case WM_SIZE:													// Size Action Has Taken Place
		switch (wParam)												// Evaluate Size Action
		{
		case SIZE_MINIMIZED:										// Was Window Minimized?
			//BOX("minimized")
			Application.Window.Visible = FALSE;								// Set Visible To False
			return 0;												// Return

		case SIZE_MAXIMIZED:										// Was Window Maximized?
			Application.Window.Visible = TRUE;									// Set Visible To True
			Application.Window.Width = LOWORD(lParam);
			Application.Window.Height = HIWORD(lParam);
			Game.ReshapeGL();											// Reshape Window - LoWord=Width, HiWord=Height
			return 0;												// Return

		case SIZE_RESTORED:											// Was Window Restored?
			Application.Window.Visible = TRUE;									// Set Visible To True
			Application.Window.Width = LOWORD(lParam);
			Application.Window.Height = HIWORD(lParam);
			Game.ReshapeGL();							// Reshape Window - LoWord=Width, HiWord=Height
			return 0;									// Return
		}
		break;											// Break
	case WM_KEYDOWN:									// Update Keyboard Buffers For Keys Pressed
		if ((wParam >= 0) && (wParam < CK_MAX_TOKENS))	// Is Key (wParam) In A Valid Range?
		{
			SIKeyboard.KeyPressed(wParam);				// Set The Selected Key (wParam) To True
			
			return 0;									// Return
		}
		break;											// Break

	case WM_KEYUP:										// Update Keyboard Buffers For Keys Released
		if ((wParam >= 0) && (wParam < CK_MAX_TOKENS))	// Is Key (wParam) In A Valid Range?
		{
			SIKeyboard.KeyReleased(wParam);

			return 0;									// Return
		}
		break;											// Break

	case WM_LBUTTONDOWN:
		{
			Scene.Moving = true;						// We are moving the scene
			Scene.Start.v[XDIM] = LOWORD(lParam);		// mouse x
			Scene.Start.v[YDIM] = HIWORD(lParam);		// mouse y

			// Resolution width
			int w = Application.Window.Width;

			// paliers de résolutions
			int w2;
			if (w >= 1600) w2 = 1600; else if (w >= 1280) w2 = 1280; else if (w >= 1024) w2 = 1024;	else if (w >= 800) w2 = 800; else w2 = 640;
		
		}
		break;

	case WM_LBUTTONUP:
		{
			Scene.Moving		= false;							// Stop moving the scene
		}
		break;

	case WM_MOUSEMOVE:
		{
		// Resolution width
		int w = Application.Window.Width;

		// paliers de résolutions
		int w2;
		if (w >= 1600) w2 = 1600; else if (w >= 1280) w2 = 1280; else if (w >= 1024) w2 = 1024;	else if (w >= 800) w2 = 800; else w2 = 640;
		
		//// rotation de la scène (bouton enfonçé mais pas sur l'équalizer)
		if (Scene.Moving)
		{
			Scene.Angle.v[XDIM] += (LOWORD(lParam) - Scene.Start.v[XDIM]) / 2;			// souris axe x (/2 pour diminuer la vitesse)
			Scene.Angle.v[YDIM] += (HIWORD(lParam) - Scene.Start.v[YDIM]) / 2;			// souris axe y (/2 pour diminuer la vitesse)
		}
		Scene.Start.v[XDIM] = LOWORD(lParam);								// mouse x
		Scene.Start.v[YDIM] = HIWORD(lParam);								// mouse y
		}
		break;

	case WM_TOGGLEFULLSCREEN:									// Toggle FullScreen Mode On/Off
		Application.Window.FullScreen = !Application.Window.FullScreen;
		PostMessage(hWnd, WM_QUIT, 0, 0);
		break;													// Break
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Controlador de mensajes del cuadro Acerca de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
