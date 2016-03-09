//Class created as an extension of Global.h for execution specifications purpose

#define DEF_CONTINUOUS_WITH_SIMULATE_TIME

//*****************************	Controles de seguimiento y análisis de mediciones ***********************
#define DEF_EXP_CSV_FILE					//Ejecución limitada por numero de pasos y genera archivo csv con toma de tiempos
	#ifdef DEF_EXP_CSV_FILE
	#define DEF_MAX_TICKS 82998900			//Ticks que dura la ejecución si se activa DEF_EXP_CSV_FILE.
	#define DEF_MIN_TICKS 27666300			//Ticks a partir de los cuales se empiezan a registrar datos si se activa DEF_EXP_CSV_FILE.
											//82998900->30seg  165997800->60seg  16599780->6seg 27666300->10seg 55332600->20seg
											//(portatil casa)66241110->30seg  132482220->60seg  13248222->6seg 22080370->10seg 44160740->20seg
	#define DEF_END_IN_FRAME	1500		//Finaliza ejecución cuando realiza el numero de frame indicado
	//#define DEF_LEC_PARTIAL				//Para activar toma de lecturas parciales

	#endif

//#define DEF_RND_TIME					//Activa la medición del tiempo de render.
//#define DEF_UPD_TIME					//Activa la medición del tiempo de update.
//#define DEF_IDL_TIME					//Activa la medición del tiempo de idle.
//#define DEF_RTD_TIME					//Activa la medición del tiempo de rt-desk.

//****************************************** No tocar **********************************************

#define DEF_LONG_ARRAYS 30000			//Longitud de los arrays utilizados para medir parciales