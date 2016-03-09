/*	Definition of the class Laguage Manager
  General class to manage all the languages supported by the application

	Prefix: CLM_
	@author Ramon Molla
	@version 2011-11
*/

#include "LanguageManager.h"
#include "LanguageParser.h"
#include "Global.h"
#include "Game.h"

extern CSIGame Game;

///Translation table
///Converts every resource into the external code that will be seen at the language.html file
unsigned int TR_Resources_ID [TR_MAX_TEXT_RESOURCES] = {
	0001,
	0002,
	0010,
	1000	///<Internet Error. We cannot reach internet access
};

CLanguageManager::CLanguageManager()
{
	TextDefaultDir = "";

	for (unsigned int i = 0; i<TR_MAX_TEXT_RESOURCES;i++)
		TextResource[i] = "";	//Set all the strings to no string

	for (unsigned int i = 0; i<TR_MAX_TEXT_RESOURCES2;i++)
		TextResource2[i] = "";	//Set all the strings to no string

	LanguageReady=false;

	Game.InternationalizationFixed=false;

	if(Game.InternationalizationFixed){
	InitializeLoad();
	}
}

void CLanguageManager::Initialize(const char *file)
{
	int msgBox;
	char languageDefaultFile	[TR_MAX_LOG_NAME],
		languageLog				[TR_MAX_LOG_NAME];

	strcpy(languageDefaultFile,"");
	strcpy(languageLog,"");

	strcpy(languageDefaultFile, Game.Directories[CG_LANGUAGE_DIR]);
	strcpy(languageLog, Game.Directories[CG_LANGUAGE_DIR]);	

	strcpy(languageDefaultFile, "\\languages\\default.html");	
	strcpy(languageLog, "\\Log.txt");

	//HTML Reader
	CFile			HTMLFile;		//File to parser in the initialization
	CLanguageReader	LanguageReader;	//The parser used to understand the contents of the language file
	CLiteHTMLReader	HTMLReader;		//The real HTML reader

	//Initialization load

	LanguageReader.Init(languageLog);
	HTMLReader.setEventHandler(&LanguageReader);//Assigning the initialization parser to the generic reader

	if (HTMLFile.Open(_T(file), CFile::modeRead))
		
	{
		LanguageReady=true;
		msgBox = MessageBox(NULL,
			TEXT(file),
			TEXT("Ok:Leyendo el fichero de idioma indicado"),
			MB_ICONASTERISK | MB_OKCANCEL );
		char c = (char) HTMLReader.ReadFile((HANDLE) HTMLFile.m_hFile);
		HTMLFile.Close();

	}else
		if (HTMLFile.Open(_T(languageDefaultFile), CFile::modeRead))
		{
			LanguageReady=true;
			msgBox = MessageBox(NULL,
				TEXT(languageDefaultFile),
				TEXT("Error:Leyendo el fichero de idioma por defecto al no encontrar el indicado"),
				MB_ICONASTERISK | MB_OKCANCEL );
			HTMLReader.ReadFile((HANDLE) HTMLFile.m_hFile);
			HTMLFile.Close();
		}

}


/********************************************//**
 *  This method is used at game startup
 *  Game load the default language in order to show
 *  the startup dialog
 ***********************************************/
void CLanguageManager::InitializeLoad()
{
	// This method access to the default language document
	// in order to get default language. So the startup dialog 
	// will be load using default language

	//strcpy(Game.LanguageName, "ES");


	int msgBox;
	char languageDefaultFile	[TR_MAX_LOG_NAME],
		languageLog			[TR_MAX_LOG_NAME];

	strcpy(languageDefaultFile,"");
	strcpy(languageLog,"");

	//strcpy(languageDefaultFile, Game.Directories[CG_LANGUAGE_DIR]);	
	//strcpy(languageLog, Game.Directories[CG_LANGUAGE_DIR]);

	strcpy(languageDefaultFile, "\\languages\\default.html");	
	strcpy(languageLog, "\\Log.txt");	

	//HTML Reader
	CFile			HTMLFile;		//File to parser in the initialization
	CLanguageReader	LanguageReader;	//The parser used to understand the contents of the language file
	CLiteHTMLReader	HTMLReader;		//The real HTML reader

	//Initialization load

	LanguageReader.Init(languageLog);
	HTMLReader.setEventHandler(&LanguageReader);//Assigning the initialization parser to the generic reader

	if (HTMLFile.Open(_T(languageDefaultFile), CFile::modeRead))
	{
		LanguageReady=true;
		msgBox = MessageBox(NULL,
			TEXT(languageDefaultFile),
			TEXT("Leyendo el fichero de idioma por defecto al arrancar"),
			MB_ICONASTERISK | MB_OKCANCEL );
		HTMLReader.ReadFile((HANDLE) HTMLFile.m_hFile);
		HTMLFile.Close();
	}
}

void CLanguageManager::SetText(const unsigned int TR, const char *text)
{
	if(!Game.InternationalizationFixed)
	strcpy((char*)&TextResource[TR],text);

}

void CLanguageManager::Initialize2(const char *language)////////////////////////////temporal
{

	if (strcmp(language,"EN") == 0)
	{
		TextResource2[1] = "Internet Error";
		TextResource2[2] = "Info : Internet connection can't be used.";
		TextResource2[3] = "Warning";

		TextResource2[11] = "1 click = ";
		TextResource2[12] = "Time.ms hold down key";
		TextResource2[13] = "Could not create edit control!";
		TextResource2[14] = "Error";

		TextResource2[21] = "Creates different size textures for best visual quality.\r\nNeeds a few more memory megabytes.";
		TextResource2[22] = "MIPMAP TEXTURES";
		TextResource2[23] = "Adjusts frame rate to screen frequency for smoothed movements.\r\nFPS is limited by this frequency.\r\n\r\n<V> key to swap ON/OFF during game.";
		TextResource2[24] = "VERTICAL SYNCHRONIZATION";
		TextResource2[25] = "Enables Internet music streaming.        \r\nChoose from :\r\n- Cable/DSL (128k)\r\n- (Dual)ISDN (56k)\r\n- Dialup (24k)\r\n- Custom (enter an URL)";
		TextResource2[26] = "STREAMED MUSIC";
		TextResource2[27] = "Adjusts the orientation of 2D objects (like particles) so they face the camera for best representation (only in 3D mode).\r\nNeeds more CPU.";
		TextResource2[28] = "BILLBOARDING";
		TextResource2[29] = "Enables Anti-aliasing, mainly for 3D objects for best visual quality.\r\nNeeds a compatible graphic card and more CPU / graphic card RAM.\r\n\r\n<A> key to swap ON/OFF during game.";
		TextResource2[30] = "ANTI-ALIASING";
		TextResource2[31] = "The quality/speed depends strongly on the video card driver.\r\n\r\nx2 (2 samples,2 taps) fastest mode.\r\nx2(Qx) (2,5) ~ speed of previous, quality of next.\r\nx4 (4,4) little nicer than previous but slower.\r\nx4S (4,9) nicer than previous but slower.";
		TextResource2[32] = "ANTI-ALIASING MODE";
		TextResource2[33] = "sio: TrackMouseEvent Failed";
		TextResource2[34] = "Error Creating OpenGL Window";
		TextResource2[35] = "Dialup";
		TextResource2[36] = "Custom";
		TextResource2[37] = "Sound Initialization Error";

		TextResource2[51] = "InitCommonControlEx() failed.";
		TextResource2[52] = "Error Registering Window Class!";
		TextResource2[53] = "Error";
		TextResource2[54] = " [Start]";
		TextResource2[55] = "Error";
		
		TextResource2[101] = "Streaming URL is empty";
		TextResource2[102] = "Streaming : NumberOfEntries in pls file not found";
		TextResource2[103] = "Streaming : numeric value for NumberOfEntries not found";
		TextResource2[104] = "Streaming : can't allocate memory for pls file (playlist)";
		TextResource2[105] = "Streaming : can't allocate memory for pls file (playlist->name)";
		TextResource2[106] = "Streaming : can't allocate memory for pls file (playlist->displayname)";
		
		TextResource2[111] = "Display";
		TextResource2[112] = "Quick FullScreen (current mode)";
		TextResource2[113] = "FullScreen";
		TextResource2[114] = "Window";
		TextResource2[115] = "Misc.";
		TextResource2[116] = "VSync";
		TextResource2[117] = "Streaming";
		TextResource2[118] = "Quality";
		TextResource2[119] = "Antialiasing";
		TextResource2[120] = "Billboarding";
		TextResource2[121] = "MipMap Textures";
		TextResource2[122] = "Start Game";
		TextResource2[123] = "Dialup";
		TextResource2[124] = "Custom";
		
		TextResource2[151] = "Web page";
		TextResource2[152] = "Forum";
		TextResource2[153] = "Wikik screen virtual keyboard";
		TextResource2[154] = "Close window";
		TextResource2[155] = "Customized with URL";
		TextResource2[156] = "Insert URL from playlist Shoutcast (*.pls),";
		TextResource2[157] = "or MP3. Check";

		TextResource2[201] = "Loading level: ";

		TextResource2[211] = "Loading saved level";

		TextResource2[221] = "Date ";
		TextResource2[222] = "Loading Sounds...";
		TextResource2[223] = "Reading initialization file...";
		TextResource2[224] = "A saved game has been detected. Would you like to load this?";
		TextResource2[225] = "Load Game";
		TextResource2[226] = "Reading saved game...";
		TextResource2[227] = "Reading first level of the game...";
		TextResource2[228] = "Generating default navy...";
		TextResource2[229] = "Shoots...";
		TextResource2[230] = "Environment variables...";
		TextResource2[231] = "Sound Initialization Error";

		TextResource2[241] = "Loading Textures...";
		TextResource2[242] = "Loading Meshes...";
		TextResource2[243] = "Exception: There was a problem during loading of the 3D objects.";
		TextResource2[244] = "Space Invaders [End loading]";
		
		TextResource2[301] = "Loading level: ";
		TextResource2[302] = "Error ";
		TextResource2[303] = "Enter immortal mode.";
		TextResource2[304] = "Enter mortal mode.";
		TextResource2[305] = "Please put some mp3 files in game directory.";
		TextResource2[306] = "Please choose Anti-aliasing option on start.";
		TextResource2[307] = "Anti-aliasing not used in 3D Line mode.";
		TextResource2[308] = "Anti-aliasing : ON";
		TextResource2[309] = "Anti-aliasing : OFF";
		TextResource2[310] = "Sorry, SwapIntervalExt() not available.";
		TextResource2[311] = "Vertical Synchronization : ON";
		TextResource2[312] = "Vertical Synchronization : OFF";
		
		TextResource2[321] = "sonido";
		TextResource2[322] = "audio";
		TextResource2[323] = "Time";
		TextResource2[324] = "angle";
		TextResource2[325] = "Camera.zoom";
		TextResource2[326] = "Exit ESC";
		TextResource2[327] = "MOUSE Rotation";
		TextResource2[328] = "M \r Music (on)";
		TextResource2[329] = "M \r Music (off)";
		TextResource2[330] = "B \r Background";
		TextResource2[331] = "R \r Replace cam";
		TextResource2[332] = "T \r Hide Text";
		TextResource2[333] = "+/- Zoom";
		TextResource2[334] = "Height";
		TextResource2[335] = "Width";
		TextResource2[336] = "< Press space key to change music >";
		
		TextResource2[351] = "ENTERING LEVEL";
		TextResource2[352] = "YOU WIN !";
		TextResource2[353] = "GAME OVER !";
		TextResource2[354] = "YOU LOSE !";
		
		TextResource2[361] = "error in the chgt of zik";

		TextResource2[401] = "Character Type (";
		TextResource2[402] = ") not compatible.";

		TextResource2[411] = "Start interpretation from initialization file.\n";

		TextResource2[421] = "Start Tag: ";
		TextResource2[422] = "First tag was not an HTML tag.";
		TextResource2[423] = "Tag ";
		TextResource2[424] = " not allowed in a BODY group.";
		TextResource2[425] = "Tag ";
		TextResource2[426] = " not allowed in a CHARACTER group.";
		TextResource2[427] = "Tag ";
		TextResource2[428] = " not allowed in a DIR group.";
		TextResource2[429] = "Tag ";
		TextResource2[430] = " not allowed in a HEAD group.";
		TextResource2[431] = "Only the attribute HEAD or BODY may appear in a group HTML. Not ";
		TextResource2[432] = "Only the attribute NAME or FILE may appear in a group NAME. Not ";
		TextResource2[433] = "Only a LANGUAGE command may appear in a LANGUAGES group. Not ";
		TextResource2[434] = "Only a NAME or SCORE command may appear in a SCORES group. Not ";
		TextResource2[435] = "Only the attribute SPS or EPS may appear in a SHOOT group. Not ";
		TextResource2[436] = "Speed coordinates not defined. Tag ";
		TextResource2[437] = " erroneous.";
		TextResource2[438] = "Only the attribute NAME or FILE may appear in a group TEXTURE. Not ";

		TextResource2[441] = " clossing tag without correspondent opening tag in a BODY segment.";

		TextResource2[451] = "Etiqueta fin: ";
		TextResource2[452] = " clossing tag in an unknown state.";
		TextResource2[453] = " clossing tag without correspondent opening tag in a ";
		TextResource2[454] = " segment.";

		TextResource2[461] = "Text: ";
		TextResource2[462] = "Maximum number of lives indicated (";
		TextResource2[463] = ") is less than the minimum amount allowed by the program (1). Value truncated to a minimum.";
		TextResource2[464] = "Maximum number of lives indicated (";
		TextResource2[465] = ") Is greater than the maximum amount allowed by the program (1). Truncated to the maximum value.";
		TextResource2[466] = "El fichero no corresponde a este videojuego.";
		TextResource2[467] = "El fichero es de tipo \"";
		TextResource2[468] = "\". Se requiere un fichero de tipo \"inicialización\".";
		TextResource2[469] = "File versión is v";
		TextResource2[470] = ", which does not correspond to this videogame version v";

		TextResource2[481] = "Comments: ";

		TextResource2[491] = "Interpretation of the file finished. \n";

		TextResource2[601] = "Loading Textures...";
		TextResource2[602] = "Loading Textures...(100%)";

		TextResource2[651] = "Error loading png";

		TextResource2[701] = "Character Type (";
		TextResource2[702] = ") not compatible.";

		TextResource2[711] = "Tag ";
		TextResource2[712] = " not allowed in a ";
		TextResource2[713] = " group.";

		TextResource2[721] = "Begining the initialization file parsing.\n";

		TextResource2[731] = "Start tag: ";
		TextResource2[732] = "First tag was not an HTML tag.";
		TextResource2[733] = "Tag not allowed in a BODY group.";
		TextResource2[734] = "Tag not allowed in a RESOURCE group.";
		TextResource2[735] = "Tag not allowed in a HEAD group.";
		TextResource2[736] = "Only the attribute HEAD or BODY may appear in a group HTML.";
		
		TextResource2[751] = "Etiqueta fin: ";
		TextResource2[752] = " clossing tag out from the HTML section.";
		TextResource2[753] = " clossing tag in an unknown state.";
		TextResource2[754] = " clossing tag without correspondent opening tag in a ";
		TextResource2[755] = " segment.";

		TextResource2[771] = "Texto: ";
		TextResource2[772] = "This file does not corresponds to this videogame.";
		TextResource2[773] = "This file type is \"";
		TextResource2[774] = "\". It is required a \"LANGUAGE\" file type.";
		TextResource2[775] = "This file versión is v";
		TextResource2[776] = ". This does not corresponds with this game version v";

		TextResource2[791] = "Comentarios: %s\n";

		TextResource2[801] = "Se ha acabado la interpretación del fichero.\n";
	}

	if (strcmp(language,"FR") == 0)
	{
		TextResource2[1] = " D'erreurs Internet  ";
		TextResource2[2] = "Infos: Connexion Internet ne peut pas être utilisé  ";
		TextResource2[3] = " Attention  ";
		TextResource2[11] = " 1 clic =  ";
		TextResource2[12] = " Time.ms maintenez enfoncée la touche  ";
		TextResource2[13] = " Impossible de créer le contrôle d'édition!  ";
		TextResource2[14] = " Erreur  ";
		TextResource2[21] = "Crée textures différentes tailles pour une meilleure qualité visuelle. \r\nNeeds quelques mégaoctets de mémoire plus.  ";
		TextResource2[22] = " TEXTURES mipmap  ";
		TextResource2[23] = "Règle cadence à la fréquence d'écran pour les mouvements lissés.\r\nPFN est limitée par cette fréquence.\r\n\r\n<V> clé pour échanger ON / OFF pendant le jeu.  ";
		TextResource2[24] = "SYNCHRONISATION VERTICALE ";
		TextResource2[25] = "Permet le streaming de musique sur Internet. \r \nChoisissez à partir de: \r \n-câble / DSL (128k) \r \n-(double) RNIS (56k) \r \n-Dialup (24k) \r \n-Custom (entrez une URL)  ";
		TextResource2[26] = " Musique en continu  ";
		TextResource2[27] = "Règle l'orientation des objets 2D (comme les particules), de sorte qu'ils font face à la caméra pour une meilleure représentation (uniquement en mode 3D). \r\nCPU Needs plus.  ";
		TextResource2[28] = " Billboarding  ";
		TextResource2[29] = "Permet anti-aliasing, surtout pour les objets 3D pour une meilleure qualité visuelle. \r \nNeeds une carte graphique compatible et plus de RAM carte CPU / graphique. \r \n \r \n <A> clé pour échanger ON / OFF lors d'un match.  ";
		TextResource2[30] = " ANTICRENELAGE  ";
		TextResource2[31] = " Le rapport qualité / vitesse dépend fortement de la pilote de carte vidéo. \r \n \r \nX2 (2 échantillons, 2 robinets) mode le plus rapide. \r \nX2 (Qx) (2,5) ~ vitesse de précédent, la qualité de la prochaine. \r \nx4 (4,4) peu mieux que le précédent, mais plus lente. \r \nx4S (4,9) plus agréable que le précédent mais plus lent.  ";
		TextResource2[32] = " Anti-crénelage  ";
		TextResource2[33] = " Sio: TrackMouseEvent Échec  ";
		TextResource2[34] = " Erreur Création fenêtre OpenGL  ";
		TextResource2[35] = " Dialup  ";
		TextResource2[36] = " Personnalisé  ";
		TextResource2[37] = "Une erreur d'initialisation sonore  ";
		TextResource2[51] = " InitCommonControlEx() a échoué.  ";
		TextResource2[52] = " Classe de fenêtre d'erreur vous inscrire!  ";
		TextResource2[53] = " Erreur  ";
		TextResource2[54] = " [Commencer]  ";
		TextResource2[55] = " Erreur  ";
		TextResource2[101] = "URL streaming est vide  ";
		TextResource2[102] = " Streaming: NumberOfEntries dans pls pas trouvé  ";
		TextResource2[103] = " Streaming: valeur numérique pour NumberOfEntries pas trouvé  ";
		TextResource2[104] = " Streaming: ne peut pas allouer de la mémoire pour le fichier pls (playlist)  ";
		TextResource2[105] = " Streaming: ne peut pas allouer de la mémoire pour le fichier pls (playlist-> nom)  ";
		TextResource2[106] = " Streaming: ne peut pas allouer de la mémoire pour le fichier pls (playlist-> display)  ";
		TextResource2[111] = " Afficher  ";
		TextResource2[112] = " FullScreen rapide (mode courant)  ";
		TextResource2[113] = " Plein écran ";
		TextResource2[114] = " Fenêtre  ";
		TextResource2[115] = " Divers.  ";
		TextResource2[116] = " VSync  ";
		TextResource2[117] = " Streaming  ";
		TextResource2[118] = " Qualité  ";
		TextResource2[119] = " Antialiasing  ";
		TextResource2[120] = " Billboarding  ";
		TextResource2[121] = " Textures mipmap  ";
		TextResource2[122] = " Début du jeu  ";
		TextResource2[123] = " Dialup  ";
		TextResource2[124] = " Personnalisé  ";
		TextResource2[151] = " Page Web  ";
		TextResource2[152] = " Forum  ";
		TextResource2[153] = " Wikik écran clavier virtuel  ";
		TextResource2[154] = "Fermer la fenêtre ";
		TextResource2[155] = " Personnalisé avec l'URL  ";
		TextResource2[156] = " Insérez URL à partir de Shoutcast playlist (*. Pls),  ";
		TextResource2[157] = " Ou MP3. Vérifiez  ";
		TextResource2[201] = " Chargement niveau:  ";
		TextResource2[211] = " Chargement sauvé niveau  ";
		TextResource2[221] = " Date  ";
		TextResource2[222] = "Sons chargement ...  ";
		TextResource2[223] = " Lecture du fichier d'initialisation ...  ";
		TextResource2[224] = " Une partie sauvegardée a été détecté. Voulez-vous charger cette?  ";
		TextResource2[225] = " Charger une partie  ";
		TextResource2[226] = " Lecture partie sauvegardée ...  ";
		TextResource2[227] = " Lecture de premier niveau du jeu ...  ";
		TextResource2[228] = " Génération marine par défaut ...  ";
		TextResource2[229] = " Pousses ...  ";
		TextResource2[230] = "Les variables d'environnement...  ";
		TextResource2[231] = "Une erreur d'initialisation sonore  ";
		TextResource2[241] = "Textures chargement ...  ";
		TextResource2[242] = "Meshes chargement ...  ";
		TextResource2[243] = " Exception: Il y avait un problème lors du chargement des objets 3D  ";
		TextResource2[244] = "Space Invaders [Fin de chargement]  ";
		TextResource2[301] = " Chargement niveau:  ";
		TextResource2[302] = " Erreur  ";
		TextResource2[303] = " Entrer en mode immortel.  ";
		TextResource2[304] = " Entrer en mode mortel.  ";
		TextResource2[305] = "S'il vous plaît mettre quelques fichiers mp3 dans le répertoire de jeu.  ";
		TextResource2[306] = "S'il vous plaît choisir anti-aliasing l'option au démarrage.  ";
		TextResource2[307] = " Anti-aliasing n'est pas utilisé en mode ligne 3D.  ";
		TextResource2[308] = " Anti-aliasing: ON  ";
		TextResource2[309] = " Anti-aliasing: OFF  ";
		TextResource2[310] = " Désolé, SwapIntervalExt () ne sont pas disponibles.  ";
		TextResource2[311] = "Synchronisation verticale: ON  ";
		TextResource2[312] = "Synchronisation verticale: OFF  ";
		TextResource2[321] = " Sonido  ";
		TextResource2[322] = " Audio  ";
		TextResource2[323] = " Temps  ";
		TextResource2[324] = " Angle  ";
		TextResource2[325] = " Camera.zoom  ";
		TextResource2[326] = " Sortie ESC  ";
		TextResource2[327] = "Rotation SOURIS ";
		TextResource2[328] = " M \r Musique (on)  ";
		TextResource2[329] = " M \r Musique (off)  ";
		TextResource2[330] = " B \r Contexte  ";
		TextResource2[331] = " R \r Remplacer la came  ";
		TextResource2[332] = " T \r Masquer le texte des  ";
		TextResource2[333] = " + / - Zoom  ";
		TextResource2[334] = " Hauteur  ";
		TextResource2[335] = " Largeur  ";
		TextResource2[336] = " <Touche espace presse pour changer la musique>  ";
		TextResource2[351] = " Entrant au niveau  ";
		TextResource2[352] = " VOUS GAGNEZ!  ";
		TextResource2[353] = " GAME OVER!  ";
		TextResource2[354] = " VOUS PERDEZ!  ";
		TextResource2[361] = " Erreur dans le chgt de zik  ";
		TextResource2[401] = "Type de caractère ( ";
		TextResource2[402] = ") Pas compatible.  ";
		TextResource2[411] = " Démarrer interprétation de fichier d'initialisation. \n  ";
		TextResource2[421] = " Démarrer Tag:  ";
		TextResource2[422] = "Tag premier n'était pas une balise HTML.  ";
		TextResource2[423] = " Tag  ";
		TextResource2[424] = " Pas autorisé dans un groupe de corps.  ";
		TextResource2[425] = " Tag  ";
		TextResource2[426] = " Pas autorisé dans un groupe de caractères.  ";
		TextResource2[427] = " Tag  ";
		TextResource2[428] = " Pas admis dans un groupe DIR.  ";
		TextResource2[429] = " Tag  ";
		TextResource2[430] = " Pas admis dans un groupe de tête.  ";
		TextResource2[431] = " Seul le chef d'attribut ou un organisme peut apparaître dans une page HTML groupe. Non  ";
		TextResource2[432] = " Seul le nom d'attribut ou un dossier peut apparaître dans un nom de groupe. Non  ";
		TextResource2[433] = " Seule une commande LANGUE peut apparaître dans un groupe LANGUES. Non  ";
		TextResource2[434] = " Seulement un nom ou une commande SCORE peuvent apparaître dans un groupe SCORES. Non  ";
		TextResource2[435] = " Seul l'attribut ou SPS EPS peuvent apparaître dans un groupe SHOOT. Non  ";
		TextResource2[436] = " Vitesse coordonnées n'est pas défini. Tag  ";
		TextResource2[437] = " Erronée.  ";
		TextResource2[438] = " Seul le nom d'attribut ou fichier peut apparaître dans une texture groupe. Non  ";
		TextResource2[441] = " Clossing balise sans balise d'ouverture correspondant à un segment de corps.  ";
		TextResource2[451] = " Etiqueta fin:  ";
		TextResource2[452] = " Clossing tag dans un état inconnu.  ";
		TextResource2[453] = " Clossing balise sans balise d'ouverture correspondant à un  ";
		TextResource2[454] = " Segment.  ";
		TextResource2[461] = " Texte:  ";
		TextResource2[462] = " Le nombre maximum de vies ont indiqué ( ";
		TextResource2[463] = ") Est inférieure à la quantité minimale autorisée par le programme (1). La valeur est tronquée à un minimum.  ";
		TextResource2[464] = " Le nombre maximum de vies ont indiqué ( ";
		TextResource2[465] = ") Est supérieure à la quantité maximale autorisée par le programme (1). Tronqué à la valeur maximale.  ";
		TextResource2[466] = " Le fichier n'est pas de ce veideogame.  ";
		TextResource2[467] = " Le type du fichier est ";
		TextResource2[468] = "Est nécessaire, une \" inicialización \" type de fichier.  ";
		TextResource2[469] = "Fichier version est v  ";
		TextResource2[470] = ", Qui ne correspond pas à cette version du jeu vidéo v  ";
		TextResource2[481] = " Commentaires:  ";
		TextResource2[491] = " Interprétation du fichier terminé. \n  ";
		TextResource2[601] = "Chargement Textures...";
		TextResource2[602] = "Chargement Textures...(100%)\n";
		TextResource2[651] = "Erreur chargement png";

		TextResource2[701] = "Character Type (";
		TextResource2[702] = ") not compatible.";

		TextResource2[711] = "Tag ";
		TextResource2[712] = " not allowed in a ";
		TextResource2[713] = " group.";

		TextResource2[721] = "Begining the initialization file parsing.\n";

		TextResource2[731] = "Start tag: ";
		TextResource2[732] = "First tag was not an HTML tag.";
		TextResource2[733] = "Tag not allowed in a BODY group.";
		TextResource2[734] = "Tag not allowed in a RESOURCE group.";
		TextResource2[735] = "Tag not allowed in a HEAD group.";
		TextResource2[736] = "Only the attribute HEAD or BODY may appear in a group HTML.";
		
		TextResource2[751] = "Etiqueta fin: ";
		TextResource2[752] = " clossing tag out from the HTML section.";
		TextResource2[753] = " clossing tag in an unknown state.";
		TextResource2[754] = " clossing tag without correspondent opening tag in a ";
		TextResource2[755] = " segment.";

		TextResource2[771] = "Texto: ";
		TextResource2[772] = "This file does not corresponds to this videogame.";
		TextResource2[773] = "This file type is \"";
		TextResource2[774] = "\". It is required a \"LANGUAGE\" file type.";
		TextResource2[775] = "This file versión is v";
		TextResource2[776] = ". This does not corresponds with this game version v";

		TextResource2[791] = "Comentarios: %s\n";

		TextResource2[801] = "Se ha acabado la interpretación del fichero.\n";
	}
	
	if (strcmp(language,"CA") == 0)
	{
		TextResource2[1] = "Error d'Internet";
		TextResource2[2] = "Informació: connexió a Internet no es poden utilitzar.  ";
		TextResource2[3] = "Adverténcia  ";
		TextResource2[11] = "Feu 1 clic a =  ";
		TextResource2[12] = "Tiempo.ms premeu tecla  ";
		TextResource2[13] = "No s'ha pogut crear el control d'edició  ";
		TextResource2[14] = "Error  ";
		TextResource2[21] = "Crea diferents mides de textures per a millor qualitat visual.\r\nNecessitats uns megabytes més de memòria.  ";
		TextResource2[22] = "MIPMAP TEXTURES  ";
		TextResource2[23] = "Ajustar la velocitat de fotogrames a freqüència de trama per suavitzen els moviments. \r\nFPS està  limitada per la freqüència. \r\n\r\n<v> clau per intercanviar ON / OFF durant joc.  ";
		TextResource2[24] = "SINCRONITZACIÓ VERTICAL  ";
		TextResource2[25] = "Habilita flux de música d'Internet música.        \r\nTrieu a partir de:\r\n- Cable / DSL (128 K)\r\n- (Dual) ISDN (56 K)\r\n- Accés telefònic (de 24 k)\r\n- Personalitzada (introduir una URL.)  ";
		TextResource2[26] = "MUSICA EN STREAMING  ";
		TextResource2[27] = "Ajusta l'orientació d'objectes 2D (com partícules), de manera que s'enfronten la cà mera per millor representació (només en la manera 3D).\r\nCal més CPU.  ";
		TextResource2[28] = "Billboarding  ";
		TextResource2[29] = "Habilita el suavitzat, principalment per objectes 3D per a la millor qualitat visual.\r\nNecessita un compatible targeta grà fica i molt més CPU / targetes grà fiques de RAM.\r\n<a> clau per intercanviar ON / OFF durant joc.  ";
		TextResource2[30] = "Anti-aliasing  ";
		TextResource2[31] = "La qualitat i velocitat depén de manera significativa al controlador de la targeta gràfica.\r\n\r\nx2 (2 mostres, 2 aixetes) més rà pid manera.\r\nx2(Qx) (2,5) ~ velocitat d'anterior , la qualitat de la propera.\r\nx4 (4,4) poc més que anteriors perà² més lenta.\r\n4S (4,9) més que anteriors perà² més lent.  ";
		TextResource2[32] = "Anti-aliasing MODE  ";
		TextResource2[33] = "Sio: TrackMouseEvent Error  ";
		TextResource2[34] = "Pogut crear OpenGL Finestra  ";
		TextResource2[35] = "Marcar  ";
		TextResource2[36] = "Personalitzada  ";
		TextResource2[37] = "So Error d'inicialització de  ";
		TextResource2[51] = "InitCommonControlEx () ha fallat.  ";
		TextResource2[52] = "Error en registrar la classe Application.Window.  ";
		TextResource2[53] = "Error  ";
		TextResource2[54] = "[Inici]  ";
		TextResource2[55] = "Error  ";
		TextResource2[101] = "URL de Transmissió està  buit  ";
		TextResource2[102] = "Transmissió: NumberOfEntries no trobat a l'arxiu pls  ";
		TextResource2[103] = "Transmissió: valor numéric per NumberOfEntries no trobada  ";
		TextResource2[104] = "Transmissió: no es pot assignar memà²ria per pls arxiu (llista de reproducció)  ";
		TextResource2[105] = "Transmissió: no es pot assignar memà²ria per pls arxiu (playlist-> nom)  ";
		TextResource2[106] = "Transmissió: no es pot assignar memà²ria per pls arxiu (playlist-> displayname)  ";
		TextResource2[111] = "Mostra  ";
		TextResource2[112] = "Pantalla completa ràpida (mode actiu)  ";
		TextResource2[113] = "Pantalla completa </CONTENT> ";
		TextResource2[114] = "Finestra  ";
		TextResource2[115] = "Misc.  ";
		TextResource2[116] = "VSync  ";
		TextResource2[117] = "Tranmisión / CONTENT> ";
		TextResource2[118] = "Qualitat  ";
		TextResource2[119] = "Antialias  ";
		TextResource2[120] = "Billboarding  ";
		TextResource2[121] = "MipMap de les textures  ";
		TextResource2[122] = "Comenàçar joc  ";
		TextResource2[123] = "Marcat  ";
		TextResource2[124] = "Personalitzat  ";
		TextResource2[151] = "Pà gina Web  ";
		TextResource2[152] = "Fà²rum  ";
		TextResource2[153] = "Wikik pantalla teclat virtual  ";
		TextResource2[154] = "Tancar finestra  ";
		TextResource2[155] = "Personalitzada amb la direcció URL  ";
		TextResource2[156] = "Insereix adreàça URL de la llista de reproducció Shoutcast (*. Pls),  ";
		TextResource2[157] = "o MP3. Comproveu  ";
		TextResource2[201] = "Carregant nivell:  ";
		TextResource2[211] = "Carregant nivell guardat  ";
		TextResource2[221] = "Data </CONTENT> ";
		TextResource2[222] = "Carregant Sons ...  ";
		TextResource2[223] = "Lectura d'arxiu d'inicialització ...  ";
		TextResource2[224] = "Un joc desat s'ha detectat. Voleu carregar aquest?  ";
		TextResource2[225] = "Càrrega joc  ";
		TextResource2[226] = "Lectura joc guardat ...  ";
		TextResource2[227] = "Lectura primer nivell del joc ...  ";
		TextResource2[228] = "Generant nau per defecte ...  ";
		TextResource2[229] = "Trets ...  ";
		TextResource2[230] = "Variables d'entorn ...  ";
		TextResource2[231] = "Error en la inicialització del so  ";
		TextResource2[241] = "Carregant textures ...  ";
		TextResource2[242] = "Carregant Malles ...  ";
		TextResource2[243] = "Excepció: S'ha produà¯t un problema durant la cà rrega dels objectes 3D.  ";
		TextResource2[244] = "Espai Invaders OpenGL [End cà rrega]  ";
		TextResource2[301] = "Carregant nivell:  ";
		TextResource2[302] = "Error  ";
		TextResource2[303] = "Entrant manera immortal.  ";
		TextResource2[304] = "Entrant manera mortal.  ";
		TextResource2[305] = "Poso alguns arxius mp3 en el directori del joc.  ";
		TextResource2[306] = "Trieu l'opció de suavitzat d'inici.  ";
		TextResource2[307] = "El suavitzat no s'utilitza en Línia 3D manera.  ";
		TextResource2[308] = "Suavitzat: ON  ";
		TextResource2[309] = "Suavitzat: OFF  ";
		TextResource2[310] = "Ho sentim, SwapIntervalExt () no està  disponible.  ";
		TextResource2[311] = "Sincronització vertical: ON  ";
		TextResource2[312] = "Sincronització vertical: OFF  ";
		TextResource2[321] = "So  ";
		TextResource2[322] = "àudio  ";
		TextResource2[323] = "Hora  ";
		TextResource2[324] = "Angle  ";
		TextResource2[325] = "Camara.zoom  ";
		TextResource2[326] = "Sortir ESC  ";
		TextResource2[327] = "Rotació del MOUSE  ";
		TextResource2[328] = "M \r música (en)  ";
		TextResource2[329] = "M \r música (desactivat)  ";
		TextResource2[330] = "B \r Fons  ";
		TextResource2[331] = "R \r Reemplaàçar lleva  ";
		TextResource2[332] = "T \r Amaga el text  ";
		TextResource2[333] = "+ / - Zoom  ";
		TextResource2[334] = "Alçada  ";
		TextResource2[335] = "Ample  ";
		TextResource2[336] = "<; Premeu espai per canviar música >  ";
		TextResource2[351] = "ENTRAT NIVELL  ";
		TextResource2[352] = "VOSTàˆ GUANYA!  ";
		TextResource2[353] = "FI DEL JOC!  ";
		TextResource2[354] = "VOSTàˆ PERD!  ";
		TextResource2[361] = "Error en el chgt de zik  ";
		TextResource2[401] = "Tipus de carà cter ( ";
		TextResource2[402] = ") No compatible.  ";
		TextResource2[411] = "Inici interpretació de l'arxiu d'inicialització.  ";
		TextResource2[421] = "Etiqueta inicial:  ";
		TextResource2[422] = "Primera etiqueta no és l'etiqueta HTML.  ";
		TextResource2[423] = "Etiqueta </CONTENT> ";
		TextResource2[424] = "No està  permesa en grup BODY.  ";
		TextResource2[425] = "Etiqueta </CONTENT> ";
		TextResource2[426] = "No està  permesa en grup CHARACTER.  ";
		TextResource2[427] = "Etiqueta </CONTENT> ";
		TextResource2[428] = "No està  permesa en grup DIR.  ";
		TextResource2[429] = "Etiqueta </CONTENT> ";
		TextResource2[430] = "No està  permesa en una HEAD grup.  ";
		TextResource2[431] = "Només l'atribut HEAD o BODY poden aparéixer en el grup HTML. No  ";
		TextResource2[432] = "Només l'atribut NOM o FILE poden aparéixer en grup NAME. No  ";
		TextResource2[433] = "Només un LANGUAGE comanda pot aparéixer en el grup LANGUAGES. No  ";
		TextResource2[434] = "Només un NOME o SCORE pot aparéixer en el grup scores. No  ";
		TextResource2[435] = "Només l'atribut SPS o EPS poden aparéixer en el grup SHOOT. No  ";
		TextResource2[436] = "Velocitat de coordenades no definit. Tag  ";
		TextResource2[437] = "Erronis.  ";
		TextResource2[438] = "Només l'atribut NOM o FILE poden aparéixer en un grup texture. No  ";
		TextResource2[441] = "Etiqueta de tancament sense corresponent etiqueta d'obertura en el segment BODY.  ";
		TextResource2[451] = "Etiqueta fi:  ";
		TextResource2[452] = "Clossing etiqueta en un estat desconegut.  ";
		TextResource2[453] = "Etiqueta de tancament sense corresponent etiqueta d'obertura en  ";
		TextResource2[454] = "Segment.  ";
		TextResource2[461] = "Text:  ";
		TextResource2[462] = "Capacitat mà xima de vida indicat ( ";
		TextResource2[463] = ") à‰s menor que el mínim permés pel programa (1). Valor truncat al mínim.  ";
		TextResource2[464] = "Capacitat mà xima de vida indicat ( ";
		TextResource2[465] = ") à‰s més gran que l'import mà xim permés pel programa (1). Truncat que el valor mà xim.  ";
		TextResource2[466] = "El fitxer no correspon un aquest videojoc.  ";
		TextResource2[467] = "El fitxer és de tipus ";
		TextResource2[468] = "Cal un fitxer de tipus \" inicialització \".  ";
		TextResource2[469] = "Versió de l'arxiu és v  ";
		TextResource2[470] = "que no es correspon amb aquesta versió v  ";
		TextResource2[481] = "Comentaris:  ";
		TextResource2[491] = "Interpretació del fitxer finalitzat.  ";
		TextResource2[601] = "Cargant Textures...";
		TextResource2[602] = "Cargant Textures...(100%)\n";
		TextResource2[651] = "Error cargant png";

		TextResource2[701] = "Tipus de caracter (";
		TextResource2[702] = ") no compatible.";

		TextResource2[711] = "Etiqueta ";
		TextResource2[712] = " nt permitida in a ";
		TextResource2[713] = " grup.";

		TextResource2[721] = "Inici del parseat del fichero de initcilizatió.\n";

		TextResource2[731] = "Etiqueta de inicio: ";
		TextResource2[732] = "Primera etiqueta no es una etiqueta HTML.";
		TextResource2[733] = "Etiqueta no permitida en el grup BODY.";
		TextResource2[734] = "Etiqueta no permitida en el grup RESOURCE.";
		TextResource2[735] = "Etiqueta no permitida en el grup HEAD.";
		TextResource2[736] = "Només el atribut HEAD o BODY pòt apareiçer en el grup HTML.";
		
		TextResource2[751] = "Etiqueta fin: ";
		TextResource2[752] = " tancant etiquetes de la secció HTML.";
		TextResource2[753] = " tancant etiquetes en stat desconegut.";
		TextResource2[754] = " tancant etiquetes sense etiqueta de opertura correspont en ";
		TextResource2[755] = " segment.";

		TextResource2[771] = "Text: ";
		TextResource2[772] = "El tipus de arxiu no correspon a este joc.";
		TextResource2[773] = "El tipus de arxiu es\"";
		TextResource2[774] = "\". Es requerit el tipus de arxiu \"LANGUAGE\".";
		TextResource2[775] = "Versió de l'arxiu és v  ";
		TextResource2[776] = ". This does not corresponds with this game version v";

		TextResource2[791] = "Comentaris: %s\n";

		TextResource2[801] = "Interpretació del fitxer finalitzat.\n";
	}

	if (strcmp(language,"ES") == 0)
	{
		TextResource2[1] = "Error de Internet";
		TextResource2[2] = "Información: conexión a Internet no se pueden utilizar.";
		TextResource2[3] = "Advertencia";
		TextResource2[11] = "haga 1 clic en =";
		TextResource2[12] = "Tiempo.ms mantenga presionada tecla";
		TextResource2[13] = "No se pudo crear el control de edición!";
		TextResource2[14] = "Error";
		TextResource2[21] = "Crea distintos tamaà±os de texturas para mejor calidad visual.\r Necesidades unos megabytes más de memoria.";
		TextResource2[22] = "MIPMAP TEXTURES";
		TextResource2[23] = "Ajusta la velocidad de fotogramas a frecuencia de trama para suavizan los movimientos.\r FPS está limitada por la frecuencia.\r\r<V>clave para intercambiar ON/OFF durante juego.";
		TextResource2[24] = "SINCRONIZACIÓN VERTICAL";
		TextResource2[25] = "Habilita flujo de música de Internet música.\r Elija a partir de:\r - Cable/DSL (128 K)\r - (dual) de ISDN (RDSI) (56 K)\r - Configurar el acceso telefónico (de 24 k)\r - Personalizada (introducir una URL.)";
		TextResource2[26] = "MUSICA EN STREAMING";
		TextResource2[27] = "Ajusta la orientación de objetos 2D (como partículas), de modo que se enfrentan la cámara para mejor representación (sólo en el modo 3D).\r Se necesita más CPU.";
		TextResource2[28] = "BILLBOARDING";
		TextResource2[29] = "Habilita el suavizado, principalmente para objetos 3D para la mejor calidad visual.\r Necesita un compatible tarjeta gráfica y mucho más CPU / tarjetas gráficas de RAM.\r\r <A> clave para intercambiar ON / OFF durante juego.";
		TextResource2[30] = "ANTI-ALIASING";
		TextResource2[31] = "La calidad y velocidad depende de manera significativa en el controlador de tarjeta de vídeo.\r\r x2 (2 muestras, 2 grifos) más rápido modo.\r x2 (Qx) (2,5) ~ velocidad de anterior, la calidad de la próxima.\r x4 (4,4) poco más que anteriores pero más lenta.\r x4S (4,9) más que anteriores pero más lento.";
		TextResource2[32] = "ANTI-ALIASING MODE";
		TextResource2[33] = "sio: TrackMouseEvent Error";
		TextResource2[34] = "Error al crear OpenGL Ventana";
		TextResource2[35] = "marcar";
		TextResource2[36] = "personalizada";
		TextResource2[37] = "sonido Error de inicialización de";
		TextResource2[51] = "InitCommonControlEx() falló.";
		TextResource2[52] = "Error al registrar la clase Application.Window.";
		TextResource2[53] = "Error";
		TextResource2[54] = " [Inicio]";
		TextResource2[55] = "Error";
		TextResource2[101] = "URL de Transmisión está vacío";
		TextResource2[102] = "Transmisión: NumberOfEntries no encontrado en el archivo pls";
		TextResource2[103] = "Transmisión: valor numérico para NumberOfEntries no encontrado";
		TextResource2[104] = "de transmisión por secuencias: no se puede asignar memoria para pls archivo (lista de reproducción)";
		TextResource2[105] = "de transmisión por secuencias: no se puede asignar memoria para pls archivo (playlist- > nombre)";
		TextResource2[106] = "de transmisión por secuencias: no se puede asignar memoria para pls archivo (playlist- > displayname)";
		TextResource2[111] = "Mostrar";
		TextResource2[112] = "Pantalla completa rápida (modo activo)";
		TextResource2[113] = "Pantalla completa";
		TextResource2[114] = "Ventana";
		TextResource2[115] = "Misc.";
		TextResource2[116] = "VSync";
		TextResource2[117] = "Tranmisión/CONTENT>";
		TextResource2[118] = "Calidad";
		TextResource2[119] = "Antialias";
		TextResource2[120] = "Billboarding";
		TextResource2[121] = "MipMap de las texturas";
		TextResource2[122] = "Empezar juego";
		TextResource2[123] = "Marcado";
		TextResource2[124] = "Personalizado";
		TextResource2[151] = "Página Web";
		TextResource2[152] = "Foro";
		TextResource2[153] = "Wikik pantalla teclado virtual";
		TextResource2[154] = "Cerrar ventana";
		TextResource2[155] = "personalizada con la dirección URL";
		TextResource2[156] = "Insertar dirección URL de la lista de reproducción Shoutcast (* .pls),";
		TextResource2[157] = "o MP3. Compruebe";
		TextResource2[201] = "Cargando nivel:";
		TextResource2[211] = "Cargando nivel guardado";
		TextResource2[221] = "Fecha";
		TextResource2[222] = "Cargando Sonidos...";
		TextResource2[223] = "Lectura de archivo de inicialización...";
		TextResource2[224] = "Un juego guardado se ha detectado. ¿Desea cargar este?";
		TextResource2[225] = "Carga juego";
		TextResource2[226] = "Lectura juego guardado...";
		TextResource2[227] = "Lectura primer nivel del juego...";
		TextResource2[228] = "Generando nave predeterminada...";
		TextResource2[229] = "Disparos...";
		TextResource2[230] = "Variables de entorno...";
		TextResource2[231] = "Error en la inicialización del sonido";
		TextResource2[241] = "Cargando texturas...";
		TextResource2[242] = "Cargando Mallas...";
		TextResource2[243] = "Excepción: Se ha producido un problema durante la carga de los objetos 3D.";
		TextResource2[244] = "Space Invaders [Fin carga]";
		TextResource2[301] = "Cargando nivel:";
		TextResource2[302] = "Error";
		TextResource2[303] = "Entrando modo immortal.";
		TextResource2[304] = "Entrando modo mortal.";
		TextResource2[305] = "Pongo algunos archivos mp3 en el directorio del juego .";
		TextResource2[306] = "Elija la opción de suavizado de inicio.";
		TextResource2[307] = "El suavizado no se utiliza en Línea 3D modo.";
		TextResource2[308] = "Suavizado: ON";
		TextResource2[309] = "Suavizado: OFF";
		TextResource2[310] = "Lo sentimos, SwapIntervalExt() no está disponible.";
		TextResource2[311] = "Sincronización vertical: ON";
		TextResource2[312] = "Sincronización vertical: OFF";
		TextResource2[321] = "sonido";
		TextResource2[322] = "audio";
		TextResource2[323] = "Hora";
		TextResource2[324] = "ángulo";
		TextResource2[325] = "Camara.zoom";
		TextResource2[326] = "Salir ESC";
		TextResource2[327] = "Rotación del MOUSE";
		TextResource2[328] = "M \r música (en)";
		TextResource2[329] = "M \r música (desactivado)";
		TextResource2[330] = "B \r Fondo";
		TextResource2[331] = "R \r Reemplazar leva";
		TextResource2[332] = "T \r Ocultar texto";
		TextResource2[333] = "+/- Zoom";
		TextResource2[334] = "Altura";
		TextResource2[335] = "Ancho";
		TextResource2[336] = "< Presione espacio para cambiar música >";
		TextResource2[351] = "ENTRADO NIVEL";
		TextResource2[352] = "USTED GANA!";
		TextResource2[353] = "FIN DEL JUEGO!";
		TextResource2[354] = "USTED PIERDE!";
		TextResource2[361] = "error en el chgt de zik";
		TextResource2[401] = "tipo de carácter (";
		TextResource2[402] = ") no compatible.";
		TextResource2[411] = "Inicio interpretación del archivo de inicialización.";
		TextResource2[421] = "etiqueta inicial:";
		TextResource2[422] = "Primera etiqueta no es la etiqueta HTML.";
		TextResource2[423] = "Etiqueta ";
		TextResource2[424] = " no está permitida en grupo BODY.";
		TextResource2[425] = "Etiqueta ";
		TextResource2[426] = " no está permitida en grupo CHARACTER .";
		TextResource2[427] = "Etiqueta ";
		TextResource2[428] = " no está permitida en grupo DIR.";
		TextResource2[429] = "Etiqueta ";
		TextResource2[430] = " no está permitida en una HEAD grupo.";
		TextResource2[431] = "Sólo el atributo HEAD o BODY pueden aparecer en el grupo HTML. No";
		TextResource2[432] = "Sólo el atributo NOMBRE o FILE pueden aparecer en grupo NAME. No";
		TextResource2[433] = "Sólo un LANGUAGE comando puede aparecer en el grupo LANGUAGES. No";
		TextResource2[434] = "Sólo un NOME o SCORE puede aparecer en el grupo SCORES. No ";
		TextResource2[435] = "Sólo el atributo SPS o EPS pueden aparecer en el grupo SHOOT. No ";
		TextResource2[436] = "Velocidad de coordenadas no definido. Tag";
		TextResource2[437] = "erróneos.";
		TextResource2[438] = "Sólo el atributo NOMBRE o FILE pueden aparecer en un grupo TEXTURE. No";
		TextResource2[441] = "Etiqueta de cierre sin correspondiente etiqueta de apertura en el segmento BODY.";
		TextResource2[451] = "Etiqueta fin: ";
		TextResource2[452] = "clossing etiqueta en un estado desconocido.";
		TextResource2[453] = " Etiqueta de cierre sin correspondiente etiqueta de apertura en ";
		TextResource2[454] = " segmento.";
		TextResource2[461] = "Texto: ";
		TextResource2[462] = "Número máximo de vida indicado (";
		TextResource2[463] = ") es menor que el mínimo permitido por el programa (1). Valor truncado al mínimo.";
		TextResource2[464] = "Número máximo de vida indicado (";
		TextResource2[465] = ") Es mayor que el importe máximo permitido por el programa (1). Truncado que el valor máximo.";
		TextResource2[466] = "El fichero no corresponde un este videojuego.";
		TextResource2[467] = "El fichero es de tipo \"";
		TextResource2[468] = "\". Se requiere un fichero de tipo \"inicialización\".";
		TextResource2[469] = "Versión del archivo es v";
		TextResource2[470] = ", que no se corresponde con esta versión v";
		TextResource2[481] = "Comentarios:";
		TextResource2[491] = "Interpretación del archivo finalizado.";
		TextResource2[601] = "Cargando texturas...";
		TextResource2[602] = "Cargando texturas...(100%)\n";
		TextResource2[651] = "Error cargando png";

		TextResource2[701] = "Character Type (";
		TextResource2[702] = ") not compatible.";

		TextResource2[711] = "Tag ";
		TextResource2[712] = " not allowed in a ";
		TextResource2[713] = " group.";

		TextResource2[721] = "Begining the initialization file parsing.\n";

		TextResource2[731] = "Start tag: ";
		TextResource2[732] = "First tag was not an HTML tag.";
		TextResource2[733] = "Tag not allowed in a BODY group.";
		TextResource2[734] = "Tag not allowed in a RESOURCE group.";
		TextResource2[735] = "Tag not allowed in a HEAD group.";
		TextResource2[736] = "Only the attribute HEAD or BODY may appear in a group HTML.";
		
		TextResource2[751] = "Etiqueta fin: ";
		TextResource2[752] = " clossing tag out from the HTML section.";
		TextResource2[753] = " clossing tag in an unknown state.";
		TextResource2[754] = " clossing tag without correspondent opening tag in a ";
		TextResource2[755] = " segment.";

		TextResource2[771] = "Texto: ";
		TextResource2[772] = "This file does not corresponds to this videogame.";
		TextResource2[773] = "This file type is \"";
		TextResource2[774] = "\". It is required a \"LANGUAGE\" file type.";
		TextResource2[775] = "This file versión is v";
		TextResource2[776] = ". This does not corresponds with this game version v";

		TextResource2[791] = "Comentarios: %s\n";

		TextResource2[801] = "Se ha acabado la interpretación del fichero.\n";
	}
}