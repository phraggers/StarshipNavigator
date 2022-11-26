
#include "Starship_Navigator.h"

static bool _SetBasePath(std::string* _base_path)
{
	char* tmp_base_path = SDL_GetBasePath();

	if (!tmp_base_path)
	{
		SDL_Log("SDL_GetBasePath Error: %s", SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "GetBasePath Error",SDL_GetError(), 0);
		return 0;
	}

	*_base_path = std::string(tmp_base_path);
	SDL_free(tmp_base_path);

	SDL_Log("Base Path: %s", _base_path->c_str());

	return 1;
}

static bool _SetPrefPath(std::string* _pref_path, std::string* _org, std::string* _name)
{
	char* tmp_pref_path = SDL_GetPrefPath(_org->c_str(), _name->c_str());

	if (!tmp_pref_path)
	{
		SDL_Log("SDL_GetPrefPath Error: %s", SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "GetPrefPath Error", SDL_GetError(), 0);
		return 0;
	}

	*_pref_path = std::string(tmp_pref_path);
	SDL_free(tmp_pref_path);

	SDL_Log("Pref Path: %s", _pref_path->c_str());

	return 1;
}

static bool _SDLInit(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_Log("SDL_Init Error: %s", SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Init Error", SDL_GetError(), 0);
		return 0;
	}
	else
	{
		SDL_version ver = {};
		SDL_GetVersion(&ver);
		SDL_Log("SDL [%d.%d.%d]", ver.major, ver.minor, ver.patch);
	}

	if (SDL_JoystickEventState(SDL_ENABLE) != 1)
	{
		SDL_Log("SDL_JoystickEventState Error: %s", SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "JoystickEventState Error", SDL_GetError(), 0);
	}
	else
	{
		SDL_Log("SDL_JoystickEventState Enabled");
	}

	return 1;
}

static bool _SDLIMGInit(void)
{
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		SDL_Log("IMG_Init Error: %s", IMG_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "IMG_Init Error", IMG_GetError(), 0);
		return 0;
	}
	else
	{
		const SDL_version* ver = IMG_Linked_Version();
		if (ver)
		{
			SDL_Log("SDL_IMG [%d.%d.%d] (linked)", ver->major, ver->minor, ver->patch);
		}
		else
		{
			SDL_version img_ver = {};
			SDL_IMAGE_VERSION(&img_ver);
			SDL_Log("SDL_IMG [%d.%d.%d] (static)", img_ver.major, img_ver.minor, img_ver.patch);
		}
	}

	return 1;
}

static bool _SDLMixInit(int _freq, u16 _format, int _channels, int _chunksize)
{
	if (!Mix_Init(MIX_INIT_OGG))
	{
		SDL_Log("Mix_Init Error: %s", Mix_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Mix_Init Error", Mix_GetError(), 0);
		return 0;
	}
	else
	{
		SDL_version ver = {};
		MIX_VERSION(&ver);
		SDL_Log("SDL_Mixer [%d.%d.%d]", ver.major, ver.minor, ver.patch);
	}

	if (Mix_OpenAudio(_freq, _format, _channels, _chunksize) < 0)
	{
		SDL_Log("Mix_OpenAudio Error: %s", Mix_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Mix_OpenAudio Error", Mix_GetError(), 0);
		return 0;
	}
	else
	{
		int freq;
		u16 format;
		int channels;
		Mix_QuerySpec(&freq, &format, &channels);
		SDL_Log("AudioDevice Opened: freq[%d] format[%u] channels[%d]", freq, format, channels);
	}

	return 1;
}

static bool _SDLNetInit(void)
{
	if (SDLNet_Init() < 0)
	{
		SDL_Log("SDLNet_Init Error: %s", SDLNet_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDLNet_Init Error", SDLNet_GetError(), 0);
		return 0;
	}
	else
	{
		const SDLNet_version *ver = SDLNet_Linked_Version();
		if (ver)
		{
			SDL_Log("SDL_Net [%d.%d.%d] (linked)", ver->major, ver->minor, ver->patch);
		}
		else
		{
			SDLNet_version net_ver = {};
			SDL_NET_VERSION(&net_ver);
			SDL_Log("SDL_Net [%d.%d.%d] (static)", net_ver.major, net_ver.minor, net_ver.patch);
		}
	}

	return 1;
}

static bool _SDLTTFInit(void)
{
	if (TTF_Init() < 0)
	{
		SDL_Log("TTF_Init Error: %s", TTF_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "TTF_Init Error", TTF_GetError(), 0);
		return 0;
	}
	else
	{
		const SDL_version* ver = TTF_Linked_Version();
		if (ver)
		{
			SDL_Log("SDL_TTF [%d.%d.%d] (linked)", ver->major, ver->minor, ver->patch);
		}
		else
		{
			SDL_version ttf_ver = {};
			TTF_VERSION(&ttf_ver);
			SDL_Log("SDL_TTF [%d.%d.%d] (static)", ttf_ver.major, ttf_ver.minor, ttf_ver.patch);
		}
	}

	return 1;
}

Application::Application(void)
{
	running = 0;
	settings = {};
}

Application::~Application(void)
{
	TTF_Quit();
	SDLNet_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Application::Init(Application_Settings* _settings)
{
	Timer startup_timer;

	settings.name = std::string(_settings->name);
	settings.org = std::string(_settings->org);
	settings.version = _settings->version;
	for (int i = 0; i < _settings->args.size(); i++) settings.args.push_back(_settings->args[i]);
	settings.fps = _settings->fps;
	settings.vsync = _settings->vsync;
	settings.audio = _settings->audio;

	SDL_Log("%s [%d.%d.%d]", settings.name.c_str(), settings.version.major, settings.version.minor, settings.version.patch);

	if (!_SetBasePath(&base_path)) return 0;
	if (!_SetPrefPath(&pref_path, &settings.org, &settings.name)) return 0;
	if (!_SDLInit()) return 0;
	if (!_SDLIMGInit()) return 0;
	if (!_SDLMixInit(settings.audio.freq, settings.audio.format,
					 settings.audio.channels, settings.audio.chunksize)) return 0;
	if (!_SDLNetInit()) return 0;
	if (!_SDLTTFInit()) return 0;

	if (!video.NewWindow("main", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540}, 1)) return 0;
	{
		char window_title[256];
		SDL_snprintf(window_title, 256, "%s [%d.%d.%d]", settings.name.c_str(), settings.version.major, settings.version.minor, settings.version.patch);
		Window* main_window = video.GetWindow("main");
		if(main_window) main_window->SetTitle(window_title);
	}

	input.Init(&running, &video);
	frame_timer.Init(settings.fps);

	SDL_Log("Startup time: %.02f ms", startup_timer.Delta());

	return 1;
}