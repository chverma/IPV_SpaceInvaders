/**	Methods to play Space Invaders with kinect
*	Prefix: KNT_
*	@Author: Jordi Mansanet
*	@version: 2011-12
**/

#include <Kinect.h> 

#ifdef CHW_USE_KINECT

char KNT_Msgs[KNT_ERROR_MAX_MSG][KNT_MSJ_MAX_LONG] = 
{
	"WARNING: nItem = -1",
	"Slider: Bad direction for selection: %s\n",
	"WARNING: wrong direction",
	"LEFT !!",
	"CENTER !!",
	"RIGHT !!",
	"WARNING: wrong value on slider",
	"Init From Xml",
	"Init From Xml File",
	"Find depth generator",
	"Couldn't initialize the Session Manager: %s\n",
	"Please perform wave gesture to start session\n",
	"Hit any key to exit\n"
};

// Global variables for teh Kinect
XnVSessionManager* g_pSessionManager = NULL;
XnVFlowRouter* g_pMainFlowRouter;
XnVSelectableSlider1D* g_pMainSlider;
XnBool g_bActive = false;
XnBool g_bIsInput = false;
XnBool g_bInSession = false;
XnFloat g_fValue = 0.5f;
xn::Context g_Context;
xn::ScriptNode g_ScriptNode;
xn::DepthGenerator g_DepthGenerator;
XnBool g_bQuit = false;
bool g_session_started = false;
bool g_init_kinect = false;

K_SHIP_MOVE g_dir = K_STOP_MOVE; //Posicion por defecto del Kinect

/**   
	* @fn MainSlider_OnSelect
	* Callback Kinect directory
    * @param XnInt32 nItem input variable to the function
    * @param dir XnVDirection input variable of the direction
    * @param cxt void input variable representing the kinect
    * It checks if there nItem and direction is upward and onward
*/
void XN_CALLBACK_TYPE MainSlider_OnSelect(XnInt32 nItem, XnVDirection dir, void* cxt)
{
	if (nItem == -1) 
	{
		printf(KNT_Msgs[KNT_BAD_ITEM]);
	}
	else if (dir == DIRECTION_UP || dir == DIRECTION_FORWARD)
	{
		printf(KNT_Msgs[KNT_SLIDER_BAD_DIR], XnVDirectionAsString(dir));
	}
	else
	{
		printf(KNT_Msgs[KNT_WRONG_DIR]);
	}
}

/**   
	* @fn MainSlider_OnValueChange
	* Call Kinect movements
    * @ Param XnFloat fValue input variable for the movement
    * @ Param cxt void input variable representing the kinect
    * FValue between ranges is checked to determine the direction of movement left, right or center
*/
void XN_CALLBACK_TYPE MainSlider_OnValueChange(XnFloat fValue, void* cxt)
{
	char Aux[64];

	g_bActive = true;
	g_bIsInput = true;
	g_fValue = fValue;
	
	if (fValue >= 0 && fValue <= 0.333)
	{
		strcpy(Aux, KNT_Msgs[KNT_LEFT]);
		g_dir = K_LEFT_MOVE;
	}
	else if (fValue > 0.333 && fValue <= 0.666)
	{
		strcpy(Aux, KNT_Msgs[KNT_CENTER]);
		g_dir = K_STOP_MOVE;
	}
	else if (fValue > 0.666 && fValue <= 1.0)
	{
		strcpy(Aux, KNT_Msgs[RIGHT]);
		g_dir = K_RIGHT_MOVE;
	}
	else
	{
		strcpy(Aux, KNT_Msgs[KNT_WRONG_VALUE_SLIDER]);
	}
	strcat(Aux, "\t\t%f\n"); 
	printf(Aux,fValue);
}

/**   
	* @fn MainSlider_OnActivate
	* Activity Recognition and Kinect
    * @param cxt void input variable representing the kinect
    * Enable status Kinect
*/
void XN_CALLBACK_TYPE MainSlider_OnActivate(void* cxt)
{
	g_session_started = true;
	g_bActive = true;
	g_bIsInput = false;
}

/**   
	* @fn MainSlider_OnDeactivate
	* No Recognition and deactivated the Kinect
    * @param cxt void input variable representing the kinect
    * Turns off the state of Kinect
*/
void XN_CALLBACK_TYPE MainSlider_OnDeactivate(void* cxt)
{
	g_bActive = false;
	g_bIsInput = false;
}
/**   
	* @fn MainSlider_OnPrimaryCreate
	* Create entry of Kinect
    * @param cxt void input variable representing the kinect
    * Enable input Kinect
*/
void XN_CALLBACK_TYPE MainSlider_OnPrimaryCreate(const XnVHandPointContext* hand, const XnPoint3D& ptFocus, void* cxt)
{
	g_bIsInput = true;
}
/**   
	* @fn MainSlider_OnPrimaryDestroy
	* Destroy the entrance of Kinect
    * @param cxt void input variable representing the kinect
    * Allows entry Kinect destroy
*/
void XN_CALLBACK_TYPE MainSlider_OnPrimaryDestroy(XnUInt32 nID, void* cxt)
{
	g_bIsInput = false;
}

/**   
	* @fn SessionStart
	* Start a session for the kinect
    * @param void UserCxt input variable representing the user's kinect
    * Starts a user session for Kinect
*/
void XN_CALLBACK_TYPE SessionStart(const XnPoint3D& pFocus, void* UserCxt)
{ 
	g_bInSession = true;
	g_pMainFlowRouter->SetActive(g_pMainSlider);
}

/**   
	* @fnSessionEnd
	* Close the session for the kinect
    * @param void UserCxt input variable representing the user's kinect
    * Allows user to close the session for Kinect
*/
void XN_CALLBACK_TYPE SessionEnd(void* UserCxt)
{
	g_bInSession = false;
	g_pMainFlowRouter->SetActive(NULL);
}

/**   
	* @fn fileExists
	* There Archives
    * @param char fn represents the names of the files
    * Lets you know if the library file or returning the existence kinect
*/
XnBool fileExists(const char *fn)
{
	XnBool exists;
	xnOSDoesFileExist(fn, &exists);
	return exists;
}

/**   
	* @fn initKinect
	* Start Kinect
    * This is where you start the kinect and s are checked for connection of the same
    * If you activate the state variables and unicia session for Kinect
    * Starts motion capture
    * The flow of movements
    * It is finished start the kinect once you register a movement with the same
*/
int initKinect()
{
	XnStatus rc = XN_STATUS_OK;
	xn::EnumerationErrors errors;

	rc = g_Context.InitFromXmlFile(SAMPLE_XML_FILE, g_ScriptNode, &errors);
	CHECK_ERRORS(rc, errors, KNT_Msgs[KNT_INIT_FROM_XML_FILE]);
	CHECK_RC(rc, KNT_Msgs[KNT_INIT_FROM_XML]);

	rc = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
	CHECK_RC(rc, KNT_Msgs[KNT_FIND_DEPTH_GEN]);

	// Create and initialize point tracker
	g_pSessionManager = new XnVSessionManager;
	rc = g_pSessionManager->Initialize(&g_Context, "Wave", "RaiseHand");
	if (rc != XN_STATUS_OK)
	{
		printf(KNT_Msgs[KNT_COULD_NOT_INIT_SESSION], xnGetStatusString(rc));
		delete g_pSessionManager;
		return rc;
	}

	g_pSessionManager->RegisterSession(NULL, &SessionStart, &SessionEnd);

	// Start catching signals for quit indications
	CatchSignals(&g_bQuit);

	// Create and initialize the main slider
	g_pMainSlider = new XnVSelectableSlider1D(3);
	g_pMainSlider->RegisterItemSelect(NULL, &MainSlider_OnSelect);
	g_pMainSlider->RegisterActivate(NULL, &MainSlider_OnActivate);
	g_pMainSlider->RegisterDeactivate(NULL, &MainSlider_OnDeactivate);
	g_pMainSlider->RegisterPrimaryPointCreate(NULL, &MainSlider_OnPrimaryCreate);
	g_pMainSlider->RegisterPrimaryPointDestroy(NULL, &MainSlider_OnPrimaryDestroy);
	g_pMainSlider->RegisterValueChange(NULL, &MainSlider_OnValueChange);
	g_pMainSlider->SetValueChangeOnOffAxis(true);

	// Creat the flow manager
	g_pMainFlowRouter = new XnVFlowRouter;

	// Connect flow manager to the point tracker
	g_pSessionManager->AddListener(g_pMainFlowRouter);

	g_Context.StartGeneratingAll();

	g_init_kinect = true;

	printf(KNT_Msgs[KNT_WAVE_GESTURE]);
	printf(KNT_Msgs[KNT_HIT_ANY_2_EXIT]); 

	return 0;
}

/**   
	* @fn waitKinect
	* Wait Kinect
    * This function defines the standby state kinect
    * While not made any motion is put on hold
*/
bool waitKinect()
{
	if (g_init_kinect == true)
	{
		// Read next available data
		g_Context.WaitOneUpdateAll(g_DepthGenerator);

		// Process the data
		g_pSessionManager->Update(&g_Context);	
	}	
	return g_session_started;
}

/**   
   * @fn updateKinect
   * update de movements of Kinect
*/
K_SHIP_MOVE updateKinect()
{
	if (g_session_started == true)
	{
		g_init_kinect = false;
		// Read next available data
		g_Context.WaitOneUpdateAll(g_DepthGenerator);

		// Process the data
		g_pSessionManager->Update(&g_Context);
	}
	
	return g_dir;
}

/**   
	* @fn CleanupExit
	* Clean Kinect
    * Function for when you finish using the kinect
    * Closes the session user deletes Kinect flow movements
*/

void CleanupExit()
{
	if (NULL != g_pSessionManager) {
		delete g_pSessionManager;
		g_pSessionManager = NULL;
	}

	delete g_pMainFlowRouter;
	delete g_pMainSlider;

}

#endif
