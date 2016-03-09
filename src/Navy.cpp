/*The place where resides the class Navy
  General class to support all the attribute general to all the ships, supply ships,...

	@author Ramon Molla
	@version 2012-10
*/

#include <AI\FSMParser.h>
#include "SI_AI.h"

#include "Global.h"

#include <UGKPhysics.h>
#include "SIGame.h"
#include "Navy.h"
#include <math.h>
#include <glext.h>
#include <UGKLog.h>

#define STRINGMAXLENGTH 256

/**   
   * @fn  CNavy::Init
   * Start Navy
   * The default parameters starts firing frequency, level disabled health    
   * Speed ??starts in left direction, and acceleration from 0    
   * Starts the position in 0    
   * It also defined the limits of movement of enemy ships
*/
void CNavy::Init(void)
{
	//Ships Control
	Bounce					= CN_MAX_X_SHIFT;	// Initial value to let the navy move to the same direction for a while
	ShootsFrequency			=  0;
	ShipLevDefaultHealth	= -1;				///Ships health level per level

	ReGenShips				= false;

	Speed.Set(CN_INITIAL_SPEED,	// Initial speed value. Only moving in the "x" direction towards the left
			  0.0f,
			  0.0f);

	Speed.Set(CN_INITIAL_ACC,
			  0.0f,
			  0.0f);

	StartShipsPos.Set(CN_INITIAL_X_POS,
					  CN_INITIAL_Y_POS,
					  0.0);

	//INITIAL POSITION IS 0 BECAUSE OF CHARACTER class INITIALIZATION

	Direction			= UGKPHY_RIGHT;			// Moving by default to the left
	CurrentShip			= 0;					// Current ship being loaded initialy is on the first position
	CurrentSupplyShip	= 0;
	CurrentCircleShip	= 0;

	RenderMode		= CHAR_NO_RENDER;
	PhysicMode		= UGKPHY_PHANTOM;					//Invisible and untouchable
	Type			= CHARS_NAVY;

	faseCambio		= false;
	ShipsMoveDown	= 1;
	WithShoots		= 1;

	NextShip2Upd	= 0.0;
	NextSShip2Upd	= 0.0;
	NextCShip2Upd	= 0.0;
	
#ifndef CHAR_USE_QUADTREE
	///Global dimensions of the navy playground.
	///When any ship of the navy reaches left or right BB limit, the navy has to change direction
	///When any ship of the navy reaches the bottom of the AABB, the game is finished
	AABB[CHAR_BBMIN][XDIM].Value = CSIG_PG_LEFT;
	AABB[CHAR_BBMAX][XDIM].Value = CSIG_PG_RIGHT;
	AABB[CHAR_BBMIN][YDIM].Value =  .0f;
	AABB[CHAR_BBMAX][YDIM].Value = CSIG_PLAYER_Y_POSITION;
	AABB[CHAR_BBMIN][ZDIM].Value = 0.0f;
	AABB[CHAR_BBMAX][ZDIM].Value = 0.0f;
#endif

	msg = new cMsgNavy;
	msg->Type = RTDESKMSG_NAVY;
	msg->msgSubType = TIPOUPDSHIPS;
	msg->Propietary = true;

	///Sounds
}

void CNavy::Deinitialize(void)
{
	/*for (unsigned int i = 0; i <= Ship.size(); i++)
	{
		CShip *ShipAux;
		ShipAux = Ship[i];
		ShipAux->AI->outEvent(CSS_NameTransition[CSS_FINISHED], NULL, (void*) ShipAux);
	}

	for (unsigned int i = 0; i <= SupplyShip.size(); i++)
	{
		CSupplyShip *SupplyShipAux;
		SupplyShipAux = SupplyShip[i];
		SupplyShipAux->AI->outEvent(CSS_NameTransition[CSS_FINISHED], NULL, (void*) SupplyShipAux);
	}

	for (unsigned int i = 0; i <= CircleShip.size(); i++)
	{
		CCircleShip *CirlceShipAux;
		CirlceShipAux = CircleShip[i];
		CirlceShipAux->AI->outEvent(CSS_NameTransition[CSS_FINISHED], NULL, (void*) CirlceShipAux);
	}*/

	Ship.clear();
	SupplyShip.clear();
	CircleShip.clear();
}

/**   
   * @fn CNavy::Update
   * Updates both ships and supplyships
*/
void CNavy::Update (void)	///Performs all the geometric transformations on every ship alive in the navy
{
	UpdateShips();
	UpdateSupplyShips();
	UpdateCircleShips();
}

/**   
   * @fn CNavy::UpdateSupplyShips
   * 
*/
void CNavy::UpdateSupplyShips (void)	///Performs all the geometric transformations on every ship alive in the navy
{
	#ifdef DEF_UPD_TIME
	Game.TimerManager.Timers[TmrUpd].InitCounting();
	#endif

	unsigned int i;
	
	NextSShip2Upd	= LLONG_MAX;
	for(i=0;i<SupplyShip.size();i++){
		SupplyShip[i]->Update();
		if(SupplyShip[i]->TicksToUpdateSupplyShip < NextSShip2Upd)
			NextSShip2Upd = SupplyShip[i]->TicksToUpdateSupplyShip;
	}

	#ifdef DEF_UPD_TIME
	EndAccCounting(TmrUpd);
	#endif
}


/**   
   * @fn CNavy::UpdateCircleShips
   * 
*/
void CNavy::UpdateCircleShips (void){	///Performs all the geometric transformations on every ship alive in the navy

	#ifdef DEF_UPD_TIME
	Game.TimerManager.Timers[TmrUpd].InitCounting();
	#endif

	unsigned int i;

	NextCShip2Upd	= LLONG_MAX;
	for(i=0;i<CircleShip.size();i++){
		CircleShip[i]->Update();
		if(CircleShip[i]->TicksToUpdateCircleShip < NextCShip2Upd)
			NextCShip2Upd = CircleShip[i]->TicksToUpdateCircleShip;
	}

	#ifdef DEF_UPD_TIME
	EndAccCounting(TmrUpd);
	#endif
}

/**   
   * @fn CNavy::UpdateShips
   * Created a temporally variable NewDirection for update
   * After view which ships are alived and base on their directions, we will asigned them their NewDirection
   * Base on a NewDirection updated the enemies ships
*/
void CNavy::UpdateShips (void)	///Performs all the geometric transformations on every ship alive in the navy
{
	unsigned int i;

	#ifdef DEF_UPD_TIME
	Game.TimerManager.Timers[TmrUpd].InitCounting();
	#endif
	if(faseCambio){

		for(i=0;i<Ship.size();i++)
			Ship[i]->faseActual = true;
		faseCambio=false;
	}

	NextShip2Upd	= LLONG_MAX;
	for (i=0; i<Ship.size(); i++){
		Ship[i]->Update();
		if(Ship[i]->TicksToUpdateShip < NextShip2Upd)
			NextShip2Upd = Ship[i]->TicksToUpdateShip;
	}

	#ifdef DEF_UPD_TIME
	EndAccCounting(TmrUpd);
	#endif
}

/**   
   * @fn CNavy::SetMeshes
   * Set ship's mesh
   * Asigne a mesh of MeshesManager to each ship and supplyship 
*/
void CNavy::SetMeshes (void)	
{
	unsigned int i;

	//Ships
	for (i=0; i<Ship.size(); i++){
		if(Ship[i]->IndMesh != CMM_NO_MESH){
			Ship[i]->Mesh = MeshesManager.Meshes[Ship[i]->IndMesh];
			if(Ship[i]->IndTexture3D != CTM_NO_TEXTURE) Ship[i]->Mesh->modelo.Materials->tex.texture[0] = TexturesManager.Textures[Ship[i]->IndTexture3D]->Texture.gl_id;
		}
	}

	//SupplyShips
	for (i=0; i<SupplyShip.size(); i++){
		if(SupplyShip[i]->IndMesh != CMM_NO_MESH){
			SupplyShip[i]->Mesh = MeshesManager.Meshes[SupplyShip[i]->IndMesh];
			if(SupplyShip[i]->IndTexture3D != CTM_NO_TEXTURE) SupplyShip[i]->Mesh->modelo.Materials->tex.texture[0] = TexturesManager.Textures[SupplyShip[i]->IndTexture3D]->Texture.gl_id;
		}
	}

	//CircleShips
	for (i=0; i<CircleShip.size(); i++){
		if(CircleShip[i]->IndMesh != CMM_NO_MESH){
			CircleShip[i]->Mesh = MeshesManager.Meshes[CircleShip[i]->IndMesh];
			if(CircleShip[i]->IndTexture3D != CTM_NO_TEXTURE) CircleShip[i]->Mesh->modelo.Materials->tex.texture[0] = TexturesManager.Textures[CircleShip[i]->IndTexture3D]->Texture.gl_id;
		}
	}
}

/**   
   * @fn CNavy::ChangeRenderMode
   * @param CHAR_RENDER_MODE Mode represents the mode
   * That depends if the ships is alive, it can be change his mode
*/
void CNavy::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	RenderMode = Mode;

	unsigned int i;
	
	for (i=0;i<Ship.size(); i++) if (Ship[i]->Alive) Ship[i]->ChangeRenderMode(Mode);
	for (i=0;i<SupplyShip.size(); i++) if (SupplyShip[i]->Alive) SupplyShip[i]->ChangeRenderMode(Mode);
	for (i=0;i<CircleShip.size(); i++) if (CircleShip[i]->Alive) CircleShip[i]->ChangeRenderMode(Mode);
}

void CNavy::ReceiveMessage(RTDESK_CMsg *pMsg){

	switch (pMsg->Type)
	{
	case RTDESKMSG_NAVY:
		cMsgNavy *auxMsg;
			auxMsg = (cMsgNavy*)pMsg;
			switch (auxMsg->msgSubType)
			{
				case TIPOTURNALL:
					#ifdef DEF_RTD_TIME
					Game.TimerManager.Timers[TmrRTDSKMM].InitCounting();
					#endif

					for(unsigned int i=0;i<Navy.Ship.size();i++){
						cMsgShip *msg2Ships;
						msg2Ships = (cMsgShip*) GetMsgToFill(RTDESKMSG_SHIP);
						msg2Ships->msgSubType = TIPOTURN;
						SendMsg(msg2Ships,Navy.Ship[i],Navy.Ship[i]->TicksToUpdateShip/4);
					}

					#ifdef DEF_RTD_TIME
					EndAccCounting(TmrRTDSKMM);
					#endif
					break;




				case TIPOUPDSHIPS:
					break;
				case TIPOUPDSSHIPS:
					break;
				case TIPOUPDCSHIPS: 
					break;
			}
	case RTDESKMSG_BASIC_TYPE:
		break;
	}
}

/**   
   * @fn CNavy::Render
   * That depends if the ships is alive to be rendered
   * it can be do for all ships and supplyships en 3D 
*/
void CNavy::Render (void)
{
	unsigned int i;

	if (Application.Window.Antialiasing_active && CHAR_3D == Game.RenderMode) // Anti-aliasing
		glEnable(GL_MULTISAMPLE_ARB);

	for (i=0;i<Ship.size(); i++)
		if (Ship[i]->Alive)	//Although it is not active
			Ship[i]->AI->outEvent(CS_NameTransition[CS_DISPLAYING], NULL, (void*) Ship[i]); //v 2->2

	for(i=0;i<SupplyShip.size();i++)
		if (SupplyShip[i]->Alive)
			SupplyShip[i]->AI->outEvent(CSS_NameTransition[CSS_DISPLAYING], NULL, (void*) SupplyShip[i]);	//v 2->2

	for(i=0;i<CircleShip.size();i++)
		if (CircleShip[i]->Alive)
			CircleShip[i]->AI->outEvent(CCS_NameTransition[CCS_DISPLAYING], NULL, (void*) CircleShip[i]);	//v 2->2

	if (Application.Window.Antialiasing_active && CHAR_3D == Game.RenderMode) // Anti-aliasing
		glDisable(GL_MULTISAMPLE_ARB);
}


/**
   * @fn CNavy::GenerateDefaultShipAI
   * Start the artifical Inteligence of the Ship
   * The 'AIManager' receives the structure of the behaviors of the Ship objetcs.
   * This structure will be available each time this kind of object asks the 'AIManager' for its behavior.
   * Load the file of AI and start the analyzer for the interpretation
*/
void CNavy::GenerateDefaultShipAI	(void)	///Default Artificial Intelligence character initialization
{
	FSM *NewAI; ///<Declares the FSM to deliver to the AIManager 
	char AIPath[STRINGMAXLENGTH];

	//NEW CODE TO INITIALIZE SHIP FSM
	//-----------------------------
	CFile					HTMLFileSh;		//File to parser in the initialization
	FSMReader				readerSh;		//The parser used to understand the contents of the FSM file
	CLiteHTMLReader			HTMLReaderSh;		//The real HTML reader

	//INITIALIZE PARSER FILE
	Game.GenerateFullAIName (AIPath, SIAI_SHIP_AI_LOG_FILE);

	readerSh.Init		(AIPath);
	readerSh.Title		= Title;
	readerSh.Version	= Version;

	//ADD FUNCTIONS TO BE USED BY THE FSM
	readerSh.addPublicFunction("NULL",NULL);
	readerSh.addPublicFunction("init_ship",(mFuncPtr)init_ship);
	readerSh.addPublicFunction("CS_Move",(mFuncPtr)CS_Move);
	readerSh.addPublicFunction("CS_Display",(mFuncPtr)CS_Display);
	readerSh.addPublicFunction("CS_Bursting",(mFuncPtr)CS_Bursting);
	readerSh.addPublicFunction("CS_Kill",(mFuncPtr)CS_Kill);
	HTMLReaderSh.setEventHandler(&readerSh);

	Game.GenerateFullAIName (AIPath, SIAI_SHIP_AI_FILE);

	if (HTMLFileSh.Open(_T(AIPath), CFile::modeRead))
	{
		HTMLReaderSh.ReadFile((HANDLE) HTMLFileSh.m_hFile);
		HTMLFileSh.Close();
	}
	
	NewAI = new FSM();
	NewAI->addStates(readerSh.getFSMStatesVector(),readerSh.getFSMNumStates()); ///<Sets the 'Ship' FSM states
	NewAI->SetState(readerSh.getFSMInitState()); 
	AIManager.SetAI(NewAI,SIAI_SHIP_DEFAULT);  ///<Informs the AIManager about the FSM that the 'Ship' must have
}

/**
   * @fn CNavy::GenerateDefaultSupplyShipAI
   * Start the artifical Inteligence of the 'SupplyShip'
   * The 'AIManager' receives the structure of the behaviors of the 'SupplyShip' objetcs.
   * This structure will be available each time this kind of object asks the 'AIManager' for its behavior.
   * Load the file of AI and start the analyzer for the interpretation
*/
void CNavy::GenerateDefaultSupplyShipAI	(void)	///Default Artificial Intelligence character initialization
{
	FSM *NewAI; ///<Declares the FSM to deliver to the AIManager 
	char AIPath[STRINGMAXLENGTH];

	//NEW CODE TO INITIALIZE SUPPLYSHIP FSM
	//-------------------------------------
	CFile					HTMLFileSSh;		//File to parser in the initialization
	FSMReader				readerSSh;		//The parser used to understand the contents of the FSM file
	CLiteHTMLReader			HTMLReaderSSh;		//The real HTML reader

	//INITIALIZE PARSER FILE

	Game.GenerateFullAIName(AIPath, SIAI_SUPPLYSHIP_AI_LOG_FILE);

	readerSSh.Init		(AIPath);
	readerSSh.Title		= Title;
	readerSSh.Version	= Version;

	//ADD FUNCTIONS TO BE USED BY THE FSM
	readerSSh.addPublicFunction("NULL",NULL);
	readerSSh.addPublicFunction("CSS_FSM_Move",(mFuncPtr)CSS_FSM_Move);
	readerSSh.addPublicFunction("CSS_display",(mFuncPtr)CSS_display);
	readerSSh.addPublicFunction("CSS_FSM_Healthing",(mFuncPtr)CSS_FSM_Healthing);
	readerSSh.addPublicFunction("CSS_FSM_Dye",(mFuncPtr)CSS_FSM_Dye);
	readerSSh.addPublicFunction("CSS_FSM_Death",(mFuncPtr)CSS_FSM_Death);
	HTMLReaderSSh.setEventHandler(&readerSSh);
	
	Game.GenerateFullAIName (AIPath, SIAI_SUPPLYSHIP_AI_FILE);
	if (HTMLFileSSh.Open(_T(AIPath), CFile::modeRead))
	{
		HTMLReaderSSh.ReadFile((HANDLE) HTMLFileSSh.m_hFile);
		HTMLFileSSh.Close();
	}

	NewAI = new FSM();
	NewAI->addStates(readerSSh.getFSMStatesVector(),readerSSh.getFSMNumStates()); ///<Sets the 'SupplyShip' FSM states
	NewAI->SetState(readerSSh.getFSMInitState()); 
	AIManager.SetAI(NewAI,SIAI_SUPPLYSHIP_DEFAULT);  ///<Informs the AIManager about the FSM that the 'SupplyShip' must have
}

/**
   * @fn CNavy::GenerateDefaultCircleShipAI
   * Start the artifical Inteligence of the 'CircleShip'
   * The 'AIManager' receives the structure of the behaviors of the 'CircleShip' objetcs.
   * This structure will be available each time this kind of object asks the 'AIManager' for its behavior.
   * Load the file of AI and start the analyzer for the interpretation
*/
void CNavy::GenerateDefaultCircleShipAI	(void)	///Default Artificial Intelligence character initialization
{
	FSM *NewAI; ///<Declares the FSM to deliver to the AIManager
	char AIPath[STRINGMAXLENGTH];

	//NEW CODE TO INITIALIZE CIRCLESHIP FSM
	//-------------------------------------
	CFile					HTMLFileCSh;	//File to parser in the initialization
	FSMReader				readerCSh;		//The parser used to understand the contents of the FSM file
	CLiteHTMLReader			HTMLReaderCSh;	//The real HTML reader

	//INITIALIZE PARSER FILE
	Game.GenerateFullAIName (AIPath, SIAI_CIRCLESHIP_AI_LOG_FILE);

	readerCSh.Init		(AIPath);
	readerCSh.Title		= Title;
	readerCSh.Version	= Version;

	//ADD FUNCTIONS TO BE USED BY THE FSM
	readerCSh.addPublicFunction("NULL",NULL);
	readerCSh.addPublicFunction("CCS_FSM_Move",(mFuncPtr)CCS_FSM_Move);
	readerCSh.addPublicFunction("CCS_display",(mFuncPtr)CCS_display);
	readerCSh.addPublicFunction("CCS_FSM_Healthing",(mFuncPtr)CCS_FSM_Healthing);
	readerCSh.addPublicFunction("CCS_FSM_Dye",(mFuncPtr)CCS_FSM_Dye);
	readerCSh.addPublicFunction("CCS_FSM_Death",(mFuncPtr)CCS_FSM_Death);
	HTMLReaderCSh.setEventHandler(&readerCSh);

	Game.GenerateFullAIName (AIPath, SIAI_CIRCLESHIP_AI_FILE);
	if (HTMLFileCSh.Open(_T(AIPath), CFile::modeRead))
	{
		HTMLReaderCSh.ReadFile((HANDLE) HTMLFileCSh.m_hFile);
		HTMLFileCSh.Close();
	}

	NewAI = new FSM();
	NewAI->addStates(readerCSh.getFSMStatesVector(),readerCSh.getFSMNumStates()); ///<Sets the 'CircleShip' FSM states
	NewAI->SetState(readerCSh.getFSMInitState()); 
	AIManager.SetAI(NewAI,SIAI_CIRCLESHIP_DEFAULT);  ///<Informs the AIManager about the FSM that the 'CircleShip' must have
}

/**
   * @fn CNavy::AI_Init
   * Start the artifical Inteligence of the 'Ship' and 'SupplyShip'
   * The 'AIManager' receives the structure of the behaviors of the 'Ship' and 'SupplyShip' objetcs.
   * This structure will be available each time this kind of object asks the 'AIManager' for its behavior.
   * Load the file of IA and start the analyzer for the interpretation
*/
void CNavy::AI_Init	(void)	///Default Artificial Intelligence character initialization
{
	GenerateDefaultShipAI();
	GenerateDefaultSupplyShipAI();
	GenerateDefaultCircleShipAI();
}

/**
   * @fn CNavy::AI_Dye
   * Kill the IA of the 'Ship' and 'SupplyShip'
   * Change the status to false for end AI
*/
void CNavy::AI_Dye (void)
{
	Active = Alive  = false;
}

/**
   * @fn CNavy::AI_Go2Play
   * Go to Play IA
   * This function through the enemies ships to attack
*/
void CNavy::AI_Go2Play(void)
{
	unsigned int i;

	for(i=0;i<Ship.size();i++){
		Ship[i]->AI->outEvent(CS_NameTransition[CS_GROWING], NULL, (void*) Ship[i]);  //v 1->2
	}

	for (i=0; i<SupplyShip.size(); i++){
		SupplyShip[i]->AI->outEvent(CSS_NameTransition[CSS_GETTING_ALIVE], NULL, (void*) SupplyShip[i]);	//v 1->2
	}

	for (i=0; i<CircleShip.size(); i++){
		CircleShip[i]->AI->outEvent(CCS_NameTransition[CCS_GETTING_ALIVE], NULL, (void*) CircleShip[i]);	//v 1->2
	}
}



