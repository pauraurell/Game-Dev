#ifndef __j1CREDITS_H__
#define __j1CREDITS_H__

#include "j1Module.h"
#include "p2DynArray.h"
#include "p2List.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;
class UIelement;

class j1Credits : public j1Module
{
public:

	j1Credits();

	virtual ~j1Credits();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	bool Draw();

	SDL_Rect	background;
	bool		enabled;
	int			vel;
	
	//LICENSE
	p2List<UIelement*> license;
	SDL_Texture*	licTex;
	UIelement*	Label = nullptr;

	//GITHUBS
	UIelement*	githubLabel1 = nullptr;
	UIelement*	githubLabel2 = nullptr;
	UIelement*	githubButton1 = nullptr;
	UIelement*	githubButton2 = nullptr;

};

#endif // __j1CREDITS_H__