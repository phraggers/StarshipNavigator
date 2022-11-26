
#include "Starship_Navigator.h"

int main(int argc, char** argv)
{
	Application* app = new Application;

	if (app)
	{
		Application_Settings settings = {};
		settings.name = "Starship_Navigator";
		settings.org = "Phragware";
		settings.version = { 0,0,1 };
		for (int i = 0; i < argc; i++) settings.args.push_back(argv[i]);
		settings.audio.freq = 48000;
		settings.audio.format = MIX_DEFAULT_FORMAT;
		settings.audio.channels = 2;
		settings.audio.chunksize = 1024;
		settings.fps = 60;
		settings.vsync = 1;

		app->running = app->Init(&settings);
		app->video.NewWindow("test", { 100,100,300,300 }, 1);

		while (app->running)
		{
			app->frame_timer.FrameStart();
			app->input.HandleEvents();
			
			Window* window = app->video.GetWindow("main");
			if (window)
			{
				SDL_SetRenderDrawColor(window->renderer, 0, 0, 0xaa, 0xff);
				SDL_RenderClear(window->renderer);
				SDL_RenderPresent(window->renderer);
			}

			Window* window2 = app->video.GetWindow("test");
			if (window2)
			{
				SDL_SetRenderDrawColor(window2->renderer, 0, 0xaa, 0, 0xff);
				SDL_RenderClear(window2->renderer);
				SDL_RenderPresent(window2->renderer);
			}

			app->frame_timer.FrameEnd();
		}

		delete app;
	}
	else return -1;

	return 0;
}
