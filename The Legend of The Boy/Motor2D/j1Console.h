#ifndef __j1CONSOLE_H__
#define __j1CONSOLE_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"

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

	SDL_Rect	cons;
	bool		active;
	bool		input;
	SDL_Texture* tex;
	
};

#endif //__j1CONSOLE_H__
