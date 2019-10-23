#ifndef __j1INPUT_H__
#define __j1INPUT_H__

#include "j1Module.h"
#include "p2List.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50
#define JUMP_TIME 900
#define DASH_TIME 700

struct SDL_Rect;

enum player_states
{
	ST_IDLE,
	ST_WALK,
	ST_JUMP,
	ST_DASH,
	ST_FALL,
};

enum player_inputs
{
	IN_LEFT_LEFT,
	IN_LEFT_UP,
	IN_LEFT,
	IN_RIGHT_RIGHT,
	IN_RIGHT_UP,
	IN_RIGHT,
	IN_LEFT_RIGHT,
	IN_UP,
	IN_UP_FINISH,
	IN_UP_RIGHT_RIGHT,
	IN_UP_LEFT_LEFT,
	IN_DASH,
	IN_DASH_FINISH,
};

enum j1EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum j1KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class j1Input : public j1Module
{

public:

	j1Input();

	// Destructor
	virtual ~j1Input();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	bool external_input();
	void internal_input(p2List<player_inputs>& inputs);

	// Gather relevant win events
	bool GetWindowEvent(j1EventWindow ev);

	// Check key states (includes mouse and joy buttons)
	j1KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	j1KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	// Check if a certain window event happened
	bool GetWindowEvent(int code);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

private:
	bool		windowEvents[WE_COUNT];
	j1KeyState*	keyboard;
	j1KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	int			mouse_motion_x;
	int			mouse_motion_y;
	int			mouse_x;
	int			mouse_y;

public:
	p2List<player_inputs> inputs;
	int jump_timer = 0;
	int dash_timer = 0;
	bool left = false;
	bool right = false;
	bool up = false;
	bool playerinput = false;
};

#endif // __j1INPUT_H__