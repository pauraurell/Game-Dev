#ifndef __j1CONSOLE_H__
#define __j1CONSOLE_H__

#include "j1Module.h"
#include "p2List.h"
#include "SDL/include/SDL.h"

#define OFFSET -1 / 2

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

	SDL_Rect	cons;
	bool		active;
	bool		input;
	p2List<SDL_Texture*> text;
	
};

#endif //__j1CONSOLE_H__
