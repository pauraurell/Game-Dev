#ifndef __j1CONSOLE_H__
#define __j1CONSOLE_H__

#include "j1Module.h"
#include "p2List.h"
#include "SDL/include/SDL.h"

#define OFFSET -1 / 2
#define MAX_INPUT 4096

class UIelement;

class j1Console : public j1Module
{
public:

	j1Console();

	~j1Console();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void Log(const char* string);
	bool ConsoleInput();
	bool Commands(p2SString string);

	SDL_Rect	cons;
	bool		active;
	bool		input;
	p2List<SDL_Texture*> text;
	p2List<char*> consInput;
	p2SString	inputString;
	UIelement*	inputLabel;
	char	lastInput;
	char	inputChar[MAX_INPUT];

	//COMMANDS
	p2SString list;
	p2SString god_mode;
	p2SString quit;
	p2SString fps;
	p2SString map1;
	p2SString map2;
	p2SString photo_mode;

	
};

#endif //__j1CONSOLE_H__
