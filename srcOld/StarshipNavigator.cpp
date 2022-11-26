
#define SN_MAIN
#include "StarshipNavigator.h"

static bool Main_Init(int _argc, char** _argv)
{
	State = (SN_AppState*)State_Init(0);

	if (!State)
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

	// SDL 2
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error: SDL_Init", SDL_GetError(), 0);
			return 0;
		}

		SDL_VERSION(&State->SDLVersion);
	}

	// SDL Mixer
	{
		if (!Mix_Init(MIX_INIT_OGG))
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error: Mix_Init", Mix_GetError(), 0);
			return 0;
		}

		MIX_VERSION(&State->MixVersion);

		if (Mix_OpenAudio(DEF_AUDIO_FREQ, DEF_AUDIO_FORMAT, DEF_AUDIO_CHANNELS, DEF_AUDIO_CHUNKSIZE) < 0)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error: Mix_OpenAudio", Mix_GetError(), 0);
			return 0;
		}
	}

	// SDL Net
	{
		if (SDLNet_Init() < 0)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error: SDLNet_Init", SDLNet_GetError(), 0);
			return 0;
		}

		SDL_NET_VERSION(&State->NetVersion);
	}

	// SDL Config
	if (SDL_JoystickEventState(SDL_ENABLE) != 1)
	{
		SDL_Log("MAIN: Failed to enable SDL_JoystickEventState: %s", SDL_GetError());
	}

	// Window
	State->Window.Dimensions.w = DEF_WINDOW_WIDTH;
	State->Window.Dimensions.h = DEF_WINDOW_HEIGHT;

	if (!Window_Init())
	{
		return 0;
	}

	State->Running = 1;

	SDL_Log("MAIN: Startup took %.02f ms", Timer_End(StartupTimer));

	return 1;
}

static void Main_Quit(void)
{
	if (State->Window.Renderer)
	{
		SDL_DestroyRenderer(State->Window.Renderer);
		State->Window.Renderer = 0;
	}

	if (State->Window.Handle)
	{
		SDL_DestroyWindow(State->Window.Handle);
		State->Window.Handle = 0;
	}

	SDLNet_Quit();
	Mix_CloseAudio();
	Mix_Quit();

	if ((SDL_WasInit(SDL_INIT_EVERYTHING)))
	{
		SDL_Quit();
	}

	State_Quit();
}

int main(int argc, char** argv)
{
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

		benderrect = (SDL_Rect*)State_malloc(sizeof(SDL_Rect) * 127);
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