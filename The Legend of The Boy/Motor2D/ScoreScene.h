#ifndef __j1SCORESCENE_H__
#define __j1SCORESCENE_H__

#include "j1Module.h"
#include "p2DynArray.h"
#include "p2List.h"
#include "SDL/include/SDL.h"

#define CREDLIMIT -1850

struct SDL_Texture;
class UIelement;

class j1ScoreScene : public j1Module
{
public:

	j1ScoreScene();

	virtual ~j1ScoreScene();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	bool Draw();
	

	//LICENSE
	p2List<UIelement*> license;
	UIelement*	Label = nullptr;


};

#endif // __j1SCORESCENE_H__
