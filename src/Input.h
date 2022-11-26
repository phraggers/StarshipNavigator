
#ifndef INPUT_H_
#define INPUT_H_

struct KeyState
{
	bool is_down;
	bool was_down;
	bool text_down;
};

struct ButtonState
{
	bool is_down;
	bool was_down;
};

enum MOUSEWHEEL
{
	MW_NONE,
	MW_UP,
	MW_DOWN,
	MW_LEFT,
	MW_RIGHT
};

struct Mouse //TODO: separate mouse state by window and global (desktop pos vs window pos)
{
	int x;
	int y;
	ButtonState left; // SDL_BUTTON_LEFT
	ButtonState right; // SDL_BUTTON_RIGHT
	ButtonState middle; // SDL_BUTTON_MIDDLE
	ButtonState x1; // SDL_BUTTON_X1
	ButtonState x2; // SDL_BUTTON_X2
	MOUSEWHEEL wheel; // enum MOUSEWHEEL
};

struct Controller
{
	SDL_GameController* handle;
	SDL_JoystickID id;
	bool connected;
	ButtonState a;
	ButtonState b;
	ButtonState x;
	ButtonState y;
	ButtonState back;
	ButtonState guide;
	ButtonState start;
	ButtonState left_stick;
	ButtonState right_stick;
	ButtonState left_shoulder;
	ButtonState right_shoulder;
	ButtonState dpad_up;
	ButtonState dpad_down;
	ButtonState dpad_left;
	ButtonState dpad_right;
	ButtonState misc1;
	ButtonState paddle1;
	ButtonState paddle2;
	ButtonState paddle3;
	ButtonState paddle4;
	ButtonState touchpad;
	i16 left_x;
	i16 left_y;
	i16 right_x;
	i16 right_y;
	i16 left_trigger;
	i16 right_trigger;
};

struct Input
{
	bool* running;
	Video* video;
	std::map<SDL_Keycode, KeyState> keys;
	Mouse mouse;
	std::map<int, Controller> controllers;
	i16 controller_deadzone;

	Input();
	void Init(bool* _running, Video* _video);
	void HandleEvents(void);

	bool KeySingle(SDL_Keycode _key);
	bool KeyDown(SDL_Keycode _key);
	bool KeyRepeat(SDL_Keycode _key);
	bool KeyUp(SDL_Keycode _key);

	bool MouseSingle(Uint8 _button);
	bool MouseDown(Uint8 _button);
	bool MouseUp(Uint8 _button);
	bool MouseWheel(MOUSEWHEEL _direction);

	bool ControllerBtnSingle(int _controller, SDL_GameControllerButton _button);
	bool ControllerBtnDown(int _controller, SDL_GameControllerButton _button);
	bool ControllerBtnUp(int _controller, SDL_GameControllerButton _button);
	i16 ControllerAxisRaw(int _controller, SDL_GameControllerAxis _axis);
	i16 ControllerAxisDZ(int _controller, SDL_GameControllerAxis _axis);
};

#endif // INPUT_H_