
#define SN_MAIN
#include "StarshipNavigator.h"

u64 SN_GetCurrentStackSize(void)
{
	int x = 0;
	return SN_TopOfTheStack - (u64)&x;
}

static bool Main_Init(int _argc, char** _argv)
{
	State = (SN_AppState*)State_Init(0);

	if (!State)
	{
		return 0;
	}

	if (!Log_Init())
	{
		return 0;
	}

	Timer_Init(60);
	int StartupTimer = Timer_Add();

	//char* env = SDL_getenv("USERNAME");
	//SN_Log(LOGLEVEL_INFO, "MAIN: Login[%s]", ((env) ? env : "None"));
	//if (env) SDL_free(env);

	State->AppVersion.major = DEF_APPVER_MAJ;
	State->AppVersion.minor = DEF_APPVER_MIN;
	State->AppVersion.patch = DEF_APPVER_PAT;
	State->PrefPath = SDL_GetPrefPath(DEF_APPORG, DEF_APPNAME);
	State->BasePath = SDL_GetBasePath();
	State->ArgC = _argc;
	State->ArgV = _argv;

	SN_Log(LOGLEVEL_INFO, "MAIN: %s %d.%d.%d",
			DEF_APPNAME, State->AppVersion.major, State->AppVersion.minor, State->AppVersion.patch);
	SN_Log(LOGLEVEL_INFO, "MAIN: PrefPath: %s", State->PrefPath);
	SN_Log(LOGLEVEL_INFO, "MAIN: BasePath: %s", State->BasePath);
	Log_SetLogFilePath();

	// SDL 2
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			SN_Log(LOGLEVEL_FATAL, "MAIN: Failed to start SDL2: %s", SDL_GetError());
			return 0;
		}

		SDL_VERSION(&State->SDLVersion);
		SN_Log(LOGLEVEL_INFO, "MAIN: SDL %d.%d.%d", (int)State->SDLVersion.major,
				(int)State->SDLVersion.minor, (int)State->SDLVersion.patch);
	}

	// SDL Mixer
	{
		if (!Mix_Init(MIX_INIT_OGG))
		{
			SN_Log(LOGLEVEL_FATAL, "MAIN: SDLMixer Init Error: %s", Mix_GetError());
			return 0;
		}

		MIX_VERSION(&State->MixVersion);
		SN_Log(LOGLEVEL_INFO, "MAIN: SDLMixer %d.%d.%d", (int)State->MixVersion.major,
				(int)State->MixVersion.minor, (int)State->MixVersion.patch);

		if (Mix_OpenAudio(DEF_AUDIO_FREQ, DEF_AUDIO_FORMAT, DEF_AUDIO_CHANNELS, DEF_AUDIO_CHUNKSIZE) < 0)
		{
			SN_Log(LOGLEVEL_FATAL, "MAIN: Failed to open Audio Device: %s", Mix_GetError());
			return 0;
		}

		SN_Log(LOGLEVEL_INFO, "MAIN: AudioDevice Opened: Freq[%d] Format[%d] Channels[%d] Chunksize[%d]",
				DEF_AUDIO_FREQ, DEF_AUDIO_FORMAT, DEF_AUDIO_CHANNELS, DEF_AUDIO_CHUNKSIZE);
	}

	// SDL Net
	{
		if (SDLNet_Init() < 0)
		{
			SN_Log(LOGLEVEL_FATAL, "MAIN: SDLNet Init Error: %s", SDLNet_GetError());
			return 0;
		}

		SDL_NET_VERSION(&State->NetVersion);
		SN_Log(LOGLEVEL_INFO, "MAIN: SDLNet %d.%d.%d", (int)State->NetVersion.major,
				(int)State->NetVersion.minor, (int)State->NetVersion.patch);

	}

	// SDL Config
	if (SDL_JoystickEventState(SDL_ENABLE) != 1)
	{
		SN_Log(LOGLEVEL_ERROR, "MAIN: Failed to enable SDL_JoystickEventState: %s", SDL_GetError());
	}
	else
	{
		SN_Log(LOGLEVEL_DEBUG, "MAIN: JoystickEventState[%d]", SDL_JoystickEventState(SDL_QUERY));
	}

	// Window
	State->Window.Dimensions.w = DEF_WINDOW_WIDTH;
	State->Window.Dimensions.h = DEF_WINDOW_HEIGHT;

	if (!Window_Init())
	{
		SN_Log(LOGLEVEL_FATAL, "MAIN: Failed to create Window!");
		return 0;
	}

	State->Running = 1;

	SN_Log(LOGLEVEL_INFO, "MAIN: Startup took %.02f ms", Timer_End(StartupTimer));

	return 1;
}

static void Main_Quit(void)
{
	SN_Log(LOGLEVEL_DEBUG, "MAIN: Stack Size: %u", SN_GetCurrentStackSize());

	if (State->Window.Renderer)
	{
		SDL_DestroyRenderer(State->Window.Renderer);
		State->Window.Renderer = 0;
		SN_Log(LOGLEVEL_DEBUG, "MAIN: Renderer Destroyed");
	}

	if (State->Window.Handle)
	{
		SDL_DestroyWindow(State->Window.Handle);
		State->Window.Handle = 0;
		SN_Log(LOGLEVEL_DEBUG, "MAIN: Window Destroyed");
	}

	SDLNet_Quit();
	SN_Log(LOGLEVEL_DEBUG, "MAIN: SDLNet Quit");
	Mix_CloseAudio();
	SN_Log(LOGLEVEL_INFO, "MAIN: Audio Device Closed");
	Mix_Quit();
	SN_Log(LOGLEVEL_DEBUG, "MAIN: SDLMixer Quit");

	if ((SDL_WasInit(SDL_INIT_EVERYTHING)))
	{
		SN_Log(LOGLEVEL_INFO, "MAIN: SDL Quit");
		SDL_Quit();
	}

	Log_Quit();
	State_Quit();
}

int main(int argc, char** argv)
{
	{ int x = 0; SN_TopOfTheStack = (u64)&x; }
	if (!Main_Init(argc, argv)) return 1;
	SN_Log(LOGLEVEL_DEBUG, "MAIN: Stack Size: %u", SN_GetCurrentStackSize());

	State->Running = 1;

	SDL_Texture* bender_texture = 0;
	SDL_Rect* benderrect;
	{
		int width, height, comp;
		int req_comp = STBI_rgb_alpha;
		unsigned char* data = stbi_load("Atlas_BenderBold.png", &width, &height, &comp, req_comp);
		//unsigned char* data = stbi_load_from_memory(buffer, length, &width, &height, &comp, req_comp);

		if (!data)
		{
			SDL_Log("stbi Failed to load bender");
			return -1;
		}

		Uint32 rmask, gmask, bmask, amask;

	#if SDL_BYTEORDER  == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else // little endian, like x86
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif

		int depth = 32;
		int pitch = 4 * width;

		SDL_Surface* bender_surface = SDL_CreateRGBSurfaceFrom((void*)data, width, height, depth, pitch,
															   rmask, gmask, bmask, amask);
		if (!bender_surface)
		{
			SDL_Log("failed to create bender surface, %s", SDL_GetError());
			stbi_image_free(data);
			return -1;
		}

		bender_texture = SDL_CreateTextureFromSurface(State->Window.Renderer, bender_surface);
		SDL_FreeSurface(bender_surface);
		stbi_image_free(data);

		if (!bender_texture)
		{
			SDL_Log("failed to create bender surface, %s", SDL_GetError());
			return -1;
		}

		benderrect = (SDL_Rect*)State_Alloc(sizeof(SDL_Rect) * 127, 0);
		if (benderrect)
		{
			SDL_RWops* benderfile = SDL_RWFromFile("BenderBold.atlas", "rb");
			if (benderfile)
			{
				benderfile->read(benderfile, benderrect, 1, sizeof(SDL_Rect) * 127);
				benderfile->close(benderfile);
			}
		}
	}

	while (State->Running)
	{
		Timer_FrameStart();
		Input_HandleEvents();
		SDL_SetRenderDrawColor(State->Window.Renderer, 0, 0, 0, 0xff);
		SDL_RenderClear(State->Window.Renderer);

		for (int i = 0x20; i < 0x7f; i++)
		{
			if (Input_KeyDown(i) && benderrect)
			{
				SDL_RenderCopy(State->Window.Renderer, bender_texture, &benderrect[i], 0);
			}
		}

		SDL_RenderPresent(State->Window.Renderer);
		Timer_FrameEnd();
	}

	return 0;
}