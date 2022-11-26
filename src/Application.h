
#ifndef APPLICATION_H_
#define APPLICATION_H_

struct Application_Settings
{
	std::string name;
	std::string org;
	struct { i8 major, minor, patch; } version;
	std::vector<std::string> args;
	struct { int freq; u16 format; int channels; int chunksize; } audio;
	u32 fps;
	bool vsync;
};

struct Application
{
	bool running;
	Application_Settings settings;
	std::string base_path;
	std::string pref_path;
	Video video;
	Input input;
	FrameTimer frame_timer;

	Application(void);
	~Application(void);
	bool Init(Application_Settings* _settings);
};

#endif // APPLICATION_H_