#ifndef __MODULEUI_H__
#define __MODULEUI_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class j1UI : public j1Module
{
public:

	j1UI();
	~j1UI();

	bool Start() override;
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;

	void Draw();

	int				pLife;
	SDL_Texture*	ui_tex = nullptr;
	SDL_Rect		heart;
	SDL_Rect		emptHeart;
};

#endif //__MODULEUI_H__