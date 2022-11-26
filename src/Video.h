
#ifndef VIDEO_H_
#define VIDEO_H_

struct Window
{
	std::string id;
	u32 sdl_window_id;
	SDL_Window* handle;
	SDL_Renderer* renderer;
	std::string title;
	SDL_Rect dim;
	bool exposed;
	bool minimized;
	bool maximized;
	bool mouse_focus;
	bool keyboard_focus;
	bool fullscreen;
	bool mouse_shown;

	Window(void);
	~Window(void);
	bool Init(std::string _title, SDL_Rect _dim, bool _vsync);
	void Resize(void);
	void SetDim(SDL_Rect _dim); // negative values are unchanged
	bool Fullscreen(void); // toggle, return new state
	bool Fullscreen(bool _fullscreen); // set state, return new state
	void SetTitle(std::string _title);
};

enum CursorStyle
{
	CURSOR_DEFAULT,
	CURSOR_SOLID,
	CURSOR_SOLID_INVERTED,
	CURSOR_HOLLOW,
	CURSOR_HOLLOW_INVERTED
};

struct Video
{
	std::map<std::string, Window*> windows;

	Video(void);
	~Video(void);
	bool NewWindow(std::string _id, SDL_Rect _dim, bool _vsync);
	bool DeleteWindow(std::string _id);
	Window* GetWindow(std::string _id);
	bool SetCursor(CursorStyle _style);
};

#endif // VIDEO_H_