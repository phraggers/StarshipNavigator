#ifndef _SN_INPUT_H_
#define _SN_INPUT_H_

typedef struct
{
	SDL_Keycode Key;
	bool IsDown;
	bool WasDown;
	bool TextDown;
} SN_KeyState;

typedef struct
{
	bool IsDown;
	bool WasDown;
} SN_BtnState;

typedef enum
{
	MW_NONE,
	MW_UP,
	MW_DOWN,
	MW_LEFT,
	MW_RIGHT
} SN_MOUSEWHEEL;

typedef struct
{
	int X;
	int Y;
	SN_BtnState Left; // SDL_BUTTON_LEFT
	SN_BtnState Right; // SDL_BUTTON_RIGHT
	SN_BtnState Middle; // SDL_BUTTON_MIDDLE
	SN_BtnState X1; // SDL_BUTTON_X1
	SN_BtnState X2; // SDL_BUTTON_X2
	SN_MOUSEWHEEL MouseWheel; // enum SN_MOUSEWHEEL
} SN_Mouse;

typedef struct
{
	SDL_GameController* Controller;
	SDL_JoystickID ID;
	bool Connected;
	SN_BtnState A;
	SN_BtnState B;
	SN_BtnState X;
	SN_BtnState Y;
	SN_BtnState Back;
	SN_BtnState Guide;
	SN_BtnState Start;
	SN_BtnState LeftStick;
	SN_BtnState RightStick;
	SN_BtnState LeftShoulder;
	SN_BtnState RightShoulder;
	SN_BtnState DPadUp;
	SN_BtnState DPadDown;
	SN_BtnState DPadLeft;
	SN_BtnState DPadRight;
	SN_BtnState Misc1;
	SN_BtnState Paddle1;
	SN_BtnState Paddle2;
	SN_BtnState Paddle3;
	SN_BtnState Paddle4;
	SN_BtnState TouchPad;
	i16 LeftX;
	i16 LeftY;
	i16 RightX;
	i16 RightY;
	i16 LeftTrigger;
	i16 RightTrigger;
} SN_Controller;

typedef struct
{
	SN_KeyState Keys[DEF_INPUT_KEYCOUNT];
	SN_Mouse Mouse;
	SN_Controller Controllers[DEF_INPUT_CONTROLLERS];
} SN_Input;

void Input_HandleEvents(void);

bool Input_KeySingle(SDL_Keycode _Key);
bool Input_KeyDown(SDL_Keycode _Key);
bool Input_KeyRepeat(SDL_Keycode _Key);
bool Input_KeyUp(SDL_Keycode _Key);

bool Input_MouseSingle(Uint8 _Button);
bool Input_MouseDown(Uint8 _Button);
bool Input_MouseUp(Uint8 _Button);
bool Input_MouseWheel(SN_MOUSEWHEEL _Direction);

bool Input_ControllerBtnSingle(int _Controller, SDL_GameControllerButton _Button);
bool Input_ControllerBtnDown(int _Controller, SDL_GameControllerButton _Button);
bool Input_ControllerBtnUp(int _Controller, SDL_GameControllerButton _Button);
i16 Input_ControllerAxisRaw(int _Controller, SDL_GameControllerAxis _Axis);
i16 Input_ControllerAxisDZ(int _Controller, SDL_GameControllerAxis _Axis);

#endif //_SN_INPUT_H_