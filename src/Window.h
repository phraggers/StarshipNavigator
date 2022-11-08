
#ifndef _SN_WINDOW_H_
#define _SN_WINDOW_H_

typedef struct
{
	char Title[64];
	SDL_Window* Handle;
	SDL_Renderer* Renderer;
	SDL_Rect Dimensions;
	SDL_Cursor* Cursor;
	bool Exposed;
	bool Minimized;
	bool Maximized;
	bool MouseFocus;
	bool KeyboardFocus;
	bool Fullscreen;
} SN_Window;

bool Window_Init(void);
void Window_Resize(void);
bool Window_ToggleFullscreen(void);
SDL_Cursor* Window_SDLCursor(void);

#endif //_SN_WINDOW_H_