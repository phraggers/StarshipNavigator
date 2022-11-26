
#ifndef _SN_APPSTATE_H_
#define _SN_APPSTATE_H_

typedef struct
{
	void* Data; // byte where user data starts
	void* DataEnd; // last byte of usable memory (do not access beyond this byte)

	SDL_version AppVersion;
	SDL_version SDLVersion;
	SDL_version MixVersion;
	SDL_version NetVersion;
	SDL_version GLVersion;

	char* PrefPath;
	char* BasePath;
	int ArgC;
	char** ArgV;

	SN_Timer Timer;
	SN_Input Input;
	SN_Window Window;
	//SN_Audio Audio;

	bool Running;
} SN_AppState;

// Pass 0 for default (sizeof SN_AppState + DEF_ALLOCSIZE)
void* State_Init(u64 _Size);
void State_Quit(void);

#endif //_SN_APPSTATE_H_