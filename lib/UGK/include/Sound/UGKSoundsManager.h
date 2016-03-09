/** Definition of the class Music Manager for the UPV Game Kernel
*  General class to manage all the sounds
*  Prefix: UGKCSM_

*	@author Ramon Molla, Carlos Martinez Perez
*	@NewVersion Ramón Mollá 2014-05 - API conversion and refactoring

*/

#ifndef UGKCSM_SOUND
#define UGKCSM_SOUND

#include <UGKSound.h>
#include <UGKMusic.h>
#include <UGKLog.h>
#include <vector>
#include <string>

//For the error msg displayed
#define UGKCSM_ERR_MAX_LONG 128
#define UGKCSM_FADING_STEP_DURATION 30	//Time lasting every step up or down while Fading up or down a music/sound
#define UGKCSM_FADING_STEP 5			//Amount of power a sound/music can be increased/decreased in power while Fading it up or down
#define UGKCSM_NO_INDEX -1			//Amount of power a sound/music can be increased/decreased in power while Fading it up or down

//Sampling Rates in Hz
#define CSM_CD_SAMPLING_RATE 44100	///< CD Sampling rate 

//Number of final audio channels (loudspeakers) listened by humans
#define CSM_STEREO 2	///< For earphones, stereo devices,...

//Amount of bytes used to allocate temporaly any sound or part of sound
#define CSM_DEFAULT_SOUND_BUFFER 4096	///< 4KB

extern char *CSM_ErrorString[FMOD_ERR_MAX];

namespace UGK
{
	// *.PLS (WINAMP PLAYLIST) PARSER FOR STREAMS
	typedef struct
	{
		int    count;
		void **name;
		void **displayname;
	} Playlist;

	typedef enum
	{
		CSM_MUSIC,	///<Music
		CSM_SOUNDS,	///<Sound
		CSM_VOCAL,	///<Vocals
		CSM_MAX_SOUND_TRACKS	///<For accounting purpouses only
	}CSM_SOUND_TRACKS;

#ifdef UGKSND_FMOD375
#elif defined(UGKSND_FMODEx)
#elif defined(UGKSND_OAL)
#elif defined(UGKSND_SDL)
	enum SDL_ERRORS 
	{
		SDL_ERR_NONE,             /* No errors */
		SDL_ERR_UNKNOWN,          /* Error unknown */
		SDL_ERR_BUSY,             /* Cannot call this command after FSOUND_Init.  Call FSOUND_Close first. */
		SDL_ERR_UNINITIALIZED,    /* This command failed because FSOUND_Init or FSOUND_SetOutput was not called */
		SDL_ERR_INIT,             /* Error initializing output device. */
		SDL_ERR_ALLOCATED,        /* Error initializing output device, but more specifically, the output device is already in use and cannot be reused. */
		SDL_ERR_PLAY,             /* Playing the sound failed. */
		SDL_ERR_OUTPUT_FORMAT,    /* Soundcard does not support the features needed for this soundsystem (16bit stereo output) */
		SDL_ERR_COOPERATIVELEVEL, /* Error setting cooperative level for hardware. */
		SDL_ERR_CREATEBUFFER,     /* Error creating hardware sound buffer. */
		SDL_ERR_FILE_NOTFOUND,    /* File not found */
		SDL_ERR_FILE_FORMAT,      /* Unknown file format */
		SDL_ERR_FILE_BAD,         /* Error loading file */
		SDL_ERR_MEMORY,           /* Not enough memory or resources */
		SDL_ERR_VERSION,          /* The version number of this file format is not supported */
		SDL_ERR_INVALID_PARAM,    /* An invalid parameter was passed to this function */
		SDL_ERR_NO_EAX,           /* Tried to use an EAX command on a non EAX enabled channel or output. */
		SDL_ERR_CHANNEL_ALLOC,    /* Failed to allocate a new channel */
		SDL_ERR_RECORD,           /* Recording is not supported on this machine */
		SDL_ERR_MEDIAPLAYER,      /* Windows Media Player not installed so cannot play wma or use internet streaming. */
		SDL_ERR_CDDEVICE,         /* An error occured trying to open the specified CD device */
		SDL_ERR_MAX				  /* For accounting purpouses only. This error has to be always the last one*/
	};

//Sound files format supported
#define CSM_ASK_SOUND_FILES 0						///< For asking which sound files are supported
#define CSM_FLAC_SOUND_FILES MIX_INIT_FLAC			///< Support FLAC sound files
#define CSM_MOD_SOUND_FILES MIX_INIT_MOD			///< Support MOD sound files
#define CSM_MP3_SOUND_FILES MIX_INIT_MP3			///< Support MP3 sound files
#define CSM_OGG_SOUND_FILES MIX_INIT_OGG			///< Support OGG sound files
#define CSM_SYNTH_SOUND_FILES MIX_INIT_FLUIDSYNTH	///< Support SYNTH sound files

#endif

	class UGK_API CSoundsManager
	{
	//Attributes
	private:
	
		char *TrackPath;	///< Default path of the sound
		char *MusicPath;	///< Default path of the music

	public:
		std::vector<CSound>		Sound;	///<An array where the sounds of the game are
	
		bool			PlayingMusic;		// MUSIC ON/OFF
		CM_MUSIC_TYPE	music_type;			// music type

		int				currentMusicIndex;
	
		// SCROLL MUSIC TEXT
		float			scroll_music_x;		// SCROLL COORD X
		int				scroll_music_sens;	// SCROLL SENS DE DEPLACEMENT
		float			scroll_music_total;	// SCROLL TOTAL DEPLACEMENT

		#ifdef UGKSND_FMOD375

			FMUSIC_MODULE * FadingModule;			// Music fading in or out

			///ACHTUNG: Integrar estas variables como atributos del gestor de sonidos
			int				MP3SoundChannel;		// Sound channel assigned to the mp3 sound reproduction
		
			UGKSND_SoundType StreamChannel;			// Streaming channel
			FMUSIC_MODULE * MODModule;				// Module for MOD sounds

			// FMOD CALLBACKS
			signed short	*OscBuffer;
			int				 OscBlock ;

			FSOUND_DSPUNIT* dspUnitSounds;			///<DSP used for the sounds not seen at the spectrum
			FSOUND_DSPUNIT* dspUnitSpectrum;		///<DSP SPECTRUM (UNIT)
			CSound			*music;

		#elif defined (UGKSND_FMODEx)
		#elif defined (UGKSND_OAL)
			CSound			*music;
		#elif defined (UGKSND_SDL)
			UGKSND_MusicType	*music;
		#endif

		int Volume;		///<The intensity of the sound BY DEFAULT 0 no sound, 255 maximun power (in SDL the max vol is 128).

		int NSounds; ///<Number of sounds
	
		//Methods
		CSoundsManager	(){};
		~CSoundsManager	();

		void Init();

		void		SetTrackPath	(char *name);
		void		SetMusicPath	(char *name);
		inline char *GetTrackPath	() {return TrackPath;}
		inline char *GetMusicPath	() {return MusicPath;}
		void		LoadMusic		(const char* file); ///<Loads a music file in memory

		void		StopMusic(void);
		void		NextMusicTrack(void);

		Playlist *ParsePlaylist			(char *name);

		void LoadSounds					(int Size, std::string SoundsFiles[]); ///<Loads all the different sounds that can be performed at any time during the playing of the game
		char *ErrorMsg					(const int);

		void StreamFadingIn();

		#ifdef UGKSND_FMOD375
			void MusicFadingOut			(void);
			void MusicFadingIn			(void);
			void fmodExtract			(void);

			inline void Play			(const unsigned char s, int vol){Sound[s].Play(vol);} ///<Plays the sound s
			inline char *GetErrorMsg	()								{return CSM_ErrorString[FSOUND_GetError()];}
			inline void Close			()								{FSOUND_Close();}
		#elif defined (UGKCSND_OAL)
			void MusicFadingIn			(void);		
			void MusicFadingOut			(void);

			inline void Play			(const unsigned char s, int vol){Sound[s].Play(vol);}; ///<Plays the sound s
			inline void PauseMusic		(void){ if(PlayingMusic)		{ music->Pause(); PlayingMusic=false;}}
			inline void ResumeMusic		(void){ if(!PlayingMusic)		{ music->Play(); PlayingMusic=true;}}
			inline char *GetErrorMsg	();
		#elif defined (UGKSND_SDL)
			inline void MusicFadingIn	(void){ if(!PlayingMusic)		{ Mix_FadeInMusic(music, 0, 2000); PlayingMusic = true; }}	///<Fades in the current music
			inline void MusicFadingOut	(void)							{ Mix_FadeOutMusic(2000); StopMusic(); }	///<Fades out the current music

			inline void Play			(const unsigned char s, int vol){Sound[s].Play(vol);}; ///<Plays the sound s
			inline void PauseMusic		(void){ if(PlayingMusic)		{ Mix_PauseMusic(); PlayingMusic = false; }}	///<Pauses the current music
			inline void ResumeMusic		(void)							{if(!PlayingMusic){ Mix_ResumeMusic(); PlayingMusic = true; }} ///<Resumes the current music
			inline void CloseMixer		(void)							{Mix_CloseAudio(); PlayingMusic = false;} ///Close the mixer, halting all playing audio
			inline int	FilesSupported	(void)							{return Mix_Init(CSM_ASK_SOUND_FILES);}
			inline char *GetErrorMsg	()								{return Mix_GetError();}
			inline void Close			()								{Mix_CloseAudio();}

		#endif

		/**
		*	\fn void SetVolume	(const unsigned char s, int vol)
		*	Set a new Volume
		*	\param[in] s Tag of the sound to modify
		*	\param[in] s vol Volume of the sound
		*/
		inline void SetVolume	(const unsigned char s, int vol){Sound[s].SetVolume(vol);}
	
		/**
		*	\fn void SetVolume	(int vol)
		*	Set a new Volume in all the Sounds
		*	\param[in] s vol Volume of the sound
		*/

		void SetVolumeAll	(int vol) {for(unsigned int i=0;i<(int)Sound.size();i++) Sound[i].SetVolume(vol);}

		/**
		*	\fn void SetPosition	(const unsigned char s,int pos)
		*	Sets a new Position
		*	\param[in] s Tag of the sound to modify
		*	\param[in] s pos Position of the sound
		*/
		inline void SetPosition	(const unsigned char s,int pos){Sound[s].setPosition(pos);}

		/**
		*	\fn void setTrackPath(char *path)
		*	Set the Path of the Sound to the path that indicates the parameter
		*	\param[in] Track, Type of the track [UGKCSM_SOUND_TRACKS].
		*	\param[in] path, String of characters with the path to the track.
		*/
		inline void		SetTrackPath	(unsigned int Track, char* path) {Sound[Track].SetPath(path);}

		/**
		*	\fn void setTrackName(char *name)
		*	Set the Path of the Sound to the path that indicates the parameter
		*	\param[in] Track, Type of the track [UGKCSM_SOUND_TRACKS].
		*	\param[in] name, String of characters with the name to the track.
		*/
		inline void		SetTrackName	(unsigned int Track, char* name) {Sound[Track].SetName(name);}

		/**
		*	\fn const char* CSoundsManager::getTrackPath(unsigned int Track)
		*	Get the Path where the sounds are
		*	\param[in] Track, Type of the track [UGKCSM_SOUND_TRACKS].
		*/
		inline char* GetTrackPath	(unsigned int Track) {return Sound[Track].GetPath();}

		/**
		*	\fn const char* CSoundsManager::getTrackName(unsigned int Track)
		*	Get the Path where the sounds are
		*	\param[in] Track, Id of the track
		*/
		inline char* GetTrackName	(unsigned int Track) {return Sound[Track].GetName();}


		void		MusicStart		(void);
		int			SupportFiles	(int FilesMask);
	};
}

#endif