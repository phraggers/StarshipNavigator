
#include "Starship_Navigator.h"

static bool _CreateWindow(SDL_Window** _handle, std::string _title, SDL_Rect _dim)
{
	*_handle = SDL_CreateWindow(_title.c_str(), _dim.x, _dim.y, _dim.w, _dim.h,
							   SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);

	if (!*_handle)
	{
		SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "CreateWindow Error", SDL_GetError(), 0);
		return 0;
	}
	else
	{
		SDL_Log("Window created: %s", _title.c_str());
		return 1;
	}
}

static bool _CreateRenderer(SDL_Window* _window, SDL_Renderer** _renderer, bool _vsync)
{
	if (_window)
	{
		if (_vsync)
		{
			*_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		}
		else
		{
			*_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
		}

		if (*_renderer)
		{
			SDL_Log("Renderer Created");
			return 1;
		}
		else
		{
			SDL_Log("SDL_CreateRenderer error: %s", SDL_GetError());
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "CreateRenderer Error", SDL_GetError(), 0);
			return 0;
		}
	}

	return 0;
}

static SDL_Cursor* _cursor;

bool _SDLCursor_Solid(void)
{
	if (_cursor)
	{
		SDL_FreeCursor(_cursor);
	}

	const char* cursor[] = {
		".                               ",
		"..                              ",
		".X.                             ",
		".XX.                            ",
		".XXX.                           ",
		".XXXX.                          ",
		".XXXXX.                         ",
		".XXXXXX.                        ",
		".XXXXXXX.                       ",
		".XXXXXXXX.                      ",
		".XXXXXXXXX.                     ",
		".XXXXXXXXXX.                    ",
		".XXXX..XXXXX.                   ",
		".XXXX. ...XXX.                  ",
		".XXX.     ...X.                 ",
		".XXX.        ..                 ",
		".XX.                            ",
		".X.                             ",
		".X.                             ",
		"..                              ",
		".                               ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"0,0"
	};

	int i, row, col;
	Uint8 data[4 * 32] = { 0 };
	Uint8 mask[4 * 32] = { 0 };
	int hot_x, hot_y;

	i = -1;
	for (row = 0; row < 32; ++row)
	{
		for (col = 0; col < 32; ++col)
		{
			if (col % 8)
			{
				data[i] <<= 1;
				mask[i] <<= 1;
			}
			else
			{
				++i;
				data[i] = mask[i] = 0;
			}
			switch (cursor[row][col])
			{
				case 'X':
					data[i] |= 0x01;
					mask[i] |= 0x01;
					break;
				case '.':
					mask[i] |= 0x01;
					break;
				case ' ':
					break;
			}
		}
	}

	if (SDL_sscanf(cursor[row], "%d,%d", &hot_x, &hot_y) > 0)
	{
		_cursor = SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
		if (!_cursor)
		{
			SDL_Log("SDL_CreateCursor error: %s", SDL_GetError());
			return 0;
		}
		if (SDL_ShowCursor(SDL_ENABLE) < 0)
		{
			SDL_Log("SDL_ShowCursor error: %s", SDL_GetError());
			return 0;
		}
		SDL_SetCursor(_cursor);
		return 1;
	}
	else
	{
		return 0;
	}
}

bool _SDLCursor_SolidInverted(void)
{
	if (_cursor)
	{
		SDL_FreeCursor(_cursor);
	}

	const char* cursor[] = {
		"X                               ",
		"XX                              ",
		"XXX                             ",
		"X..X                            ",
		"X...X                           ",
		"X....X                          ",
		"X.....X                         ",
		"X......X                        ",
		"X.......X                       ",
		"X........X                      ",
		"X.........X                     ",
		"X..........X                    ",
		"X....XX.....X                   ",
		"X....X XXX...X                  ",
		"X...X     XXX.X                 ",
		"X...X        XX                 ",
		"X..X                            ",
		"X.X                             ",
		"X.X                             ",
		"XX                              ",
		"X                               ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"0,0"
	};

	int i, row, col;
	Uint8 data[4 * 32] = { 0 };
	Uint8 mask[4 * 32] = { 0 };
	int hot_x, hot_y;

	i = -1;
	for (row = 0; row < 32; ++row)
	{
		for (col = 0; col < 32; ++col)
		{
			if (col % 8)
			{
				data[i] <<= 1;
				mask[i] <<= 1;
			}
			else
			{
				++i;
				data[i] = mask[i] = 0;
			}
			switch (cursor[row][col])
			{
				case 'X':
					data[i] |= 0x01;
					mask[i] |= 0x01;
					break;
				case '.':
					mask[i] |= 0x01;
					break;
				case ' ':
					break;
			}
		}
	}

	if (SDL_sscanf(cursor[row], "%d,%d", &hot_x, &hot_y) > 0)
	{
		_cursor = SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
		if (!_cursor)
		{
			SDL_Log("SDL_CreateCursor error: %s", SDL_GetError());
			return 0;
		}
		if (SDL_ShowCursor(SDL_ENABLE) < 0)
		{
			SDL_Log("SDL_ShowCursor error: %s", SDL_GetError());
			return 0;
		}
		SDL_SetCursor(_cursor);
		return 1;
	}
	else
	{
		return 0;
	}
}

bool _SDLCursor_Hollow(void)
{
	if (_cursor)
	{
		SDL_FreeCursor(_cursor);
	}

	const char* cursor[] = {
		".                               ",
		"..                              ",
		".X.                             ",
		".XX.                            ",
		".XXX.                           ",
		".XXXX.                          ",
		".XXXXX.                         ",
		".XX XXX.                        ",
		".XX  XXX.                       ",
		".XX   XXX.                      ",
		".XX    XXX.                     ",
		".XX XXXXXXX.                    ",
		".XXXX..XXXXX.                   ",
		".XXXX. ...XXX.                  ",
		".XXX.     ...X.                 ",
		".XXX.        ..                 ",
		".XX.                            ",
		".X.                             ",
		".X.                             ",
		"..                              ",
		".                               ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"0,0",
	};

	int i, row, col;
	Uint8 data[4 * 32] = { 0 };
	Uint8 mask[4 * 32] = { 0 };
	int hot_x, hot_y;

	i = -1;
	for (row = 0; row < 32; ++row)
	{
		for (col = 0; col < 32; ++col)
		{
			if (col % 8)
			{
				data[i] <<= 1;
				mask[i] <<= 1;
			}
			else
			{
				++i;
				data[i] = mask[i] = 0;
			}
			switch (cursor[row][col])
			{
				case 'X':
					data[i] |= 0x01;
					mask[i] |= 0x01;
					break;
				case '.':
					mask[i] |= 0x01;
					break;
				case ' ':
					break;
			}
		}
	}

	if (SDL_sscanf(cursor[row], "%d,%d", &hot_x, &hot_y) > 0)
	{
		_cursor = SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
		if (!_cursor)
		{
			SDL_Log("SDL_CreateCursor error: %s", SDL_GetError());
			return 0;
		}
		if (SDL_ShowCursor(SDL_ENABLE) < 0)
		{
			SDL_Log("SDL_ShowCursor error: %s", SDL_GetError());
			return 0;
		}
		SDL_SetCursor(_cursor);
		return 1;
	}
	else
	{
		return 0;
	}
}

bool _SDLCursor_HollowInverted(void)
{
	if (_cursor)
	{
		SDL_FreeCursor(_cursor);
	}

	const char* cursor[] = {
		"X                               ",
		"XX                              ",
		"XXX                             ",
		"X..X                            ",
		"X...X                           ",
		"X.  .X                          ",
		"X.   .X                         ",
		"X.    .X                        ",
		"X.     .X                       ",
		"X.      .X                      ",
		"X.       .X                     ",
		"X.   ..   .X                    ",
		"X.  .XX... .X                   ",
		"X.  .X XXX...X                  ",
		"X. .X     XXX.X                 ",
		"X. .X        XX                 ",
		"X..X                            ",
		"X.X                             ",
		"X.X                             ",
		"XX                              ",
		"X                               ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"                                ",
		"0,0",
	};

	int i, row, col;
	Uint8 data[4 * 32] = { 0 };
	Uint8 mask[4 * 32] = { 0 };
	int hot_x, hot_y;

	i = -1;
	for (row = 0; row < 32; ++row)
	{
		for (col = 0; col < 32; ++col)
		{
			if (col % 8)
			{
				data[i] <<= 1;
				mask[i] <<= 1;
			}
			else
			{
				++i;
				data[i] = mask[i] = 0;
			}
			switch (cursor[row][col])
			{
				case 'X':
					data[i] |= 0x01;
					mask[i] |= 0x01;
					break;
				case '.':
					mask[i] |= 0x01;
					break;
				case ' ':
					break;
			}
		}
	}

	if (SDL_sscanf(cursor[row], "%d,%d", &hot_x, &hot_y) > 0)
	{
		_cursor = SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
		if (!_cursor)
		{
			SDL_Log("SDL_CreateCursor error: %s", SDL_GetError());
			return 0;
		}
		if (SDL_ShowCursor(SDL_ENABLE) < 0)
		{
			SDL_Log("SDL_ShowCursor error: %s", SDL_GetError());
			return 0;
		}
		SDL_SetCursor(_cursor);
		return 1;
	}
	else
	{
		return 0;
	}
}

Window::Window(void)
{
	sdl_window_id = 0;
	handle = 0;
	renderer = 0;
	dim = {};
	exposed = 0;
	minimized = 0;
	maximized = 0;
	mouse_focus = 0;
	keyboard_focus = 0;
	fullscreen = 0;
	mouse_shown = 0;
}

Window::~Window(void)
{
	if (renderer)
	{
		SDL_Log("Renderer deleted: (window %s)", id.c_str());
		SDL_DestroyRenderer(renderer);
		renderer = 0;
	}

	if (handle)
	{
		SDL_Log("Window deleted: %s", id.c_str());
		SDL_DestroyWindow(handle);
		handle = 0;
	}
}

bool Window::Init(std::string _title, SDL_Rect _dim, bool _vsync)
{
	id = _title;
	if (!_CreateWindow(&handle, _title, _dim)) return 0;
	if (!_CreateRenderer(handle, &renderer, _vsync)) return 0;
	sdl_window_id = SDL_GetWindowID(handle);

	_SDLCursor_Solid();
	return 1;
}

void Window::Resize(void)
{
	SDL_Rect new_dim = {};
	SDL_GetWindowSize(handle, &new_dim.w, &new_dim.h);
	SDL_GetWindowPosition(handle, &new_dim.x, &new_dim.y);

	if (new_dim.x != dim.x) dim.x = new_dim.x;
	if (new_dim.y != dim.y) dim.y = new_dim.y;
	if (new_dim.w != dim.w) dim.w = new_dim.w;
	if (new_dim.h != dim.h) dim.h = new_dim.h;
}

void Window::SetDim(SDL_Rect _dim)
{
	dim.x = ((_dim.x < 0) ? dim.x : _dim.x);
	dim.y = ((_dim.y < 0) ? dim.y : _dim.y);
	dim.w = ((_dim.w < 0) ? dim.w : _dim.w);
	dim.h = ((_dim.h < 0) ? dim.h : _dim.h);

	SDL_SetWindowSize(handle, dim.w, dim.h);
	SDL_SetWindowPosition(handle, dim.x, dim.y);
}

bool Window::Fullscreen(void)
{
	if (fullscreen)
	{
		SDL_SetWindowFullscreen(handle, 0);
		fullscreen = 0;
	}
	else
	{
		SDL_SetWindowFullscreen(handle, SDL_WINDOW_FULLSCREEN_DESKTOP);
		fullscreen = 1;
	}

	return fullscreen;
}

bool Window::Fullscreen(bool _fullscreen)
{
	if (_fullscreen)
	{
		SDL_SetWindowFullscreen(handle, 0);
		fullscreen = 0;
	}
	else
	{
		SDL_SetWindowFullscreen(handle, SDL_WINDOW_FULLSCREEN_DESKTOP);
		fullscreen = 1;
	}

	return fullscreen;
}

void Window::SetTitle(std::string _title)
{
	title = _title;
	SDL_SetWindowTitle(handle, _title.c_str());
}

Video::Video(void)
{
}

Video::~Video(void)
{
	std::vector<std::string> window_list;

	for (auto i : windows)
	{
		window_list.push_back(i.first);
	}

	for (int i = 0; i < window_list.size(); i++)
	{
		DeleteWindow(window_list[i]);
	}
}

bool Video::NewWindow(std::string _id, SDL_Rect _dim, bool _vsync)
{
	for (auto i : windows)
	{
		if (_id.compare(i.first) == 0)
		{
			SDL_Log("Video::NewWindow error: window with id %s exists", _id.c_str());
			return 0;
		}
	}

	windows[_id] = new Window;

	if (windows[_id])
	{
		if (windows[_id]->Init(_id, _dim, _vsync))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		windows.erase(_id);
		return 0;
	}
}

bool Video::DeleteWindow(std::string _id)
{
	for (auto i : windows)
	{
		if (_id.compare(i.first) == 0)
		{
			delete windows[_id];
			windows.erase(_id);
			return 1;
		}
	}

	SDL_Log("Video::DeleteWindow error: window id %s not found", _id.c_str());
	return 0;
}

Window* Video::GetWindow(std::string _id)
{
	for (auto i : windows)
	{
		if (_id.compare(i.first) == 0)
		{
			return i.second;
		}
	}

	return 0;
}

bool Video::SetCursor(CursorStyle _style)
{
	switch (_style)
	{		
		case CURSOR_SOLID:
		{
			return _SDLCursor_Solid();
		} break;

		case CURSOR_SOLID_INVERTED:
		{
			return _SDLCursor_SolidInverted();
		} break;

		case CURSOR_HOLLOW:
		{
			return _SDLCursor_Hollow();
		} break;

		case CURSOR_HOLLOW_INVERTED:
		{
			return _SDLCursor_HollowInverted();
		} break;

		case CURSOR_DEFAULT:
		default:
		{
			if (_cursor)
			{
				SDL_FreeCursor(_cursor);
			}

			SDL_SetCursor(0);
			return 1;
		} break;
	}

	return 0;
}