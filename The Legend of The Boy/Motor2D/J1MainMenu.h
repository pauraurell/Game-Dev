#ifndef __j1MAINMENU_H__
#define __j1MAINMENU_H__

#include "j1Module.h"
#include "p2DynArray.h"

struct SDL_Texture;
class UIelement;

class j1MainMenu : public j1Module
{
public:

	j1MainMenu();

	virtual ~j1MainMenu();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	bool Draw();

	SDL_Texture* texture = nullptr;

	UIelement* testButton = nullptr;
	UIelement* exitButton = nullptr;
	UIelement* creditsButton = nullptr;

	void UIevents(uiEvent type, UIelement* element);

};

#endif // __j1INTROSCENE_H__