/**	Definition of the class UGKAudioHardware. Base class for all audio hardware in the system

*	Prefix: UGKSNDHW_

*	@author Leopoldo Pla 2014-10
*/

#ifndef UGKSNDHW_HARDWARE
#define UGKSNDHW_HARDWARE

#include <HRTimerManager.h>
#include <list>
#include <SDL_mixer.h>
#include <SDL.h>
#include <UGK.h>
#include <string.h>
//#include "Copyright.h"

namespace UGK
{

	/**
	*	UGKAudioHardware

	*	Manages all the information relative to audio hardware.
	*	It is implemented as a singleton. This is not the game singleton
	*/

	class UGKAudioHardware
	{
	private:

	public:

		cHRTimerManager HRTimerManager;		///<HR counters for delaying the sensitiveness of the device

		//Atributes
		/// Type of hardware: main soundcard, secondary soundcard...
		unsigned short int	NumberSoundCard;
		/// The hardware is active. Implicitly it needs to be alive in order to be active
		bool				Active;
		/// The name of the hardware given by the OS.
		char				*name;

		static Uint8 *audio_chunk;
		static Uint32 audio_len;
		static Uint8 *audio_pos;

		SDL_AudioSpec wanted,obtained;
		Mix_Chunk *sample;
		Mix_Music *music;


		//Methods
		
		//General
		void Init (void);
		UGKAudioHardware(){Init();};		///<Constructor of the class
		~UGKAudioHardware(){Mix_Quit();};		///<Destructor of the class
		void OpenAudio();
		void CheckAudioFormat();
		void CloseAudio();

		//Samples
		void LoadSample();
		int GetNumSampleDecoders();
		const char *GetSampleDecoder(int);
		int SetVolumeSample(int);
		void DeleteSample();

		//Channels
		void AllocateChannels(int);
		int SetChannelVolume(int,int);
		void PlayChannel (int,int,int);
		void PlayChannelTimed (int, int, int, int);
		void FadeInChannel (int, int, int, int);
		void FadeInChannelTimed (int, int, int, int, int);
		void PauseChannel (int);
		void ResumeChannel (int);
		void HaltChannel (int);
		void ExpireChannel (int, int);
		void FadeOutChannel (int, int);
		void SetChannelFinishedFunction(void (*channel_finished)(int channel));
		bool IsChannelPlaying(int);
		bool IsChannelPaused(int);


		//Groups
		int ReserveChannels(int);
		bool GroupChannel(int,int);
		int GroupChannel(int,int,int);
		int GroupCount(int);
		int GroupFindAvailable(int);
		int FadeOutGroup(int,int);
		void HaltGroup(int);


		//Music
		int GetNumMusicDecoders();
		const char* GetMusicDecoder(int index);
		void LoadMusic();
		void FreeMusic();
		void PlayMusic(int musicnum, int loops);
		void FadeInMusic(int musicnum, int loops, int ms);
		void FadeInMusicPosition(int musicnum, int loops, int ms, double position);
		void HookMusic(void (*mix_func)(void *udata, Uint8 *stream, int len),void *arg);
		int VolumeMusic(int volume);
		void PauseMusic();
		void ResumeMusic();
		void RewindMusic();
		void SetMusicPosition(double position);
		void SetMusicCommand(const char* command);
		void HaltMusic();
		void FadeOutMusic(int ms);
		void SetMusicFinishedFunction(void (*music_finished)());
		bool IsMusicPlaying();
		bool IsMusicPaused();


		//Effects
		bool SetPanning(int channel, Uint8 left, Uint8 right);
		bool SetDistance(int channel, Uint8 distance);
		bool SetPosition(int channel, Sint16 angle, Uint8 distance);
		bool SetReverseStereo(int channel, int flip);

	};
} // Namespace

#endif